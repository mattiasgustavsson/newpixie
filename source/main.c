#include "pixie/pixie.h"

int pixmain( int argc, char** argv ) {
    print( "Hello world!" );
    if( argc > 0 ) print( argv[ 0 ] );

    int c = 0;
mainloop: 
    wait_vbl();
    if( c++ > 100 ) end( 0 );
    goto mainloop;
}


#define PIXIE_IMPLEMENTATION
#include "pixie/pixie.h"
