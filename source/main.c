#include "pixie/pixie.h"

#define _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>

#ifdef __cplusplus
using namespace pixie;
#endif

int pixmain( int argc, char** argv ) {
    (void) argc, argv;
    print( "Hello world!" );

    load_palette( "pal.png" );
    load_sprite( 1, "ball.png" );
    for( int i = 1; i <= 8; ++i ) sprite( i, 100,100, 1 );
    
    int c = 0;
mainloop: 
    wait_vbl();
    if( c++ > 1000 ) end( 0 );
    for( int i = 1; i <= 8; ++i ) {
        int x = (int)( sinf( ( c + 6 * i ) * 0.04f ) * cosf( ( c + 6 * i ) * 0.027f ) * 150 + 160 );
        int y = (int)( sinf( ( c + 6 * i ) * 0.052f ) * cosf( ( c + 6 * i ) * 0.017f ) * 90 + 110 );
        sprite_pos( i, x, y );
    }
    goto mainloop;
}


#define PIXIE_IMPLEMENTATION
#include "pixie/pixie.h"
