#include "pixie.h"

#ifdef __cplusplus
using namespace pixie;
#endif


ASSETS_BEGIN( "data.dat" )
ASSET_PALETTE( PAL, "pal.png" )
ASSET_SPRITE( BALL, "ball.png" )
ASSET_SONG( JAMBALA8, "jambala8.mid" )
ASSET_SONG( LARRY, "larry.mid" )
ASSETS_END()
 

int pixmain( int argc, char** argv ) {
    (void) argc, argv;
    load_assets();

    print( "Hello world!" );

    load_palette( PAL );
    for( int i = 1; i <= 8; ++i ) sprite( i, 100,100, BALL );

    play_song( JAMBALA8 );

    int c = 0;
mainloop: 
    wait_vbl();
    if( c++ == 1000 ) play_song( LARRY );
    if( c > 2000 ) end( 0 );
    for( int i = 1; i <= 8; ++i ) {
        int x = (int)( sin( ( c + 6 * i ) * 0.04f ) * cos( ( c + 6 * i ) * 0.027f ) * 150 + 160 );
        int y = (int)( sin( ( c + 6 * i ) * 0.052f ) * cos( ( c + 6 * i ) * 0.017f ) * 90 + 110 );
        sprite_pos( i, x, y );
    }
    goto mainloop;
}


#define PIXIE_IMPLEMENTATION
#include "pixie.h"

