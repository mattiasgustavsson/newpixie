#ifndef pixie_h
#define pixie_h

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

int run( int (*main)( int, char** ) );
void print( char const* str );
void wait_vbl( void );
void end( int return_code );

#endif /* pixie_h */



#ifdef PIXIE_IMPLEMENTATION
#undef PIXIE_IMPLEMENTATION


#define _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "app.h"
#include "crtemu.h"
#include "crt_frame.h"
#include "frametimer.h"
#include "thread.h"

#ifdef __cplusplus
namespace pixie {
#endif

static u64* default_font( void );

struct pixie_t {
    jmp_buf exit_jump;
    thread_atomic_int_t force_exit;

    thread_ptr_t app_thread;
    thread_atomic_int_t app_exit;
    thread_signal_t app_initialized_signal;

    thread_signal_t vbl_signal;
    thread_atomic_int_t vbl_count;

    thread_mutex_t screen_mutex;
    u32* screen_xbgr;
    int screen_width;
    int screen_height;
};


static thread_atomic_ptr_t g_tls_pixie = { NULL }; 


struct pixie_t* pixie_instance( void ) 
    { 
    struct pixie_t*  pixie = thread_tls_get( thread_atomic_ptr_load( &g_tls_pixie ) );
    int force_exit = thread_atomic_int_load( &pixie->force_exit );
    if( force_exit ) longjmp( pixie->exit_jump, force_exit );
    return pixie; 
    }


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

    thread_signal_raise( &pixie->app_initialized_signal );

    APP_U64 prev_time = app_time_count( app );       

    // Main loop
    while( !thread_atomic_int_load( &pixie->app_exit ) ) {
        frametimer_update( frametimer );

        if( app_yield( app ) == APP_STATE_EXIT_REQUESTED ) {
            thread_atomic_int_store( &pixie->force_exit, INT_MAX );
            thread_signal_raise( &pixie->vbl_signal );    
            break;
        }
    
        thread_atomic_int_add( &pixie->vbl_count, 1 );
        thread_signal_raise( &pixie->vbl_signal );    

        // Present
        APP_U64 time = app_time_count( app );
        APP_U64 delta_time_us = ( time - prev_time ) / ( app_time_freq( app ) / 1000000 );
        prev_time = time;
        crt_time_us += delta_time_us;
        thread_mutex_lock( &pixie->screen_mutex );
        crtemu_present( crtemu, crt_time_us, pixie->screen_xbgr, pixie->screen_width, pixie->screen_height, 0xffffff, 0x1c1c1c );
        thread_mutex_unlock( &pixie->screen_mutex );
        app_present( app, NULL, 1, 1, 0xffffff, 0x000000 );
    }

    frametimer_destroy( frametimer );
    crtemu_destroy( crtemu );
    free( pixie->screen_xbgr );
    return 0;
}


static int app_thread( void* user_data ) {
    return app_run( app_proc, user_data, NULL, NULL, NULL );
}


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
    thread_atomic_int_store( &pixie->app_exit, 0 );
    thread_signal_init( &pixie->app_initialized_signal );
    pixie->app_thread = thread_create( app_thread, pixie, NULL, THREAD_STACK_SIZE_DEFAULT );

    while( !thread_signal_wait( &pixie->app_initialized_signal, 1000 ) ) ;

    int result = 0;
    
    thread_atomic_int_store( &pixie->force_exit, 0 ); 
    int jumpres = setjmp( pixie->exit_jump );

    if( jumpres == 0 )
        result = main( __argc, __argv );
    else
        result = jumpres;

    thread_atomic_int_store( &pixie->app_exit, 1 );
    thread_join( pixie->app_thread );
    thread_signal_term( &pixie->app_initialized_signal );
    thread_mutex_term( &pixie->screen_mutex );
    thread_signal_term( &pixie->vbl_signal );

    free( pixie );
    return result == INT_MAX ? 0 : result;
}


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


void wait_vbl( void ) {
    struct pixie_t* pixie = pixie_instance();
    int current_vbl = thread_atomic_int_load( &pixie->vbl_count );
    while( current_vbl == thread_atomic_int_load( &pixie->vbl_count ) ) {
        thread_signal_wait( &pixie->vbl_signal, 1000 );    
        pixie = pixie_instance();
    }
}


void end( int return_code ) {
    struct pixie_t* pixie = pixie_instance();
    thread_atomic_int_store( &pixie->force_exit, return_code ? return_code : INT_MAX );
}


