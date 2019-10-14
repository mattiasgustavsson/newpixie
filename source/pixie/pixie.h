/*
------------------------------------------------------------------------------
          Licensing information can be found at the end of the file.
------------------------------------------------------------------------------

pixie.h - v0.1 - Retro game dev library for C/C++.

Do this:
    #define PIXIE_IMPLEMENTATION
before you include this file in *one* C/C++ file to create the implementation.
*/


#ifndef pixie_h
#define pixie_h

#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
namespace pixie {
#endif

// Sized types
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;


// API

int run( int (*main)( int, char** ) );
void end( int return_code );
void wait_vbl( void );

void print( char const* str );
void load_palette( char const* string );
void load_sprite( int data_index, char const* filename );
void sprite( int spr_index, int x, int y, int data_index );
void sprite_pos( int spr_index, int x, int y );

#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
} /* namespace pixie */
#endif

#endif /* pixie_h */


/*
----------------------
    IMPLEMENTATION
----------------------
*/

#ifdef PIXIE_IMPLEMENTATION
#undef PIXIE_IMPLEMENTATION

// C runtime includes
#define _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

// Libaries includes
#include "app.h"
#include "crtemu.h"
#include "crt_frame.h"
#include "frametimer.h"
#include "palettize.h"
#include "pixie_data.h"
#include "stb_image.h"
#include "thread.h"


