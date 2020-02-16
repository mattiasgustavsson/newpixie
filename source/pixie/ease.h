/*
------------------------------------------------------------------------------
		  Licensing information can be found at the end of the file.
------------------------------------------------------------------------------

ease.h - v0.1 - ease in/out interpolation functions for C/C++.

Do this:
	#define EASE_IMPLEMENTATION
before you include this file in *one* C/C++ file to create the implementation.

------------------------------------------------------------------------------

Based on http://www.robertpenner.com/easing/penner_chapter7_tweening.pdf
Note that in this code, input and output is in normalized 0.0 to 1.0 range.

*/

#ifndef ease_h
#define ease_h

typedef float (*ease_func_t)( float );

float ease_linear( float t );
float ease_smoothstep( float t );
float ease_smootherstep( float t );

float ease_out_quad( float t );
float ease_out_back( float t );
float ease_out_bounce( float t );
float ease_out_sine( float t );
float ease_out_elastic( float t );
float ease_out_expo( float t );
float ease_out_cubic( float t );
float ease_out_quart( float t );
float ease_out_quint( float t );
float ease_out_circle( float t );

float ease_in_quad( float t );
float ease_in_back( float t );
float ease_in_bounce( float t );
float ease_in_sine( float t );
float ease_in_elastic( float t );
float ease_in_expo( float t );
float ease_in_cubic( float t );
float ease_in_quart( float t );
float ease_in_quint( float t );
float ease_in_circle( float t );

float ease_in_out_quad( float t );
float ease_in_out_back( float t );
float ease_in_out_bounce( float t );
float ease_in_out_sine( float t );
float ease_in_out_elastic( float t );
float ease_in_out_expo( float t );
float ease_in_out_cubic( float t );
float ease_in_out_quart( float t );
float ease_in_out_quint( float t );
float ease_in_out_circle( float t );

#endif /* ease_h */


/*
----------------------
	IMPLEMENTATION
----------------------
*/

#ifdef EASE_IMPLEMENTATION
#undef EASE_IMPLEMENTATION

#if !defined( EASE_ACOS ) || !defined( EASE_POW ) || !defined( EASE_SIN ) || !defined( EASE_SQRT )
	#define _CRT_NONSTDC_NO_DEPRECATE 
	#define _CRT_SECURE_NO_WARNINGS
	#pragma warning( push )
	#pragma warning( disable: 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
	#include <math.h>
	#pragma warning( pop )
#endif

#ifndef EASE_ACOS
    #ifdef __TINYC__
	    #define EASE_ACOS( x ) ( (float) acos( (double) x ) )
    #else
	    #define EASE_ACOS( x ) acosf( x )
    #endif
#endif

#ifndef EASE_COS
    #ifdef __TINYC__
	    #define EASE_COS( x ) ( (float) cos( (double) x ) )
    #else
	    #define EASE_COS( x ) cosf( x )
    #endif
#endif

#ifndef EASE_POW
    #ifdef __TINYC__
	    #define EASE_POW( x, y ) ( (float) pow( (double) x, (double) y ) )
    #else
	    #define EASE_POW( x, y ) powf( x, y )
    #endif
#endif

#ifndef EASE_SIN
    #ifdef __TINYC__
	    #define EASE_SIN( x ) ( (float) sin( (double) x ) )
    #else
	    #define EASE_SIN( x ) sinf( x )
    #endif
#endif

#ifndef EASE_SQRT
    #ifdef __TINYC__
	    #define EASE_SQRT( x ) ( (float) sqrt( (double) x ) )
    #else
	    #define EASE_SQRT( x ) sqrtf( x )
    #endif
#endif


float ease_linear( float t ) { 
	return t; 
}


float ease_smoothstep( float t ) { 
	return t * t * ( 3.0f - 2.0f * t ); 
}


float ease_smootherstep( float t )  { 
	return t * t * t * ( t * ( t * 6.0f - 15.0f ) + 10.0f ); 
}


