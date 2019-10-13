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
void wait_vbl( void );
void end( int return_code );

void print( char const* str );



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
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

// Libaries includes
#include "app.h"
#include "crtemu.h"
#include "crt_frame.h"
#include "frametimer.h"
#include "pixie_data.h"
#include "thread.h"


#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
namespace pixie {
#endif

/*
----------------
    INTERNALS
----------------
*/


// Main engine state - *everything* is stored here, and data is accessed from both the app thread and the user thread
// The instance is created within the `run` function, and a pointer to it is stored in thread local storage for the
// user thread, so that every API method can access it to perform its function. The app thread gets a pointer to it 
// through the user_data parameter to the app_proc.

struct pixie_t {
    jmp_buf exit_jump;
    thread_atomic_int_t force_exit;

    thread_ptr_t app_thread;
    thread_atomic_int_t app_exit;
    thread_signal_t app_initialization_complete;

    thread_signal_t vbl_signal;
    thread_atomic_int_t vbl_count;

    thread_mutex_t screen_mutex;
    u32* screen_xbgr;
    int screen_width;
    int screen_height;
};


// A global atomic pointer to the TLS instance for storing per-thread `pixie_t` pointers. Created in the `run` function
// unless it has already been created (through a compare-and-swap). The `run` method then sets the TLS value on the 
// instance, for the current thread.

static thread_atomic_ptr_t g_tls_pixie = { NULL }; 


// Retrieves the pointer to the `pixie_t` state for the current thread, as stored in the `g_tls_pixie` TLS storage
// As this has to be called in every API function, it was also a convenient place to check if the app thread is 
// requesting a shutdown of the program, and in that case do a forced exit (using `longjmp`) no matter how deep down a
// call stack `pixie_instance` was called from.

struct pixie_t* pixie_instance( void ) { 
    // Get the `pixie_t` pointer for this thread from the global TLS instance `g_tls_pixie`
    struct pixie_t*  pixie = (struct pixie_t*) thread_tls_get( thread_atomic_ptr_load( &g_tls_pixie ) );

    // Check if app thread is requesting a forced exit (the user closed the window) and if so, call the exit point
    int force_exit = thread_atomic_int_load( &pixie->force_exit );
    if( force_exit ) longjmp( pixie->exit_jump, force_exit );