#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
namespace pixie {
#endif

/*
----------------
    INTERNALS
----------------
*/

// In C, a void* can be implicitly cast to any other kind of pointer, while in C++ you need an explicit cast. In most
// cases, the explicit cast works for both C and C++, but if we consider the case where we have nested structs, then
// the way you refer to them differs between C and C++ (in C++, `parent_type::nested_type`, in C just `nested_type`).
// In addition, with the automatic cast in C, it is possible to use unnamed nested structs and still dynamically 
// allocate arrays of that type - this would be desirable when the code is compiled from C++ as well.
// This VOID_CAST macro allows for automatic cast from void* in C++. In C, it does nothing, but for C++ it uses a simple
// template function to define a cast-to-anything operator.
// Use like this:
//      struct {
//          struct {
//              int x;
//          } *nested;
//      } parent;
//      parent.nested = VOID_CAST( malloc( sizeof( *parent.nested ) * count ) );
//
#ifdef __cplusplus
    struct void_cast {   
        inline void_cast( void* x_ ) : x( x_ ) { }
        template< typename U > inline operator U() { return (U)x; } // cast to whatever requested
        void* x;
    };
    #define VOID_CAST( x ) void_cast( x )
#else
    #define VOID_CAST( x ) x
#endif


// Main engine state - *everything* is stored here, and data is accessed from both the app thread and the user thread
// The instance is created within the `run` function, and a pointer to it is stored in thread local storage for the
// user thread, so that every API method can access it to perform its function. The app thread gets a pointer to it 
// through the user_data parameter to the app_proc.

typedef struct pixie_t {
    struct {
        jmp_buf exit_jump; // Jump target set in `run` function, to jump back to when `end` is called
        thread_atomic_int_t force_exit; // Signals (when set to `INT_MAX`) that user thread should jump to `exit_jump`
    } exit;

    struct {
        thread_signal_t signal; // Raised by app thread when a frame is finished and the next frame is starting
        thread_atomic_int_t count; // Incremented for every new frame
    } vbl;

    struct { 
        thread_mutex_t mutex;
        u32 palette[ 256 ];
        u8* pixels;
        u8* composite;
        u32* xbgr;
        int width;
        int height;
    } screen;

    struct {
        thread_mutex_t mutex;
        int sprite_count;
        struct
            {
            int x;
            int y;
            int data;
            }* sprites;

        int data_count;
        struct {
            u8* pixels;
            int width;
            int height;
        }* data;
    } sprites;
} pixie_t;


// Forward declares

static u32* pixie_render_screen( pixie_t* pixie, int* width, int* height );


// A global atomic pointer to the TLS instance for storing per-thread `pixie_t` pointers. Created in the `run` function
// unless it has already been created (through a compare-and-swap). The `run` method then sets the TLS value on the 
// instance, for the current thread.

static thread_atomic_ptr_t g_tls_pixie = { NULL }; 


// Retrieves the pointer to the `pixie_t` state for the current thread, as stored in the `g_tls_pixie` TLS storage
// As this has to be called in every API function, it was also a convenient place to check if the app thread is 
// requesting a shutdown of the program, and in that case do a forced exit (using `longjmp`) no matter how deep down a
// call stack `pixie_instance` was called from.

static pixie_t* pixie_instance( void ) { 
    // Get the `pixie_t` pointer for this thread from the global TLS instance `g_tls_pixie`
    pixie_t*  pixie = (pixie_t*) thread_tls_get( thread_atomic_ptr_load( &g_tls_pixie ) );

    // Check if app thread is requesting a forced exit (the user closed the window) and if so, call the exit point
    int force_exit = thread_atomic_int_load( &pixie->exit.force_exit );
    if( force_exit ) longjmp( pixie->exit.exit_jump, force_exit );

    return pixie; 
}


// Holds the data needed for communicating between user thread and app thread. Most of this will be in pixie, but there
// are also a few things which only needs to be accessed from within the `run` function and the app thread.

typedef struct app_context_t {
    pixie_t* pixie; // Main engine state
    thread_atomic_int_t exit_flag; // Set to 1 by `run` function on user thread to indicate app thread should terminate
    thread_signal_t init_complete; // Raised by app thread before entering main loop, to indicate `run` may continue
} app_context_t;


// Main body for the app thread (invoked by calling `app_run` in the `app_thread` entry point, invoked from `run`).
// The app thread runs independently from the user thread, and handles the main window, rendering, audio and input.
// After all initialization, and just before entering the main loop, it will raise the `init_complete` signal, which 
// lets the user thread (in the `run` function) know that it is safe to call the users entry point.
// Every iteration through the main loop, the signal `vbl.signal` is raised, and the `vbl.count` value is incremented.
// These are used by the `wait_vbl` function to pause the user thread until the start of the next frame.
// If the main window is closed (by clicking on the close button), the app thread sets the `force_exit` value to
// `INT_MAX`, to signal to the user thread that it should exit the user code and terminate. The `force_exit` value is
// checked in the `pixie_instance` function, which is called at the start of every API call.

static int app_proc( app_t* app, void* user_data ) {
    app_context_t* context = (app_context_t*) user_data;
    pixie_t* pixie = context->pixie;

    // Set up initial app parameters
    //app_screenmode( app, APP_SCREENMODE_WINDOW );
    app_interpolation( app, APP_INTERPOLATION_NONE );

    // Create and set up the CRT emulation instance
    crtemu_t* crtemu = crtemu_create( NULL );
    CRTEMU_U64 crt_time_us = 0;
    CRT_FRAME_U32* frame = (CRT_FRAME_U32*) malloc( sizeof( CRT_FRAME_U32 ) * CRT_FRAME_WIDTH * CRT_FRAME_HEIGHT );
    crt_frame( frame );
    crtemu_frame( crtemu, frame, CRT_FRAME_WIDTH, CRT_FRAME_HEIGHT );
    free( frame );

    // Create the frametimer instance, and set it to fixed 60hz update. This will ensure we never run faster than that,
    // even if the user have disabled vsync in their graphics card settings.
    frametimer_t* frametimer = frametimer_create( NULL );
    frametimer_lock_rate( frametimer, 60 );

    // Signal to the `run` function on the user thread that app initialization is complete, and it can start running
    thread_signal_raise( &context->init_complete );

    // Main loop
    APP_U64 prev_time = app_time_count( app );       
    while( !thread_atomic_int_load( &context->exit_flag ) ) {
        // Run app update (reading inputs etc)
        app_state_t app_state = app_yield( app );
        
        // Check if the close button on the window was clicked (or Alt+F4 was pressed)
        if( app_state == APP_STATE_EXIT_REQUESTED ) {
            // Signal that we need to force the user thread to exit
            thread_atomic_int_store( &pixie->exit.force_exit, INT_MAX );
            
            // In case user thread is in `wait_vbl` loop, exit it immediately so we don't have to wait for it to timeout
            thread_atomic_int_add( &pixie->vbl.count, 1 );
            thread_signal_raise( &pixie->vbl.signal );    
            break; 
        }

        int screen_width = 0;
        int screen_height = 0;
        APP_U32* xbgr = pixie_render_screen( pixie, &screen_width, &screen_height );
    
        // Signal to the game that the frame is completed, and that we are just starting the next one
        thread_atomic_int_add( &pixie->vbl.count, 1 );
        thread_signal_raise( &pixie->vbl.signal );    

        // Present the screen buffer to the window
        APP_U64 time = app_time_count( app );
        APP_U64 delta_time_us = ( time - prev_time ) / ( app_time_freq( app ) / 1000000 );
        prev_time = time;
        crt_time_us += delta_time_us;
        crtemu_present( crtemu, crt_time_us, xbgr, screen_width, screen_height, 0xffffff, 0x1c1c1c );
        app_present( app, NULL, 1, 1, 0xffffff, 0x000000 );

        // Ensure we don't run faster than 60 frames per second
        frametimer_update( frametimer );
    }

    frametimer_destroy( frametimer );
    crtemu_destroy( crtemu );
    return 0;
}


// Entry point for the app thread, which is started from the `run` function. Just runs the app with the above `app_proc` 

static int app_thread( void* user_data ) {
    return app_run( app_proc, user_data, NULL, NULL, NULL );
}


// Create the instance for holding the main engine state. Called from `run` before app thread is started.

static pixie_t* pixie_create( void ) {
    // Allocate the state and clear it, to avoid uninitialized varible problems
    pixie_t* pixie = (pixie_t*) malloc( sizeof( pixie_t ) );
    memset( pixie, 0, sizeof( *pixie ) );

    // Set up `exit` field. The `exit_jump` field is initialized from the `run` function at the desired point
    thread_atomic_int_store( &pixie->exit.force_exit, 0 ); 

    // Set up `vbl` field
    thread_signal_init( &pixie->vbl.signal );
    thread_atomic_int_store( &pixie->vbl.count, 0 );

    // Set up the screen 
    memcpy( pixie->screen.palette, default_palette(), sizeof( pixie->screen.palette ) );
    thread_mutex_init( &pixie->screen.mutex );
    pixie->screen.width = 384;
    pixie->screen.height = 288;
    pixie->screen.pixels = (u8*) malloc( sizeof( u8 ) * pixie->screen.width * pixie->screen.height );
    memset( pixie->screen.pixels, 0, sizeof( u8 ) * pixie->screen.width * pixie->screen.height );
    pixie->screen.composite = (u8*) malloc( sizeof( u8 ) * pixie->screen.width * pixie->screen.height );
    memset( pixie->screen.composite, 0, sizeof( u8 ) * pixie->screen.width * pixie->screen.height );
    pixie->screen.xbgr = (u32*) malloc( sizeof( u32 ) * pixie->screen.width * pixie->screen.height );
    memset( pixie->screen.xbgr, 0, sizeof( u32 ) * pixie->screen.width * pixie->screen.height );

    // Set up sprites
    thread_mutex_init( &pixie->sprites.mutex );
    pixie->sprites.sprite_count = 256;
    pixie->sprites.sprites = VOID_CAST( malloc( sizeof( *pixie->sprites.sprites ) * pixie->sprites.sprite_count ) );
    memset( pixie->sprites.sprites, 0, sizeof( *pixie->sprites.sprites ) * pixie->sprites.sprite_count );
    pixie->sprites.data_count = 4096;
    pixie->sprites.data = VOID_CAST( malloc( sizeof( *pixie->sprites.data ) * pixie->sprites.data_count ) );
    memset( pixie->sprites.data, 0, sizeof( *pixie->sprites.data ) * pixie->sprites.data_count );

    return pixie;
}


// Destroy the specified pixie instance. Called by `run` function, after app thread has finished.

static void pixie_destroy( pixie_t* pixie ) {
    // Cleanup `vbl` field
    thread_signal_term( &pixie->vbl.signal );

    // Cleanup screen
    free( pixie->screen.xbgr );
    free( pixie->screen.composite );
    free( pixie->screen.pixels );
    thread_mutex_term( &pixie->screen.mutex );

    // Cleanup sprites
    free( pixie->sprites.sprites );
    for( int i = 0; i < pixie->sprites.data_count; ++i ) 
        if( pixie->sprites.data[ i ].pixels ) free( pixie->sprites.data[ i ].pixels );
    free( pixie->sprites.data );
    thread_mutex_term( &pixie->sprites.mutex );

    free( pixie );
}


static u32* pixie_render_screen( pixie_t* pixie, int* width, int* height )
    {
    // Make a copy of the screen so we can draw sprites on top of it
    thread_mutex_lock( &pixie->screen.mutex );
    memcpy( pixie->screen.composite, pixie->screen.pixels, sizeof( u8 ) * pixie->screen.width * pixie->screen.height );
    thread_mutex_unlock( &pixie->screen.mutex );
    
    // Draw sprites
    thread_mutex_lock( &pixie->sprites.mutex );
    for( int i = 0; i < pixie->sprites.sprite_count; ++i )
        {    
        int data_index = pixie->sprites.sprites[ i ].data;
        if( data_index < 1 || data_index > pixie->sprites.data_count ) continue;
        --data_index;
        if( !pixie->sprites.data[ data_index ].pixels ) continue;        
        for( int y = 0; y < pixie->sprites.data[ data_index ].height; ++y )
            {
            for( int x = 0; x < pixie->sprites.data[ data_index ].width; ++x )
                {
                u8 p = pixie->sprites.data[ data_index ].pixels[ x + y * pixie->sprites.data[ data_index ].width ];
                if( ( p & 0x80 ) == 0 )
                    {
                    int xp = pixie->sprites.sprites[ i ].x + x;
                    int yp = pixie->sprites.sprites[ i ].y + y;
                    if( xp >= 0 && yp >= 0 && xp < pixie->screen.width && yp < pixie->screen.height )
                        pixie->screen.composite[ xp + yp * pixie->screen.width ] = p;                    
                    }
                }
            }
        }
    thread_mutex_unlock( &pixie->sprites.mutex );


    // Render screen
    for( int y = 0; y < pixie->screen.height; ++y )
        for( int x = 0; x < pixie->screen.width; ++x )
            pixie->screen.xbgr[ x + y * pixie->screen.width ] = 
                pixie->screen.palette[ pixie->screen.composite[ x + y * pixie->screen.width ] ];

    *width = pixie->screen.width;
    *height = pixie->screen.height;
    return pixie->screen.xbgr;
    }



/*
--------------------
    API FUNCTIONS
--------------------
*/


// The main starting point for a Pixie program. Called automatically from the standard C main function defined below,
// unless `PIXIE_NO_MAIN` has been defined, in which case the user will have to define their own `main` function and
// call `run` from it, passing in their pixie main function to it. The `run` function creates the `pixie_t` engine state
// and stores a pointer to it in thread local storage. It creates the app thread, and waits for it to be initialized
// before calling the users pixie main function (called `pixmain` if the built-in main is used). It also uses `setjmp`
// to define a jump point in order to be able to exit from the user code no matter where in the call stack it is, and 
// still get back to the `run` function to perform a controlled shutdown.

int run( int (*main)( int, char** ) ) {
    // Create and initialize the main engine state used by all of pixie
    pixie_t* pixie = pixie_create();

    // A pointer to the `pixie_t` instance needs to be stored in thread local storage, and before we do, we must create
    // the TLS instance. But only if it is not already created, and as there's nothing stopping a user from calling the
    // `run` function from two parallel threads at the same time, we must avoid a race condition. We do this by always
    // creating a new TLS instance, and then we do a compare-and-swap with the global instance, to only swap if it is 
    // not already set. If the swap does not return NULL (thus indicating it already held a value), then we destroy the
    // TLS instance we just created, as it will no longer be needed (we'll have to use the global one instead).
    thread_tls_t pixie_tls = thread_tls_create();
    if( thread_atomic_ptr_compare_and_swap( &g_tls_pixie, NULL, pixie_tls ) ) thread_tls_destroy( pixie_tls );
    
    // Store the `pixie_t` pointer in the thread local storage for the current thread. It will be retrieved by all API
    // functions so that we don't have to pass around an instance parameter to them.
    thread_tls_set( thread_atomic_ptr_load( &g_tls_pixie ), pixie );
    
    // Define the `app_context_t` instance which will be shared between `run` function and `app_proc` thread.
    app_context_t app_context = { NULL };
    app_context.pixie = pixie;
    thread_atomic_int_store( &app_context.exit_flag, 0 );
    thread_signal_init( &app_context.init_complete );
    
    // Start the app thread. The entry point `app_thread` will just run `app_proc`
    thread_ptr_t thread = thread_create( app_thread, &app_context, NULL, THREAD_STACK_SIZE_DEFAULT );
    
    // Wait until the app thread have completed its initialization. If it takes more than 5 seconds, we just quit.
    int result = thread_signal_wait( &app_context.init_complete, 5000 ) ? EXIT_SUCCESS : EXIT_FAILURE;
    if( result == EXIT_SUCCESS ) {
        // Set up the jump target for the `end` function
        #pragma warning( push )
        #pragma warning( disable: 4611 ) // interaction between '_setjmp' and C++ object destruction is non-portable
        int jumpres = setjmp( pixie->exit.exit_jump );
        #pragma warning( pop )

        // Run the user provided entry point (will be `pixmain` unless `PIXIE_NO_MAIN` was defined)
        if( jumpres == 0 ) // First time we get here we call `main`. If `exit_jump` was jumped to we will get here again
            result = main( __argc, __argv );
        else // Second time we save the result (`INT_MAX` is mapped to 0, as a jumpres of 0 would call main again)
            result = ( result == INT_MAX ? EXIT_SUCCESS : result );
    }

    // Terminate the app thread
    thread_atomic_int_store( &app_context.exit_flag, 1 );
    thread_join( thread );
    thread_signal_term( &app_context.init_complete );
    
    // Destroy pixie instance, and clear the thread local pointer to it
    pixie_destroy( pixie );
    thread_tls_set( thread_atomic_ptr_load( &g_tls_pixie ), NULL );

    return result;
}


// Terminates the user code, performing a controlled shutdown of the engine. Can be called from deep down a call stack
// as it will use `longjmp` to branch back to the top level of the `run` function.

void end( int return_code ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage
   
    // Jump back into the middle of the `run` function, to where `setjmp` was called, regardless of where we called from
    longjmp( pixie->exit.exit_jump, return_code );
}


// Waits until the start of the next frame. 

void wait_vbl( void ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    // Get the vbl count before we start - we want to wait until it has changed
    int current_vbl_count = thread_atomic_int_load( &pixie->vbl.count );

    // Since signals might suffer spurious wakeups, we want to loop until we get a new vbl count
    while( current_vbl_count == thread_atomic_int_load( &pixie->vbl.count ) ) {
        // Wait until app thread says there is a new frame, or timeout after one second.
        thread_signal_wait( &pixie->vbl.signal, 1000 );

        // Call `pixie_intance` again, just to trigger the check for `force_exit`, so we can terminate if need be
        pixie_instance();
    }
}


// Prints the specified string to the screen using the default font.

void print( char const* str ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    // Very placeholder font rendering
    thread_mutex_lock( &pixie->screen.mutex );
    static int x = 32;
    static int y = 44;
    while( *str ) {
        unsigned long long chr = default_font()[ (u8) *str++ ];
        for( int iy = 0; iy < 8; ++iy )
            for( int ix = 0; ix < 8; ++ix )
                if( chr & ( 1ull << ( ix + iy * 8 ) ) )
                    pixie->screen.pixels[ x + ix + ( y + iy ) * pixie->screen.width ] = 10; 
        x += 8;
        if( x - 32 >= 320 ) {
            x = 32;
            y += 8;
        }
    }
    x = 32;
    y += 8;
    thread_mutex_unlock( &pixie->screen.mutex );
}


void load_palette( char const* string ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    int w, h, c;
    stbi_uc* img = stbi_load( string, &w, &h, &c, 4 );
    if( !img ) return;

    u32 palette[ 256 ] = { 0 };
    int count = 0;      
    for( int y = 0; y < h; ++y ) {
        for( int x = 0; x < w; ++x ) {
            u32 pixel = ((u32*)img)[ x + y * w ];
            if( ( pixel & 0xff000000 ) == 0 ) goto skip;
            if( count < 256 ) {
                for( int i = 0; i < count; ++i ) {
                    if( palette[ i ] == pixel )
                        goto skip;
                }
                palette[ count ] = pixel;       
            }
            ++count;
        skip:
            ;
        }
    }   
    if( count > 256 )  {
        memset( palette, 0, sizeof( palette ) );
        count = palettize_generate_palette_xbgr32( (PALETTIZE_U32*) img, w, h, palette, 256, 0 );        
    }
    stbi_image_free( img );     
    memcpy( pixie->screen.palette, palette, sizeof( palette ) );
}


void load_sprite( int data_index, char const* filename ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( data_index < 1 || data_index > pixie->sprites.data_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return;
    }
    --data_index;

    if( pixie->sprites.data[ data_index ].pixels )
        {
        free( pixie->sprites.data[ data_index ].pixels );
        pixie->sprites.data[ data_index ].pixels = NULL;
        pixie->sprites.data[ data_index ].width = 0;
        pixie->sprites.data[ data_index ].height = 0;
        }

    int w, h, c;
    stbi_uc* img = stbi_load( filename, &w, &h, &c, 4 );
    if( !img ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return;
    }
   
    pixie->sprites.data[ data_index ].width = w;
    pixie->sprites.data[ data_index ].height = h;
    pixie->sprites.data[ data_index ].pixels = (u8*) malloc( sizeof( u8 ) * w * h );
    memset( pixie->sprites.data[ data_index ].pixels, 0, sizeof( u8 ) * w * h ); 
    palettize_remap_xbgr32( (PALETTIZE_U32*) img, w, h, pixie->screen.palette, 32, pixie->sprites.data[ data_index ].pixels );
    
    for( int i = 0; i < w * h; ++i )
        if( ( ( (PALETTIZE_U32*) img )[ i ] & 0xff000000 ) >> 24 < 0x80 )
            pixie->sprites.data[ data_index ].pixels[ i ] |=  0x80u;       
    
    stbi_image_free( img );     
    thread_mutex_unlock( &pixie->sprites.mutex );
}


void sprite( int spr_index, int x, int y, int data_index ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( data_index < 1 || data_index > pixie->sprites.data_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return;
    }
    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
    }
    if( !pixie->sprites.data[ data_index - 1 ].pixels ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return;
    }
    --spr_index;
    pixie->sprites.sprites[ spr_index ].data = data_index;
    pixie->sprites.sprites[ spr_index ].x = x;
    pixie->sprites.sprites[ spr_index ].y = y;
    thread_mutex_unlock( &pixie->sprites.mutex );
}