float ease_out_quad( float t ) { 
	return -t * ( t - 2.0f ); 
}
	
	
float ease_in_quad( float t ) { 
	return t * t; 
}

	
float ease_in_out_quad( float t ) { 
	return ( ( t *= 2.0f ) < 1.0f ) 
		? ( 0.5f * t * t ) 
		: ( -0.5f * ( ( t - 1.0f ) * ( t - 3.0f ) - 1.0f ) ); 
}


float ease_out_back( float t ) { 
    t -= 1.0f;
	return ( t * t * ( ( 1.70158f + 1.0f ) * t + 1.70158f ) + 1.0f ); 
}
	
	
float ease_in_back( float t ) { 
	return t * t * ( ( 1.70158f + 1.0f ) * t - 1.70158f ); 
}
	
	
float ease_in_out_back( float t ) { 
	float s = 1.70158f * 1.525f; 
    t *= 2.0f;
	
	if( t < 1.0f ) {
		return 0.5f * ( t * t * ( ( s + 1.0f ) * t - s ) );
    } else {
        t -= 2.0f;
        return 0.5f * ( t * t * ( ( s + 1.0f ) * t + s ) + 2.0f ); 
    }
}
	

float ease_out_bounce( float t ) { 
	if( t < ( 1.0f / 2.75f ) ) {
		return 7.5625f * t * t; 
    } else if( t < ( 2.0f / 2.75f ) ) {
        t -= ( 1.50f / 2.75f );
		return 7.5625f * t * t + 0.75f; 
    } else if( t < ( 2.5f / 2.75f ) ) {
        t -= ( 2.25f / 2.75f );
		return 7.5625f * t * t + 0.9375f; 
    } else {
        t -= ( 2.625f / 2.75f );
		return 7.5625f * t * t + 0.984375f; 
    }
}
	
	
float ease_in_bounce( float t )  { 
	return 1.0f - ease_out_bounce( 1.0f - t ); 
}
	
	
float ease_in_out_bounce( float t ) { 
	return ( t < 0.5f ) 
		? ( ease_in_bounce( t * 2.0f ) * 0.5f ) 
		: ( ease_out_bounce( t * 2.0f - 1.0f ) * 0.5f + 0.5f ); 
}


float ease_out_sine( float t ) { 
	return EASE_SIN( t * EASE_ACOS( 0.0f ) ); 
}
	
	
float ease_in_sine( float t ) { 
	return 1.0f - EASE_COS( t * EASE_ACOS( 0.0f ) ); 
}


float ease_in_out_sine( float t ) { 
	return -( EASE_COS( EASE_ACOS( -1.0f ) * t ) - 1.0f ) / 2.0f; 
}


float ease_out_elastic( float t ) { 
	if( t == 0.0f ) {
        return 0.0f; 	
    }
	if( t == 1.0f ) {
        return 1.0f; 
    }
	
	float p = 0.4f; 
	float s = p / 4.0f; 
	return ( EASE_POW( 2.0f, -10.0f * t ) * EASE_SIN( ( t - s ) * ( 2.0f * EASE_ACOS( -1.0f ) ) / p ) + 1.0f ); 
}


float ease_in_elastic( float t )  { 
	if( t == 0.0f ) { 
        return 0.0f; 
    }
	if( t == 1.0f ) {
        return 1.0f; 
    }
		
	float p = 0.4f; 
	float s = p / 4.0f; 
    t -= 1.0f;
	return -( EASE_POW( 2.0f, 10.0f * t ) * EASE_SIN( ( t - s ) * ( 2.0f * EASE_ACOS( -1.0f ) ) / p ) ); 
}
	
	
float ease_in_out_elastic( float t ) { 
	if( t == 0.0f) {
        return 0.0f; 
    }
    t *= 2.0f;
	if( t == 2.0f ) {
        return 1.0f; 
    }
	
	float p = 0.3f * 1.5f; 
	float s = p / 4.0f; 
	if( t < 1.0f ) {
        t -= 1.0f;
		return -0.5f * ( EASE_POW( 2.0f, 10.0f * t ) * EASE_SIN( ( t - s ) * ( 2.0f * EASE_ACOS( -1.0f ) ) / p ) ); 
    } else {
        t -= 1.0f;
		return EASE_POW( 2.0f, -10.0f * t ) * EASE_SIN( ( t - s ) * ( 2.0f * EASE_ACOS( -1.0f ) ) / p) * 0.5f + 1.0f; 
    }
}