static u64* default_font( void ) {
    static u64 data[ 256 ] = {
        0x0000000000000000,0x7e8199bd81a5817e,0x7effe7c3ffdbff7e,0x00081c3e7f7f7f36,0x00081c3e7f3e1c08,0x1c086b7f7f1c3e1c,
        0x1c083e7f3e1c0808,0x0000183c3c180000,0xffffe7c3c3e7ffff,0x003c664242663c00,0xffc399bdbd99c3ff,0x1e333333bef0e0f0,
        0x187e183c6666663c,0x070f0e0c0cfcccfc,0x0367e6c6c6fec6fe,0x18db3ce7e73cdb18,0x0001071f7f1f0701,0x0040707c7f7c7040,
        0x183c7e18187e3c18,0x0066006666666666,0x00d8d8d8dedbdbfe,0x1e331c36361cc67c,0x007e7e7e00000000,0xff183c7e187e3c18,
        0x00181818187e3c18,0x00183c7e18181818,0x000018307f301800,0x00000c067f060c00,0x00007f0303030000,0x00002466ff662400,
        0x0000ffff7e3c1800,0x0000183c7effff00,0x0000000000000000,0x000c000c0c1e1e0c,0x0000000000363636,0x0036367f367f3636,
        0x000c1f301e033e0c,0x0063660c18336300,0x006e333b6e1c361c,0x0000000000030606,0x00180c0606060c18,0x00060c1818180c06,
        0x0000663cff3c6600,0x00000c0c3f0c0c00,0x060c0c0000000000,0x000000003f000000,0x000c0c0000000000,0x000103060c183060,
        0x003e676f7b73633e,0x003f0c0c0c0c0e0c,0x003f33061c30331e,0x001e33301c30331e,0x0078307f33363c38,0x001e3330301f033f,
        0x001e33331f03061c,0x000c0c0c1830333f,0x001e33331e33331e,0x000e18303e33331e,0x000c0c00000c0c00,0x060c0c00000c0c00,
        0x00180c0603060c18,0x00003f00003f0000,0x00060c1830180c06,0x000c000c1830331e,0x001e037b7b7b633e,0x0033333f33331e0c,
        0x003f66663e66663f,0x003c66030303663c,0x001f36666666361f,0x007f46161e16467f,0x000f06161e16467f,0x007c66730303663c,
        0x003333333f333333,0x001e0c0c0c0c0c1e,0x001e333330303078,0x006766361e366667,0x007f66460606060f,0x0063636b7f7f7763,
        0x006363737b6f6763,0x001c36636363361c,0x000f06063e66663f,0x00381e3b3333331e,0x006766363e66663f,0x001e33180c06331e,
        0x001e0c0c0c0c2d3f,0x003f333333333333,0x000c1e3333333333,0x0063777f6b636363,0x0063361c1c366363,0x001e0c0c1e333333,
        0x007f664c1831637f,0x001e06060606061e,0x00406030180c0603,0x001e18181818181e,0x0000000063361c08,0xff00000000000000,
        0x0000000000180c0c,0x006e333e301e0000,0x003b66663e060607,0x001e3303331e0000,0x006e33333e303038,0x001e033f331e0000,
        0x000f06060f06361c,0x1f303e33336e0000,0x006766666e360607,0x001e0c0c0c0e000c,0x1e33333030300030,0x0067361e36660607,
        0x001e0c0c0c0c0c0e,0x00636b7f7f330000,0x00333333331f0000,0x001e3333331e0000,0x0f063e66663b0000,0x78303e33336e0000,
        0x000f06666e3b0000,0x001f301e033e0000,0x00182c0c0c3e0c08,0x006e333333330000,0x000c1e3333330000,0x00367f7f6b630000,
        0x0063361c36630000,0x1f303e3333330000,0x003f260c193f0000,0x00380c0c070c0c38,0x0018181800181818,0x00070c0c380c0c07,
        0x0000000000003b6e,0x007f6363361c0800,0x1e30181e3303331e,0x007e333333003300,0x001e033f331e0038,0x00fc667c603cc37e,
        0x007e333e301e0033,0x007e333e301e0007,0x007e333e301e0c0c,0x1c301e03031e0000,0x003c067e663cc37e,0x001e033f331e0033,
        0x001e033f331e0007,0x001e0c0c0c0e0033,0x003c1818181c633e,0x001e0c0c0c0e0007,0x0063637f63361c63,0x00333f331e000c0c,
        0x003f061e063f0038,0x00fe33fe30fe0000,0x007333337f33367c,0x001e33331e00331e,0x001e33331e003300,0x001e33331e000700,
        0x007e33333300331e,0x007e333333000700,0x1f303e3333003300,0x00183c66663c18c3,0x001e333333330033,0x18187e03037e1818,
        0x003f67060f26361c,0x0c0c3f0c3f1e3333,0xe363f3635f33331f,0x0e1b18183c18d870,0x007e333e301e0038,0x001e0c0c0c0e001c,
        0x001e33331e003800,0x007e333333003800,0x003333331f001f00,0x00333b3f3733003f,0x00007e007c36363c,0x00003e001c36361c,
        0x001e3303060c000c,0x000003033f000000,0x000030303f000000,0xf03366cc7b3363c3,0xc0f3f6ecdb3363c3,0x0018181818001818,
        0x0000cc663366cc00,0x00003366cc663300,0x1144114411441144,0x55aa55aa55aa55aa,0x77dbeedb77dbeedb,0x1818181818181818,
        0x1818181f18181818,0x1818181f181f1818,0x6c6c6c6f6c6c6c6c,0x6c6c6c7f00000000,0x1818181f181f0000,0x6c6c6c6f606f6c6c,
        0x6c6c6c6c6c6c6c6c,0x6c6c6c6f607f0000,0x0000007f606f6c6c,0x0000007f6c6c6c6c,0x0000001f181f1818,0x1818181f00000000,
        0x000000f818181818,0x000000ff18181818,0x181818ff00000000,0x181818f818181818,0x000000ff00000000,0x181818ff18181818,
        0x181818f818f81818,0x6c6c6cec6c6c6c6c,0x000000fc0cec6c6c,0x6c6c6cec0cfc0000,0x000000ff00ef6c6c,0x6c6c6cef00ff0000,
        0x6c6c6cec0cec6c6c,0x000000ff00ff0000,0x6c6c6cef00ef6c6c,0x000000ff00ff1818,0x000000ff6c6c6c6c,0x181818ff00ff0000,
        0x6c6c6cff00000000,0x000000fc6c6c6c6c,0x000000f818f81818,0x181818f818f80000,0x6c6c6cfc00000000,0x6c6c6cff6c6c6c6c,
        0x181818ff18ff1818,0x0000001f18181818,0x181818f800000000,0xffffffffffffffff,0xffffffff00000000,0x0f0f0f0f0f0f0f0f,
        0xf0f0f0f0f0f0f0f0,0x00000000ffffffff,0x006e3b133b6e0000,0x03031f331f331e00,0x0003030303333f00,0x0036363636367f00,
        0x003f33060c06333f,0x000e1b1b1b7e0000,0x03063e6666666600,0x00181818183b6e00,0x3f0c1e33331e0c3f,0x001c36637f63361c,
        0x007736366363361c,0x001e33333e180c38,0x00007edbdb7e0000,0x03067edbdb7e3060,0x001c06031f03061c,0x003333333333331e,
        0x00003f003f003f00,0x003f000c0c3f0c0c,0x003f00060c180c06,0x003f00180c060c18,0x1818181818d8d870,0x0e1b1b1818181818,
        0x000c0c003f000c0c,0x00003b6e003b6e00,0x000000001c36361c,0x0000001818000000,0x0000001800000000,0x383c3637303030f0,
        0x000000363636361e,0x0000001e060c180e,0x00003c3c3c3c0000,0x0000000000000000,
    };

    return data;
}

#ifdef __cplusplus
} /* namespace pixie */
#endif


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
        struct HINSTANCE__;
        // pass-through so the program will build with either /SUBSYSTEM:WINDOWS or /SUBSYSTEN:CONSOLE
        /*extern "C"*/ int __stdcall WinMain( struct HINSTANCE__* a, struct HINSTANCE__* b, char* c, int d ) { (void) a, b, c, d; return main( __argc, __argv ); }
    #endif /* _WIN32 */

#endif /* PIXIE_NO_MAIN */


#define APP_IMPLEMENTATION
#ifdef _WIN32
    #define APP_WINDOWS
#endif
#include "app.h"

#define CRTEMU_IMPLEMENTATION
#pragma warning( push )
#pragma warning( disable: 4204 )
#include "crtemu.h"
#pragma warning( pop )

#define CRT_FRAME_IMPLEMENTATION
#include "crt_frame.h"

    #define FRAMETIMER_IMPLEMENTATION
#include "frametimer.h"

#define THREAD_IMPLEMENTATION
#include "thread.h"



#endif /* PIXIE_IMPLEMENTATION */