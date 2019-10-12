#ifndef pixie_h
#define pixie_h

int run( int (*main)( int argc, char** argv ) );
void print( char const* str );
void wait_vbl( void );
void end( int return_code );

#endif /* pixie_h */



#ifdef PIXIE_IMPLEMENTATION
#undef PIXIE_IMPLEMENTATION


#define _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>


int run( int (*main)( int argc, char** argv ) ) {
    return main( __argc, __argv );
}


void print( char const* str ) {
    (void) str;
}


void wait_vbl( void ) {
}


void end( int return_code ) {
    exit( return_code );    
}


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

    int main( int argc, char** argv )
        {
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


#endif /* PIXIE_IMPLEMENTATION */