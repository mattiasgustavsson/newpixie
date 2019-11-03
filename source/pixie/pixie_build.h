/*
------------------------------------------------------------------------------
          Licensing information can be found at the end of the file.
------------------------------------------------------------------------------

pixie_build.h - v0.1 - Asset build and bundle tools for pixie game dev lib.

Do this:
    #define PIXIE_BUILD_IMPLEMENTATION
before you include this file in *one* C/C++ file to create the implementation.
*/


#ifndef pixie_build_h
#define pixie_build_h


#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
    #define ASSETS_BEGIN( bundle_filename_param ) \
        namespace pixie { \
            inline void multiple_pixie_ASSETS_BEGIN_declarations_not_allowed( char const** bundle_filename, int* line ) \
                { *bundle_filename = bundle_filename_param; *line = __LINE__; } \
            enum assets_t {

    #define ASSETS_END() TEMP_PIXIE_ASSETS_COUNT }; \
            int build_and_load_assets( char const* bundle_filename, char const* build_time, \
                char const* definitions_file, int definitions_line, int assets_count ); \
            inline int load_assets( void ) { \
                char const* bundle_filename; \
                int definitions_line; \
                multiple_pixie_ASSETS_BEGIN_declarations_not_allowed( &bundle_filename, &definitions_line ); \
                return build_and_load_assets( bundle_filename, __DATE__ " " __TIME__, __FILE__, definitions_line, \
                    TEMP_PIXIE_ASSETS_COUNT ); \
            } \
        } /* namespace pixie */
#else
    #define ASSETS_BEGIN( bundle_filename_param ) \
        inline void multiple_pixie_ASSETS_BEGIN_declarations_not_allowed( char const** bundle_filename, int* line ) \
            { *bundle_filename = bundle_filename_param; *line = __LINE__; } \
        enum assets_t {

    #define ASSETS_END() TEMP_PIXIE_ASSETS_COUNT }; \
        int build_and_load_assets( char const* bundle_filename, char const* build_time, \
            char const* definitions_file, int definitions_line, int assets_count ); \
        inline int load_assets( void ) { \
            char const* bundle_filename; \
            int definitions_line; \
            multiple_pixie_ASSETS_BEGIN_declarations_not_allowed( &bundle_filename, &definitions_line ); \
            return build_and_load_assets( bundle_filename, __DATE__ " " __TIME__, __FILE__, definitions_line, \
                TEMP_PIXIE_ASSETS_COUNT ); \
        } 
#endif

#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
namespace pixie {
#endif

typedef void* (*asset_build_function_t)( char const* filenames[], int count, int* out_size );
void register_asset_type( char const* name, asset_build_function_t build_function );

void* load_binary_file( char const* filename, int* size );
void free_binary_file( void* data );

char* load_text_file( char const* filename, int* size );
void free_text_file( char* text );

void* build_palette( const char* filenames[], int count, int* out_size );
void* build_sprite( const char* filenames[], int count, int* out_size );
void* build_song( char const* filenames[], int count, int* out_size );
void* build_text( char const* filenames[], int count, int* out_size );
void* build_binary( char const* filenames[], int count, int* out_size );
void* build_font( char const* filenames[], int count, int* out_size );

#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
} /* namespace pixie */
#endif

#endif /* pixie_build_h */


/*
----------------------
    IMPLEMENTATION
----------------------
*/

#ifdef PIXIE_BUILD_IMPLEMENTATION
#undef PIXIE_BUILD_IMPLEMENTATION

#ifndef _CRT_NONSTDC_NO_DEPRECATE 
    #define _CRT_NONSTDC_NO_DEPRECATE 
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
#endif

// C runtime includes
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Libraries includes
#include "dir.h"
#include "file_util.h"
#include "palettize.h"
#include "paldither.h"
#include "palrle.h"
#include "pixie_data.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"


#ifndef PIXIE_BUILD_U8
    #define PIXIE_BUILD_U8 unsigned char
#endif

#ifndef PIXIE_BUILD_U32
    #define PIXIE_BUILD_U32 unsigned int
#endif


#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
namespace pixie {
#endif

// In C, a void* can be implicitly cast to any other kind of pointer, while in C++ you need an explicit cast. In most
// cases, the explicit cast works for both C and C++, but if we consider the case where we have nested structs, then
// the way you refer to them differs between C and C++ (in C++, `parent_type::nested_type`, in C just `nested_type`).
// In addition, with the automatic cast in C, it is possible to use unnamed nested structs and still dynamically 
// allocate arrays of that type - this would be desirable when the code is compiled from C++ as well.
// This VOID_CAST macro allows for automatic cast from void* in C++. In C, it does nothing, but for C++ it uses a 
// simple template function to define a cast-to-anything operator.
// Use like this:
//      struct {
//          struct {
//              int x;
//          } *nested;
//      } parent;
//      parent.nested = VOID_CAST( malloc( sizeof( *parent.nested ) * count ) );
//

#ifndef VOID_CAST
    #ifdef __cplusplus
        struct void_cast {   
            inline void_cast( void* x_ ) : x( x_ ) { }
            inline void_cast( void const* x_ ) : x( (void*) x_ ) { }
            template< typename T > inline operator T() { return (T)x; } // cast to whatever requested
            void* x;
        };
        #define VOID_CAST( x ) void_cast( x )
    #else
        #define VOID_CAST( x ) x
    #endif
#endif



struct item_t {
    int id;
    char filename[ 256 ];
    char type[ 64 ];
};

static struct item_t* read_asset_definitions( char const* asset_definitions_file, int* count, char out_bundle_filename[ 256 ] ) { 
    int size = 0;
    char* file = load_text_file( asset_definitions_file, &size );
    if( !file ) {
        printf( "Asset definition file '%s' could not be opened\n", asset_definitions_file );
        return NULL;;
    }

    char const* start = strstr( file, "ASSETS_BEGIN" );
    if( start == NULL ) {
        printf( "Asset definition file '%s' does not contain an 'ASSETS_BEGIN' definition\n", asset_definitions_file );
        free( file );
        return NULL;;
    }
    char const* end = strstr( start, "ASSETS_END" );
    if( end == NULL ) {
        printf( "Asset definition file '%s' does not contain an 'ASSETS_END' definition\n", asset_definitions_file );
        free( file );
        return NULL;;
    }
    char const* ptr = start + strlen( "ASSETS_BEGIN" );
    while( ptr < end && *ptr <= ' ' ) ++ptr;
    if( *ptr != '(' ) {
        printf( "Asset definition file '%s': expected '(' after ASSETS_BEGIN\n", asset_definitions_file );
        free( file );
        return NULL;;
    }
    ++ptr;
    while( ptr < end && *ptr <= ' ' ) ++ptr;
    if( *ptr != '\"' ) {
        printf( "Asset definition file '%s': expected asset bundle filename\n", asset_definitions_file );
        free( file );
        return NULL;;
    }
    ++ptr;
    char const* bundle_filename_start = ptr;
    while( ptr < end && *ptr != '\"' ) ++ptr;
    if( *ptr != '\"' ) {
        printf( "Asset definition file '%s': asset bundle filename not ending with \"\n", asset_definitions_file );
        free( file );
        return NULL;;
    }
    char const* bundle_filename_end = ptr;
    ++ptr;
    while( ptr < end && *ptr <= ' ' ) ++ptr;
    if( *ptr != ')' ) {
        printf( "Asset definition file '%s': expected ')' at the end of ASSETS_BEGIN definition\n", asset_definitions_file );
        free( file );
        return NULL;;
    }
    ++ptr;

    char bundle_filename[ 256 ] = "";
    if( bundle_filename_end - bundle_filename_start >= sizeof( bundle_filename ) ) {
        printf( "Asset definition file '%s': Asset bundle filename exceeds maximum length\n", asset_definitions_file );
        free( file );
        return NULL;;
    }
    strncpy( bundle_filename, bundle_filename_start, (size_t)( bundle_filename_end - bundle_filename_start ) );
    strcpy( out_bundle_filename, bundle_filename );

    int items_capacity = 256;
    struct item_t* items = (struct item_t*) malloc( sizeof( struct item_t ) * items_capacity );

    int index = 0;
    while( ptr < end ) {
        while( ptr < end && *ptr <= ' ' ) ++ptr;
        if( ptr >= end ) break;
        if( strncmp( ptr, "ASSET_", strlen( "ASSET_" ) ) != 0 ) {
            printf( "Asset definition file '%s': ASSET_... definition expected\n", asset_definitions_file );
            free( items );
            free( file );
            return NULL;;
        }
        ptr += strlen( "ASSET_" );
        char const* asset_type_start = ptr;
        while( ptr < end && isalnum( *ptr ) ) ++ptr;
        while( ptr < end && *ptr <= ' ' ) ++ptr;
        if( *ptr != '(' ) {
            printf( "Asset definition file '%s': expected '(' after ASSET_...\n", asset_definitions_file );
            free( items );
            free( file );
            return NULL;;
        }
        char const* asset_type_end = ptr;
        ++ptr;
        while( ptr < end && *ptr <= ' ' ) ++ptr;
        if( !isalpha( *ptr ) ) {
            printf( "Asset definition file '%s': invalid ASSET identifier\n", asset_definitions_file );
            free( items );
            free( file );
            return NULL;;
        }
        while( ptr < end && ( isalnum( *ptr ) || *ptr == '_' ) ) ++ptr;
        if( *ptr != ',' ) {
            printf( "Asset definition file '%s': expected ',' after ASSET identifier\n", asset_definitions_file );
            free( items );
            free( file );
            return NULL;;
        }
        ++ptr;
        while( ptr < end && *ptr <= ' ' ) ++ptr;
        if( *ptr != '\"' ) {
            printf( "Asset definition file '%s': expected filename in ASSET definition\n", asset_definitions_file );
            free( items );
            free( file );
            return NULL;;
        }
        ++ptr;
        char const* asset_filename_start = ptr;
        while( ptr < end && *ptr != '\"' ) ++ptr;
        if( *ptr != '\"' ) {
            printf( "Asset definition file '%s': asset filename not ending with \"\n", asset_definitions_file );
            free( items );
            free( file );
            return NULL;;
        }
        char const* asset_filename_end = ptr;
        ++ptr;

        while( ptr < end && *ptr <= ' ' ) ++ptr;
        if( *ptr != ')' ) {
            printf( "Asset definition file '%s': expected ')' at the end of ASSET definition\n", asset_definitions_file );
            free( items );
            free( file );
            return NULL;;
        }
        ++ptr;

        char asset_filename[ 256 ] = "";
        if( asset_filename_end - asset_filename_start >= sizeof( asset_filename ) ) {
            printf( "Asset definition file '%s': Asset filename exceeds maximum length\n", asset_definitions_file );
            free( items );
            free( file );
            return NULL;;
        }
        strncpy( asset_filename, asset_filename_start, (size_t)( asset_filename_end - asset_filename_start ) );

        char asset_type[ 64 ] = "";
        if( asset_type_end - asset_type_start >= sizeof( asset_type ) ) {
            printf( "Asset definition file '%s': Asset type name exceeds maximum length\n", asset_definitions_file );
            free( items );
            free( file );
            return NULL;;
        }
        strncpy( asset_type, asset_type_start, (size_t)( asset_type_end - asset_type_start ) );

        items[ index ].id = index;
        strcpy( items[ index ].filename, asset_filename );
        strcpy( items[ index ].type, asset_type );
        ++index;
    }
    while( ptr < end && *ptr <= ' ' ) ++ptr;
    if( strncmp( ptr, "ASSETS_END", strlen( "ASSETS_END" ) ) != 0 ) {
        printf( "Asset definition file '%s': 'ASSETS_END' expected at the end asset list\n", asset_definitions_file );
        free( items );
        free( file );
        return NULL;;
    }

    free( file );
    *count = index;
    return items;
}

PIXIE_BUILD_U32 palette_for_build_sprite[ 256 ]; // TODO: handle this properly
int palette_for_build_sprite_count = 256;
paldither_palette_t* paldither_palette_for_build_sprite = NULL;


void* build_palette( char const* filenames[], int count, int* out_size ) {
    if( count != 1 ) return 0;

    int w, h, c;
    stbi_uc* img = stbi_load( filenames[ 0 ], &w, &h, &c, 4 );
    if( !img ) return NULL;

    PIXIE_BUILD_U32 palette[ 256 ] = { 0 };
    int pal_count = 0;      
    for( int y = 0; y < h; ++y ) {
        for( int x = 0; x < w; ++x ) {
            PIXIE_BUILD_U32 pixel = ((PIXIE_BUILD_U32*)img)[ x + y * w ];
            if( ( pixel & 0xff000000 ) == 0 ) goto skip;
            if( pal_count < 256 ) {
                for( int i = 0; i < pal_count; ++i ) {
                    if( palette[ i ] == pixel )
                        goto skip;
                }
                palette[ pal_count ] = pixel;       
            }
            ++pal_count;
        skip:
            ;
        }
    }   
    if( pal_count > 256 )  {
        memset( palette, 0, sizeof( palette ) );
        pal_count = palettize_generate_palette_xbgr32( (PALETTIZE_U32*) img, w, h, palette, 256, 0 );        
    }
    memcpy( palette_for_build_sprite, palette, sizeof( palette_for_build_sprite ) );
    palette_for_build_sprite_count = pal_count;
    if( paldither_palette_for_build_sprite ) {
        paldither_palette_destroy( paldither_palette_for_build_sprite );
        paldither_palette_for_build_sprite = NULL;
    }
    stbi_image_free( img );     
    *out_size = sizeof( palette );
    void* out_data = malloc( sizeof( palette ) );
    memcpy( out_data, palette, sizeof( palette ) );
    return out_data;
}


void* build_sprite( char const* filenames[], int count, int* out_size ) {
    int capacity = 16 * 1024;
    PIXIE_BUILD_U8* data = (PIXIE_BUILD_U8*) malloc( (size_t) capacity );
    *(int*) data = count;
    int* offsets = (int*)( data + sizeof( int ) );
    int pos = (int)( sizeof( int ) + sizeof( int ) * count );
    for( int j = 0; j < count; ++j ) {
        int w, h, c;
        stbi_uc* img = stbi_load( filenames[ j ], &w, &h, &c, 4 );
        if( !img ) return NULL;

        if( !paldither_palette_for_build_sprite ) {
            paldither_palette_for_build_sprite = paldither_palette_create( palette_for_build_sprite, palette_for_build_sprite_count, NULL, NULL );
        }
   
        PIXIE_BUILD_U8* pixels = (PIXIE_BUILD_U8*) malloc( sizeof( PIXIE_BUILD_U8 ) * w * h );
        memset( pixels, 0, sizeof( PIXIE_BUILD_U8 ) * w * h ); 
        paldither_palettize( (PALDITHER_U32*) img, w, h, paldither_palette_for_build_sprite, PALDITHER_TYPE_DEFAULT, pixels );
    
        PIXIE_BUILD_U8* mask = (PIXIE_BUILD_U8*) malloc( (size_t) w * h );
        for( int i = 0; i < w * h; ++i ) mask[ i ] = (PIXIE_BUILD_U8)(( (PALETTIZE_U32*) img )[ i ] >> 24 );

        for( int i = 0; i < w * h; ++i ) {
            ((PALDITHER_U32*) img)[ i ] = ( ( (PALDITHER_U32)mask[ i ] ) << 24 ) | ( palette_for_build_sprite[ pixels[ i ] ] & 0xffffffff );
        }

        stbi_image_free( img );     

        palrle_data_t* rle = palrle_encode_mask( pixels, mask, w, h, palette_for_build_sprite, 256, NULL );
        free( mask );
        free( pixels );
    
        if( pos + (int) rle->size > capacity ) {
            if( capacity * 2 >= pos + (int) rle->size ) {
                capacity *= 2;
            } else {
                capacity = pos + (int) rle->size;
            }
            data = (PIXIE_BUILD_U8*) realloc( data, (size_t) capacity );
            offsets = (int*)( data + sizeof( int ) );
        }
        offsets[ j ] = pos;
        memcpy( data + pos, rle, (size_t) rle->size );
        pos += (int) rle->size;
        palrle_free( rle, NULL );
    }
    *out_size = pos;
    return data;
}


void* build_song( char const* filenames[], int count, int* out_size ) {
    if( count != 1 ) return 0;

    int in_size = 0;
    void* in_data = load_binary_file( filenames[ 0 ], &in_size );
    if( !in_data ) return NULL;

    mid_t* mid = mid_create( in_data, (size_t) in_size, NULL ); 
    free_binary_file( in_data );
    size_t size = mid_save_raw( mid, NULL, 0 );
    void* out_data = malloc( size );
    mid_save_raw( mid, out_data, size );
    mid_destroy( mid );
    *out_size = (int) size;
    return out_data;
}


void* build_text( char const* filenames[], int count, int* out_size ) {
    if( count != 1 ) return 0;

    int in_size = 0;
    char* in_data = load_text_file( filenames[ 0 ], &in_size );
    if( !in_data ) return NULL;

    void* out_data = malloc( (size_t) in_size );
    memcpy( out_data, in_data, (size_t) in_size );
    free_text_file( in_data );
    *out_size = in_size;
    return out_data;
}


void* build_binary( char const* filenames[], int count, int* out_size ) {
    if( count != 1 ) return 0;

    int in_size = 0;
    void* in_data = load_binary_file( filenames[ 0 ], &in_size );
    if( !in_data ) return NULL;

    void* out_data = malloc( (size_t) in_size );
    memcpy( out_data, in_data, (size_t) in_size );
    free_binary_file( in_data );
    *out_size = in_size;
    return out_data;
}

 
void* build_font( char const* filenames[], int count, int* out_size ) {
    if( count != 1 ) return 0;

    int in_size = 0;
    void* in_data = load_binary_file( filenames[ 0 ], &in_size );
    if( !in_data ) return NULL;

    unsigned char* ttf_data = (unsigned char*) in_data;

	stbtt_fontinfo font;
    stbtt_InitFont( &font, ttf_data, stbtt_GetFontOffsetForIndex( ttf_data, 0) );
    
    int size = 0;
    for( int i = 1; i < 32; ++i )
        {
        float scale = stbtt_ScaleForPixelHeight( &font, (float) i );
        int w, h;
        PIXELFONT_U8* bitmap = stbtt_GetGlyphBitmap( &font, scale, scale, 'A', &w, &h, 0, 0 );
        int empty = 1;
        int antialiased = 0;
        for( int j = 0; j < w * h; ++j )
            {
            if( bitmap[ j ] > 0 ) 
                {
                empty = 0;
                if( bitmap[ j ] < 255 ) { antialiased = 1; break; }
                }
            }
        stbtt_FreeBitmap( bitmap, 0 );
        if( !empty && !antialiased ) { size = i; break; }
        }

    if( !size ) 
		{
		free( in_data );
		//printf( "Not a pixel font (size detection failed): %s\n", filename );
		return NULL;
		}
    
    float scale = stbtt_ScaleForPixelHeight( &font, (float) size );

    int ascent, descent;
    stbtt_GetFontVMetrics(&font, &ascent, &descent, 0 );
    ascent = (int)( scale * ascent );
    descent = (int)( scale * descent );

    int x0, y0, x1, y1;
    stbtt_GetFontBoundingBox( &font, &x0, &y0, &x1, &y1 );
    x0 = (int)( scale * x0 );
    x1 = (int)( scale * x1 );
    y0 = (int)( scale * y0 );
    y1 = (int)( scale * y1 );

	int line_spacing = ascent - descent + 1; // TODO: verify this

	for( int c = 0; c < 256; ++c )
		{
		int gi = stbtt_FindGlyphIndex( &font, c );
		if (gi > 0 && gi < font.numGlyphs)
			{
            if( !stbtt_IsGlyphEmpty( &font, gi ) )
				{
                int ix0, iy0, ix1, iy1;
                stbtt_GetGlyphBitmapBox(&font, gi, scale, scale, &ix0, &iy0, &ix1, &iy1);
				iy0 += ascent;
				iy1 += ascent;
				if( ix0 < x0 ) x0 = ix0;
				if( iy0 < y0 ) y0 = iy0;
				if( ix1 > x1 ) x1 = ix1;
				if( iy1 > y1 ) y1 = iy1;
				}
			}
		}

    pixelfont_builder_t* builder = pixelfont_builder_create( y1 - y0 + 1, ascent, line_spacing, 0 );

	PIXELFONT_U8 dummy;
	pixelfont_builder_glyph( builder, 0, 0, &dummy, 0, 0 );
	for( int c = 1; c < 256; ++c )
        {
        int gi = stbtt_FindGlyphIndex( &font, c );
        if (gi > 0 && gi < font.numGlyphs)
            {
            int advance;
            int left;
            stbtt_GetGlyphHMetrics( &font, gi, &advance, &left );
            advance = (int)( scale * advance );
            left = (int)( scale * left );
            advance -= left;

            if( !stbtt_IsGlyphEmpty( &font, gi ) )
                {
                int ix0, iy0, ix1, iy1;
                stbtt_GetGlyphBitmapBox(&font, gi, scale, scale, &ix0, &iy0, &ix1, &iy1);
                int w, h, xo, yo;
                PIXELFONT_U8* bitmap = stbtt_GetGlyphBitmap( &font, scale, scale, gi, &w, &h, &xo, &yo );
				PIXELFONT_U8* temp_bmp = (PIXELFONT_U8*) malloc( (size_t) ( y1 - y0 + 1 ) * w  );
				memset( temp_bmp, 0, (size_t) ( y1 - y0 + 1 ) * w );
				int top = ascent + yo;
				top = top < 0 ? 0 : top;
				PIXELFONT_U8* out = temp_bmp + top * w;
                for( int y = 0; y < h; ++y ) 
					for( int x = 0; x < w; ++x ) 
						*out++ = bitmap[ x + y * w ] ? 1U : 0U; // font pixel
				pixelfont_builder_glyph( builder, c, w, temp_bmp, left, advance );
                if( bitmap ) stbtt_FreeBitmap( bitmap, NULL );
				if( temp_bmp ) free( temp_bmp );
                }
			else if( advance || left )
				{
				pixelfont_builder_glyph( builder, c, 0, &dummy, left, advance );
				}

            for( int k = 0; k < 256; ++k )
                {
                int kern = stbtt_GetCodepointKernAdvance( &font, c, k );
                kern = (int)( scale * kern );
                if( kern ) pixelfont_builder_kerning( builder, c, k, kern );
                }
            }
        }
	free( in_data );

	pixelfont_t* pixfont = pixelfont_builder_font( builder );
	if( !pixfont )
		{
		//printf( "Failed to generate font (possibly too big for format): %s\n", filename );
		pixelfont_builder_destroy( builder );
		return NULL;
		}

    void* out_data = malloc( pixfont->size_in_bytes );
    memcpy( out_data, pixfont, pixfont->size_in_bytes );
    *out_size = (int) pixfont->size_in_bytes;
	pixelfont_builder_destroy( builder );

    return out_data;
}


char** list_files( char const* filename, int* out_count ) {
    int count = 0;
    int capacity = 256;
    char** files = (char**) malloc( capacity * sizeof( char* ) );
    char const* path = c_dirname( filename );
    dir_t* dir = dir_open( *path == 0 ? "." : path );
    dir_entry_t* entry = dir_read( dir );
    while( entry ) {
        if( dir_is_file( entry ) && strcmp( dir_name( entry ), "." ) != 0 && strcmp( dir_name( entry ), ".." ) != 0 ) {
            char temp[ 1024 ];
            strcpy( temp, path );
            strcat( temp, dir_name( entry ) );
            if( wildcard_compare( filename, temp ) ) {
                if( count >= capacity ) {
                    capacity *= 2;
                    files = (char**) realloc( files, capacity * sizeof( char* ) );
                }
                files[ count++ ] = strdup( temp );
            }
        }
        entry = dir_read( dir );
    }
    dir_close( dir );
    *out_count = count;
    return files;
}


void free_file_list( char** filenames, int count ) {
    for( int i = 0; i < count; ++i ) {
        free( filenames[ i ] );
    }
    free( filenames );
}


int load_bundle( char const* filename, char const* time, char const* definitions, int count );


int build_and_load_assets( char const* bundle_filename, char const* build_time, char const* definitions_file, int definitions_line, int assets_count ) { 
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    (void) definitions_line; // TODO: verify definition line
    if( load_bundle( bundle_filename, build_time, definitions_file, assets_count ) == EXIT_SUCCESS ) {
        return EXIT_SUCCESS;
    }

    register_asset_type( "BINARY", build_binary );
    register_asset_type( "TEXT", build_text );
    register_asset_type( "PALETTE", build_palette );
    register_asset_type( "SPRITE", build_sprite );
    register_asset_type( "SONG", build_song );
    register_asset_type( "FONT", build_font );

    char parsed_bundle_filename[ 256 ];

    int count = 0;
    struct item_t* items = read_asset_definitions( definitions_file, &count, parsed_bundle_filename );
    if( !items ) return EXIT_FAILURE;

    if( count != assets_count || strcmp( parsed_bundle_filename, bundle_filename ) != 0 ) {
        printf( "bundle out of date\n" );
        return EXIT_FAILURE;
    }

    printf( "%s\n", parsed_bundle_filename );

    FILE* bundle = fopen( parsed_bundle_filename, "wb" );
    struct 
    {
        char file_id[ 20 ];
        int header_size;
        int assets_count;
        char bundle_file[ 256 ];
        char definitions_file[ 256 ];
        char build_time[ 64 ];
    } header;
    memset( &header, 0, sizeof( header ) );
    strcpy( header.file_id, "PIXIE_ASSETS_BUNDLE" );
    header.header_size = (int) sizeof( header );
    header.assets_count = count;
    strcpy( header.bundle_file, bundle_filename );
    strcpy( header.definitions_file, definitions_file );
    strcpy( header.build_time, build_time);
    fwrite( &header, 1, sizeof( header ), bundle );
    
    struct file_list_t {
        int id;
        int offset;
        int size;
    };
    struct file_list_t* file_list = (struct file_list_t*) malloc( sizeof( struct file_list_t ) * count );
    memset( file_list, 0, sizeof( struct file_list_t ) * count  );
    int file_list_pos = (int) ftell( bundle );
    fwrite( file_list, sizeof( struct file_list_t ), (size_t) count, bundle );

    memcpy( palette_for_build_sprite, default_palette(), sizeof( palette_for_build_sprite ) );

    int running_offset = (int) ftell( bundle );
    for( int i = 0; i < count; ++i ) {
        printf( "%d %s %s ", items[ i ].id, items[ i ].type, items[ i ].filename );

        int size = 0;
        void* data = NULL;
        asset_build_function_t build_function = NULL;
        for( int j = 0; j < pixie->build.count; ++j ) {
            if( stricmp( items[ i ].type, pixie->build.types[ j ].name ) == 0 ) {
                build_function = pixie->build.types[ j ].func;
                break;
            }
        }
        if( build_function ) {
            int files_count = 0;
            char const** filenames = (char const**)list_files( items[ i ].filename, &files_count );
            data = build_function( filenames, files_count, &size );
            free_file_list( (char**)filenames, files_count );
        } else {
            printf( "\n\nAsset type '%s' is unknown\n", items[ i ].type );
            free( items );
            free( file_list );
            return EXIT_FAILURE;
        }
        printf( "   %d bytes\n", size );

        if( data == NULL ) {
            printf( "\nAsset file '%s' could not be built\n", items[ i ].filename );
            free( items );
            free( file_list );
            return EXIT_FAILURE;
        }

        file_list[ i ].id = i;
        file_list[ i ].offset = running_offset;
        file_list[ i ].size = size;
        running_offset += size;
        fwrite( data, 1, (size_t) size, bundle );
        free( data );
    }
    printf( "%d bytes, %d assets\n", (int) ftell( bundle ), count );
    fseek( bundle, file_list_pos, SEEK_SET );
    fwrite( file_list, sizeof( struct file_list_t ), (size_t) count, bundle );
    free( file_list );
    fclose( bundle );
    free( items );
    if( paldither_palette_for_build_sprite ) {
        paldither_palette_destroy( paldither_palette_for_build_sprite );
        paldither_palette_for_build_sprite = NULL;
    }

    return load_bundle( bundle_filename, build_time, definitions_file, assets_count );
}


void register_asset_type( char const* name, asset_build_function_t build_function ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    strcpy( pixie->build.types[ pixie->build.count ].name, name );
    pixie->build.types[ pixie->build.count ].func = build_function;
    ++pixie->build.count;
}


void* load_binary_file( char const* filename, int* size ) {
    FILE* fp = fopen( filename, "rb" );
    if( fp == NULL ) return NULL;
    fseek( fp, 0, SEEK_END );
    int filesize = (int) ftell( fp );
    fseek( fp, 0, SEEK_SET );
    void* data = malloc( (size_t) filesize );
    filesize = (int) fread( data, 1, (size_t) filesize, fp );
    fclose( fp );
    if( size ) *size = filesize;
    return data;
}


void free_binary_file( void* data ){
    free( data );
}


char* load_text_file( char const* filename, int* size ) {
    FILE* fp = fopen( filename, "r" );
    if( fp == NULL ) return NULL;
    fseek( fp, 0, SEEK_END );
    int filesize = (int) ftell( fp );
    fseek( fp, 0, SEEK_SET );
    char* text = (char*) malloc( (size_t) filesize + 1 );
    filesize = (int) fread( text, 1, (size_t) filesize, fp );
    fclose( fp );
    text[ filesize ] = '\0';
    if( size ) *size = filesize;
    return text;
}


void free_text_file( char* text ) {
    free( text );
}


#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
} /* namespace pixie */
#endif


#define DIR_IMPLEMENTATION
#define DIR_WINDOWS
#include "dir.h"

#define FILE_UTIL_IMPLEMENTATION
#include "file_util.h"

#define PALDITHER_IMPLEMENTATION
#include "paldither.h"

#define PALETTIZE_IMPLEMENTATION
#include "palettize.h"

#define PIXELFONT_BUILDER_IMPLEMENTATION
#include "pixelfont.h"

#pragma warning( push )
#pragma warning( disable: 4255 )
#pragma warning( disable: 4296 ) 
#pragma warning( disable: 4365 )
#pragma warning( disable: 4668 )
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable: 4365 )
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_MAX_OVERSAMPLE 1
#define STBTT_RASTERIZER_VERSION 1
#include "stb_truetype.h"
#undef STB_TRUETYPE_IMPLEMENTATION
#pragma warning( pop )


#endif /* PIXIE_BUILD_IMPLEMENTATION */

        
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