void sprite_pos( int spr_index, int x, int y ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
    }

    --spr_index;
    pixie->sprites.sprites[ spr_index ].x = x;
    pixie->sprites.sprites[ spr_index ].y = y;
    thread_mutex_unlock( &pixie->sprites.mutex );
}


#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
} /* namespace pixie */
#endif


/*
-------------------------
    ENTRY POINT (MAIN)
-------------------------
*/
     

#ifndef PIXIE_NO_MAIN

    #ifdef _WIN32
        #ifndef NDEBUG
            #pragma warning( push ) 
            #pragma warning( disable: 4619 ) // pragma warning : there is no warning number 'number'
            #pragma warning( disable: 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
            #include <crtdbg.h>
            #pragma warning( pop ) 
        #endif
    #endif /* _WIN32 */

    int pixmain( int argc, char** argv );

    int main( int argc, char** argv ) {
        (void) argc, argv;
        #ifdef _WIN32
            #ifndef NDEBUG
                int flag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ); // Get current flag
                flag ^= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
                _CrtSetDbgFlag( flag ); // Set flag to the new value
                //_CrtSetBreakAlloc( 0 );
            #endif
        #endif

        return run( pixmain );
    } 

    #ifdef _WIN32
        // pass-through so the program will build with either /SUBSYSTEM:WINDOWS or /SUBSYSTEN:CONSOLE
        struct HINSTANCE__;
        #ifdef __cplusplus
        extern "C" {
        #endif
            int __stdcall WinMain( struct HINSTANCE__* a, struct HINSTANCE__* b, char* c, int d ) { (void) a, b, c, d; return main( __argc, __argv ); }
        #ifdef __cplusplus
        }
        #endif
    #endif /* _WIN32 */