float ease_out_expo( float t ) { 
	return t == 1.0f 
		? 1.0f 
		: ( 1.0f - EASE_POW( 2.0f, -10.0f * t ) ); 
}
	
	
float ease_in_expo( float t ) { 
	return t == 0.0f 
		? 0.0f 
		: EASE_POW( 2.0f, 10.0f * ( t - 1.0f ) ); 
}
	
	
float ease_in_out_expo( float t ) { 
	if( t == 0.0f ) {
        return 0.0f;
    }
	if( t == 1.0f ) {
        return 1.0f; 
    }
    t *= 2.0f;
	if( t < 1.0f ) {
		return 0.5f * EASE_POW( 2.0f, 10.0f * ( t - 1.0f ) ); 
    } else {
        return 0.5f * ( 2.0f - EASE_POW( 2.0f, -10.0f * --t ) ); 
	}
}


float ease_out_cubic( float t ) { 
    t -= 1.0f;
	return t * t * t + 1.0f; 
}
	
	
float ease_in_cubic( float t ) { 
	return t * t * t; 
}
	
	
float ease_in_out_cubic( float t ) { 
    t *= 2.0f;
	if( t < 1.0f ) {
        return 0.5f * t * t * t;
    } else {
        t -= 2.0f;
		return 0.5f * ( t * t * t + 2.0f ); 
    }
}


float ease_out_quart( float t ) { 
    t -= 1.0f;
	return -( t * t * t * t - 1.0f ); 
}
	
	
float ease_in_quart( float t ) { 
	return t * t * t * t; 
}


float ease_in_out_quart( float t ) { 
    t *= 2.0f;
	if( t < 1.0f ) {
		return 0.5f * t * t * t * t;
    } else {
        t -= 2.0f;
		return -0.5f * ( t * t * t * t - 2.0f ); 
    }
}
	

float ease_out_quint( float t ) { 
    t -= 1.0f;
	return t * t * t * t * t + 1.0f; 
}
	
	
float ease_in_quint( float t ) { 
	return t * t * t * t * t; 
}
	
	
float ease_in_out_quint( float t ) { 
    t *= 2.0f;
	if( t < 1.0f ) {
		return 0.5f * t * t * t * t * t;
    } else {
        t -= 2.0f;
		return 0.5f * ( t * t * t * t * t + 2.0f ); 
    }
}


float ease_out_circle( float t ) { 
    t -= 1.0f;
	return EASE_SQRT( 1.0f - t * t ); 
}
	
	
float ease_in_circle( float t ) { 
	return 1.0f - EASE_SQRT( 1.0f - t * t ); 
}
	

float ease_in_out_circle( float t ) { 
    t *= 2.0f;
	if( t < 1.0f ) {
		return 0.5f * (1.0f - EASE_SQRT( 1.0f - t * t ) );
    } else {
        t -= 2.0f;
		return 0.5f * ( EASE_SQRT( 1.0f - t * t ) + 1.0f ); 
    }
}


#endif /* EASE_IMPLEMENTATION */

/*
------------------------------------------------------------------------------

This software is available under 2 licenses - you may choose the one you like.

------------------------------------------------------------------------------

ALTERNATIVE A - MIT License

Copyright (c) 2015 Mattias Gustavsson

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
