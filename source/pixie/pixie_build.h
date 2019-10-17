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
            void build_and_load_assets( char const* bundle_filename, char const* build_time, \
                char const* definitions_file, int definitions_line, int assets_count ); \
            inline void load_assets( void ) { \
                char const* bundle_filename; \
                int definitions_line; \
                multiple_pixie_ASSETS_BEGIN_declarations_not_allowed( &bundle_filename, &definitions_line ); \
                build_and_load_assets( bundle_filename, __DATE__ " " __TIME__, __FILE__, definitions_line, \
                TEMP_PIXIE_ASSETS_COUNT ); \
            } \
        } /* namespace pixie */
#else
    #define ASSETS_BEGIN( bundle_filename_param ) \
        inline void multiple_pixie_ASSETS_BEGIN_declarations_not_allowed( char const** bundle_filename, int* line ) \
            { *bundle_filename = bundle_filename_param; *line = __LINE__; } \
        enum assets_t {

    #define ASSETS_END() TEMP_PIXIE_ASSETS_COUNT }; \
        void build_and_load_assets( char const* bundle_filename, char const* build_time, \
            char const* definitions_file, int definitions_line, int assets_count ); \
        inline void load_assets( void ) { \
            char const* bundle_filename; \
            int definitions_line; \
            multiple_pixie_ASSETS_BEGIN_declarations_not_allowed( &bundle_filename, &definitions_line ); \
            build_and_load_assets( bundle_filename, __DATE__ " " __TIME__, __FILE__, definitions_line, \
            TEMP_PIXIE_ASSETS_COUNT ); \
        } 
#endif

#define ASSET_PALETTE( id, filename ) id,
#define ASSET_SPRITE( id, filename ) id,
#define ASSET_SONG( id, filename ) id,


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


// C runtime includes
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Libraries includes
#include "palettize.h"
#include "pixie_data.h"
#include "stb_image.h"


#ifndef PIXIE_BUILD_U8
    #define PIXIE_BUILD_U8 unsigned char
#endif

#ifndef PIXIE_BUILD_U32
    #define PIXIE_BUILD_U32 unsigned int
#endif


#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
namespace pixie {
#endif


struct item_t {
    int id;
    char filename[ 256 ];
    char type[ 64 ];
};

static struct item_t* read_asset_definitions( char const* asset_definitions_file, int* count, char out_bundle_filename[ 256 ] ) { 
    FILE* fp = fopen( asset_definitions_file, "r" );
    if( fp == NULL ) {
        printf( "Asset definition file '%s' could not be opened\n", asset_definitions_file );
        return NULL;;
    }
    fseek( fp, 0, SEEK_END );
    size_t size = (size_t) ftell( fp );
    fseek( fp, 0, SEEK_SET );
    char* file = (char*) malloc( size + 1 );
    size = fread( file, 1, size, fp );
    file[ size ] = '\0';
    fclose( fp );

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



static void* build_palette( char const* filename, int* size ) {
    int w, h, c;
    stbi_uc* img = stbi_load( filename, &w, &h, &c, 4 );
    if( !img ) return NULL;

    PIXIE_BUILD_U32 palette[ 256 ] = { 0 };
    int count = 0;      
    for( int y = 0; y < h; ++y ) {
        for( int x = 0; x < w; ++x ) {
            PIXIE_BUILD_U32 pixel = ((PIXIE_BUILD_U32*)img)[ x + y * w ];
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
    *size = sizeof( palette );
    void* ret = malloc( sizeof( palette ) );
    memcpy( ret, palette, sizeof( palette ) );
    return ret;
}


static void* build_sprite( PIXIE_BUILD_U32 palette[ 256 ], char const* filename, int* size ) {
    int w, h, c;
    stbi_uc* img = stbi_load( filename, &w, &h, &c, 4 );
    if( !img ) return NULL;
   
    struct {
        PIXIE_BUILD_U32 width;
        PIXIE_BUILD_U32 height;
        PIXIE_BUILD_U8 pixels[ 1 ];
    }* data = VOID_CAST( malloc( sizeof( *data ) + sizeof( PIXIE_BUILD_U8 ) * ( w * h - 1 ) ) );
    data->width = w;
    data->height = h;
    memset( data->pixels, 0, sizeof( PIXIE_BUILD_U8 ) * w * h ); 
    palettize_remap_xbgr32( (PALETTIZE_U32*) img, w, h, palette, 256, data->pixels );
    
    for( int i = 0; i < w * h; ++i )
        if( ( ( (PALETTIZE_U32*) img )[ i ] & 0xff000000 ) >> 24 < 0x80 )
            data->pixels[ i ] |=  0x80u;       
    
    stbi_image_free( img );     

    *size = sizeof( *data ) + sizeof( PIXIE_BUILD_U8 ) * ( w * h - 1 );
    return data;
}



int load_bundle( char const* filename, char const* time, char const* definitions, int count );

void build_and_load_assets( char const* bundle_filename, char const* build_time, char const* definitions_file, int definitions_line, int assets_count ) { 
    (void) definitions_line; // TODO: verify definition line
    if( load_bundle( bundle_filename, build_time, definitions_file, assets_count ) == EXIT_SUCCESS ) {
        return;
    }

    char parsed_bundle_filename[ 256 ];

    int count = 0;
    struct item_t* items = read_asset_definitions( definitions_file, &count, parsed_bundle_filename );
    if( !items ) return;

    if( count != assets_count || strcmp( parsed_bundle_filename, bundle_filename ) != 0 ) {
        printf( "bundle out of date\n" );
        return;
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

    PIXIE_BUILD_U32 palette[ 256 ];
    memcpy( palette, default_palette(), sizeof( palette ) );

    int running_offset = (int) ftell( bundle );
    for( int i = 0; i < count; ++i ) {
        printf( "%d %s %s\n", items[ i ].id, items[ i ].type, items[ i ].filename );

        if( strcmp( items[ i ].type, "PALETTE" ) == 0 ) {
            int size = 0;
            void* data = build_palette( items[ i ].filename, &size );
            if( data == NULL ) {
                printf( "Asset file '%s' could not be opened\n", items[ i ].filename );
                free( items );
                free( file_list );
                return;
            }
            memcpy( palette, data, sizeof( palette ) ); // Store most recent palette
            file_list[ i ].id = i;
            file_list[ i ].offset = running_offset;
            file_list[ i ].size = size;
            running_offset += size;
            fwrite( data, 1, (size_t) size, bundle );
            free( data );
        } else if( strcmp( items[ i ].type, "SPRITE" ) == 0 ) {
            int size = 0;
            void* data = build_sprite( palette, items[ i ].filename, &size );
            if( data == NULL ) {
                printf( "Asset file '%s' could not be opened\n", items[ i ].filename );
                free( items );
                free( file_list );
                return;
            }
            file_list[ i ].id = i;
            file_list[ i ].offset = running_offset;
            file_list[ i ].size = size;
            running_offset += size;
            fwrite( data, 1, (size_t) size, bundle );
            free( data );
        } else {
            FILE* fp = fopen( items[ i ].filename, "rb" );
            if( fp == NULL ) {
                printf( "Asset file '%s' could not be opened\n", items[ i ].filename );
                free( items );
                free( file_list );
                return;
            }
            fseek( fp, 0, SEEK_END );
            size_t sz = (size_t) ftell( fp );
            fseek( fp, 0, SEEK_SET );
            void* data = malloc( sz );
            sz = fread( data, 1, sz, fp );
            fclose( fp );

            file_list[ i ].id = i;
            file_list[ i ].offset = running_offset;
            file_list[ i ].size = (int)sz;
            running_offset += (int)sz;
            fwrite( data, 1, sz, bundle );
            free( data );
        }
    }
    fseek( bundle, file_list_pos, SEEK_SET );
    fwrite( file_list, sizeof( struct file_list_t ), (size_t) count, bundle );
    free( file_list );
    fclose( bundle );
    free( items );

    load_bundle( bundle_filename, build_time, definitions_file, assets_count );
}


#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
} /* namespace pixie */
#endif



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