    return pixie; 
}


// Main body for the app thread (invoked by calling `app_run` in the `app_thread` thread entry point).
// The app thread runs independently from the user thread, and handles the main window, rendering, audio and input.
// After all initialization, and just before entering the main loop, it will raise the `app_initialization_complete`
// signal, which lets the user thread (in the `run` function) know that it is safe to call the users entrypoint.
// Every iteration through the main loop, the signal `vbl_signal` is raised, and the `vbl_count` value is incremented.
// These are used by the `wait_vbl` function to pause the user thread until the start of the next frame.
// If the main window is closed (by clicking on the close button), the app thread sets the `force_exit` value to
// `INT_MAX`, to signal to the user thread that it should exit the user code and terminate. The `force_exit` value is
// checked in the `pixie_instance` function, which is called at the start of every API call.

static int app_proc( app_t* app, void* user_data ) {
    struct pixie_t* pixie = (struct pixie_t*) user_data;

    pixie->screen_width = 384;
    pixie->screen_height = 288;
    pixie->screen_xbgr = (u32*) malloc( sizeof( u32 ) * pixie->screen_width * pixie->screen_height );
    memset( pixie->screen_xbgr, 0, sizeof( u32 ) * pixie->screen_width * pixie->screen_height );

    app_screenmode( app, APP_SCREENMODE_WINDOW );
    app_interpolation( app, APP_INTERPOLATION_NONE );

    crtemu_t* crtemu = crtemu_create( NULL );
    CRTEMU_U64 crt_time_us = 0;

    CRT_FRAME_U32* frame = (CRT_FRAME_U32*) malloc( sizeof( CRT_FRAME_U32 ) * CRT_FRAME_WIDTH * CRT_FRAME_HEIGHT );
    crt_frame( frame );
    crtemu_frame( crtemu, frame, CRT_FRAME_WIDTH, CRT_FRAME_HEIGHT );
    free( frame );

    frametimer_t* frametimer = frametimer_create( NULL );
    frametimer_lock_rate( frametimer, 60 );

    APP_U64 prev_time = app_time_count( app );       

    thread_signal_raise( &pixie->app_initialization_complete );

    while( !thread_atomic_int_load( &pixie->app_exit ) ) {
        frametimer_update( frametimer );

        if( app_yield( app ) == APP_STATE_EXIT_REQUESTED ) {
            thread_atomic_int_store( &pixie->force_exit, INT_MAX );
            thread_atomic_int_add( &pixie->vbl_count, 1 );
            thread_signal_raise( &pixie->vbl_signal );    
            break;
        }
    
        // Present
        APP_U64 time = app_time_count( app );
        APP_U64 delta_time_us = ( time - prev_time ) / ( app_time_freq( app ) / 1000000 );
        prev_time = time;
        crt_time_us += delta_time_us;
        thread_mutex_lock( &pixie->screen_mutex );
        crtemu_present( crtemu, crt_time_us, pixie->screen_xbgr, pixie->screen_width, pixie->screen_height, 0xffffff, 0x1c1c1c );
        thread_mutex_unlock( &pixie->screen_mutex );
        app_present( app, NULL, 1, 1, 0xffffff, 0x000000 );

        thread_atomic_int_add( &pixie->vbl_count, 1 );
        thread_signal_raise( &pixie->vbl_signal );    
    }

    frametimer_destroy( frametimer );
    crtemu_destroy( crtemu );
    free( pixie->screen_xbgr );
    return 0;
}


// Entry point for the app thread, which is started from the `run` function. Just runs the app with the above `app_proc` 

static int app_thread( void* user_data ) {
    return app_run( app_proc, user_data, NULL, NULL, NULL );
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
    thread_tls_t pixie_tls = thread_tls_create();
    if( thread_atomic_ptr_compare_and_swap( &g_tls_pixie, NULL, pixie_tls ) )
        thread_tls_destroy( pixie_tls );

    struct pixie_t* pixie = (struct pixie_t*) malloc( sizeof( struct pixie_t ) );
    memset( pixie, 0, sizeof( *pixie ) );
    thread_tls_set( thread_atomic_ptr_load( &g_tls_pixie ), pixie );

    thread_signal_init( &pixie->vbl_signal );
    thread_atomic_int_store( &pixie->vbl_count,  0 );

    thread_mutex_init( &pixie->screen_mutex );

    thread_atomic_int_store( &pixie->force_exit, 0 ); 
    thread_atomic_int_store( &pixie->app_exit, 0 );
    thread_signal_init( &pixie->app_initialization_complete );
    pixie->app_thread = thread_create( app_thread, pixie, NULL, THREAD_STACK_SIZE_DEFAULT );

    int result = thread_signal_wait( &pixie->app_initialization_complete, 3000 ) ? EXIT_SUCCESS : EXIT_FAILURE;
    if( result == EXIT_SUCCESS ) {
        #pragma warning( push )
        #pragma warning( disable: 4611 ) // interaction between '_setjmp' and C++ object destruction is non-portable
        int jumpres = setjmp( pixie->exit_jump );
        #pragma warning( pop )

        if( jumpres == 0 )
            result = main( __argc, __argv );
        else
            result = jumpres;
    }

    thread_atomic_int_store( &pixie->app_exit, 1 );
    thread_join( pixie->app_thread );
    thread_signal_term( &pixie->app_initialization_complete );
    thread_mutex_term( &pixie->screen_mutex );
    thread_signal_term( &pixie->vbl_signal );

    free( pixie );
    return ( result == INT_MAX ? EXIT_SUCCESS : result );
}


// Terminates the user code, performing a controlled shutdown of the engine. Can be called from deep down a call stack
// as it will use `longjmp` to branch back to the top level of the `run` function.

void end( int return_code ) {
    struct pixie_t* pixie = pixie_instance();
    longjmp( pixie->exit_jump, return_code );
}


// Waits until the start of the next frame. 

void wait_vbl( void ) {
    struct pixie_t* pixie = pixie_instance();
    int current_vbl = thread_atomic_int_load( &pixie->vbl_count );
    while( current_vbl == thread_atomic_int_load( &pixie->vbl_count ) ) {
        thread_signal_wait( &pixie->vbl_signal, 1000 );    
        pixie = pixie_instance();
    }
}


// Prints the specified string to the screen using the default font.

void print( char const* str ) {
    struct pixie_t* pixie = pixie_instance();
    thread_mutex_lock( &pixie->screen_mutex );
    static int x = 32;
    static int y = 44;
    while( *str ) {
        unsigned long long chr = default_font()[ (u8) *str++ ];
        for( int iy = 0; iy < 8; ++iy )
            for( int ix = 0; ix < 8; ++ix )
                if( chr & ( 1ull << ( ix + iy * 8 ) ) )
                    pixie->screen_xbgr[ x + ix + ( y + iy ) * pixie->screen_width ] = 0xffffffff; 
        x += 8;
        if( x - 32 >= 320 ) {
            x = 32;
            y += 8;
        }
    }
    x = 32;
    y += 8;
    thread_mutex_unlock( &pixie->screen_mutex );
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

#define PIXIE_DATA_IMPLEMENTATION
#include "pixie_data.h"
        
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