#endif /* PIXIE_NO_MAIN */


/*
--------------------------------
    LIBRARIES IMPLEMENTATIONS
--------------------------------
*/
      

#define APP_IMPLEMENTATION
#ifdef _WIN32
    #define APP_WINDOWS
#endif
#include "app.h"

#define CRTEMU_IMPLEMENTATION
#include "crtemu.h"

#define CRT_FRAME_IMPLEMENTATION
#include "crt_frame.h"

#define FRAMETIMER_IMPLEMENTATION
#include "frametimer.h"

#define PALETTIZE_IMPLEMENTATION
#include "palettize.h"

#define PIXIE_DATA_IMPLEMENTATION
#include "pixie_data.h"
        
#define STB_IMAGE_IMPLEMENTATION
#pragma warning( push )
#pragma warning( disable: 4296 ) 
#pragma warning( disable: 4365 )
#pragma warning( disable: 4255 )
#pragma warning( disable: 4668 )
#include "stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION
#pragma warning( pop )

#define THREAD_IMPLEMENTATION
#include "thread.h"



#endif /* PIXIE_IMPLEMENTATION */

        
/*
------------------------------------------------------------------------------

This software is available under 2 licenses - you may choose the one you like.

------------------------------------------------------------------------------

ALTERNATIVE A - MIT License

Copyright (c) 2019 Mattias Gustavsson

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.

------------------------------------------------------------------------------

ALTERNATIVE B - Public Domain (www.unlicense.org)

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this 
software, either in source code form or as a compiled binary, for any purpose, 
commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of this 
software dedicate any and all copyright interest in the software to the public 
domain. We make this dedication for the benefit of the public at large and to 
the detriment of our heirs and successors. We intend this dedication to be an 
overt act of relinquishment in perpetuity of all present and future rights to 
this software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN 
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------------------------------------------
*/
