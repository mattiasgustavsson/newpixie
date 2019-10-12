#define _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>

#ifndef NDEBUG
    #pragma warning( push ) 
    #pragma warning( disable: 4619 ) // pragma warning : there is no warning number 'number'
    #pragma warning( disable: 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
    #include <crtdbg.h>
    #pragma warning( pop ) 
#endif


int main( int argc, char** argv )
    {
    (void) argc, argv;
    #ifndef NDEBUG
        int flag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ); // Get current flag
        flag ^= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
        _CrtSetDbgFlag( flag ); // Set flag to the new value
//        _CrtSetBreakAlloc( 0 );
    #endif


    return 0;
    }

// pass-through so the program will build with either /SUBSYSTEM:WINDOWS or /SUBSYSTEN:CONSOLE
extern "C" int __stdcall WinMain( struct HINSTANCE__*, struct HINSTANCE__*, char*, int ) { return main( __argc, __argv ); }

