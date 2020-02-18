/*
------------------------------------------------------------------------------
		  Licensing information can be found at the end of the file.
------------------------------------------------------------------------------

file_util.h - v0.1 - Utilities for working with files, folders and paths.

Do this:
	#define FILE_UTIL_IMPLEMENTATION
before you include this file in *one* C/C++ file to create the implementation.

Dependencies:
	dir.h
*/

#ifndef file_util_h
#define file_util_h

#define _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS
#include <time.h>

char const* c_basename( char const* path, char const* extension );
char const* c_dirname( char const* path );
char const* c_extname( char const* path );
char* basename( char* path, char* extension );
char* dirname( char* path );
char* extname( char* path );

char* pathcat( char* destination, char const* source );

char const* absolute_path( char const* filename );
void replace_extension( char* output, char const* path, char const* new_extension );

void file_util_mkdir( char const* path );
void file_util_rmdir( char const* path );
void change_dir( char const* path );
int create_path( char const* path ); 
void copy_file( char const* source, char const* destination );
void move_file( char const* source, char const* destination );
void delete_file( char const* filename );

size_t file_size( char const* filename );
time_t file_last_changed( char const* filename );
time_t most_recent_file_modification( char const* path );
int file_more_recent( char const* source_path, char const* output_path );
int contains_more_recent_file( char const* source_path, char const* output_path );

int folder_count( char const* path );
int file_count( char const* path );
int folder_exists( char const* path );
int file_exists( char const* filename );
int is_folder( char const* path );

int generate_temp_filename( char const* path, char const* prefix, char* filename, size_t capacity );

int wildcard_compare( char const* wild, char const* string );

#endif /* file_util_h */    

/*
----------------------
	IMPLEMENTATION
----------------------
*/

#ifdef FILE_UTIL_IMPLEMENTATION
#undef FILE_UTIL_IMPLEMENTATION

#define _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
	#include <sys/stat.h>
	#if !defined( _WIN32_WINNT ) || _WIN32_WINNT < 0x0501 
	    #undef _WIN32_WINNT
	    #define _WIN32_WINNT 0x0501 // requires Windows XP minimum
	#endif
	// 0x0400=Windows NT 4.0, 0x0500=Windows 2000, 0x0501=Windows XP, 0x0502=Windows Server 2003, 0x0600=Windows Vista, 
	// 0x0601=Windows 7, 0x0602=Windows 8, 0x0603=Windows 8.1, 0x0A00=Windows 10, 
	#define _WINSOCKAPI_
	#pragma warning( push )
    #pragma warning( disable: 4365 )
    #pragma warning( disable: 4619 )
	#pragma warning( disable: 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
    #pragma warning( disable: 4768 ) // __declspec attributes before linkage specification are ignored	
	#pragma warning( disable: 4255 ) // 'function' : no function prototype given: converting '()' to '(void)'
	#include <windows.h>
	#pragma warning( pop )

	#include <direct.h>
#else /* _WIN32 */
	#include <limits.h>
	#include <stdlib.h>
	//#error unsupported platform
#endif

#include "dir.h"

#ifndef MAX_PATH
	#define MAX_PATH 260
#endif


char const* c_basename( char const* path, char const* extension )
	{
	static char result[ MAX_PATH ];
	strcpy( result, "" );

	if( path )
		{
		char const* lastForwardSlash = strrchr( path, '/' );
		char const* lastBackSlash = strrchr( path, '\\' );
		
		char const* name = 0;
		
		if( !lastBackSlash && !lastForwardSlash )
			{
			name = path;
			}
		else if( !lastBackSlash )
			{
			name = lastForwardSlash + 1;
			}
		else if( !lastForwardSlash )
			{
			name = lastBackSlash + 1;
			}
		else if( lastForwardSlash > lastBackSlash )
			{
			name = lastForwardSlash + 1;
			}
		else
			{
			name = lastBackSlash + 1;
			}
			
		strncpy( result, name, sizeof( result ) );
			
		if( extension )
			{
			size_t extlen = strlen( extension );
			size_t reslen = strlen( result );
			
			if( reslen >= extlen )
				{
				#ifdef _WIN32
				if( stricmp( result + (reslen - extlen), extension ) == 0 )
				#else
				if( strcasecmp( result + (reslen - extlen), extension ) == 0 )
				#endif
					{
					result[ reslen - extlen ] = 0;
					}
				}
			
			}
		}

	return result;
	}


char const* c_dirname( char const* path )
	{
	static char result[ MAX_PATH ];
	strcpy( result, "" );

	if( path )
		{
		char* lastForwardSlash;
		char* lastBackSlash;
		
		strncpy( result, path, sizeof( result ) );
		lastForwardSlash = strrchr( result, '/' );
		lastBackSlash = strrchr( result, '\\' );
		
		if( !lastBackSlash && !lastForwardSlash )
			{
			result[ 0 ] = 0;
			}
		else if( !lastBackSlash )
			{
			*(lastForwardSlash + 1) = 0;
			}
		else if( !lastForwardSlash )
			{
			*(lastBackSlash +1 ) = 0;
			}
		else if( lastForwardSlash > lastBackSlash )
			{
			*(lastForwardSlash + 1 ) = 0;
			}
		else
			{
			*(lastBackSlash + 1) = 0;
			}
		}

	return result;
	}

	
char const* c_extname( char const* path )
	{
	static char result[ MAX_PATH ];
	strcpy( result, "" );

	if( path )
		{
		char const* lastForwardSlash = strrchr( path, '/' );
		char const* lastBackSlash = strrchr( path, '\\' );
		
		char const* name = 0;
		char const* ext = 0;
		
		if( !lastBackSlash && !lastForwardSlash )
			{
			name = path;
			}
		else if( !lastBackSlash )
			{
			name = lastForwardSlash + 1;
			}
		else if( !lastForwardSlash )
			{
			name = lastBackSlash + 1;
			}
		else if( lastForwardSlash > lastBackSlash )
			{
			name = lastForwardSlash + 1;
			}
		else
			{
			name = lastBackSlash + 1;
			}
			
		ext = strrchr( name, '.' );
		
		if( ext && !( ext[ 0 ] == '.' && ext[ 1 ] == 0 ) )
			{
			strncpy( result, ext, sizeof( result ) );            
			}
		}
 
	return result;
	}  
   

char* basename( char* path, char* extension ) { return (char*) c_basename( (char const*)path, (char const*)extension ); }
char* dirname( char* path ) { return (char*) c_dirname( (char const*)path ); }
char* extname( char* path ) { return (char*) c_extname( (char const*)path ); }

char* pathcat( char* destination, char const* source )
	{
	int all_back = 1;

	size_t len = strlen( destination );
	for( size_t i = 0; i < len; ++i ) if( destination[ i ] == '/' ) all_back = 0;

	size_t slen = strlen( source );
	for( size_t i = 0; i < slen; ++i ) if( source[ i ] == '/' ) all_back = 0;

	char const* end = destination + len;
	if( len == 0 || end[ -1 ] == '\\' || end[ -1 ] == '/' )
		return strcat( destination, source );
	else
		return strcat( strcat( destination, all_back ? "\\" : "/" ), source );
	}


char const* absolute_path( char const* filename )
	{
	static char path[ MAX_PATH ];
	#ifdef _WIN32
		if( _fullpath( path, filename, MAX_PATH ) )
			return path;
	#else
		if( realpath( path, (char*)filename ) )
			return path;
	#endif
	return 0;


/*	if( filename[0] == 0 || filename[1] == ':' )
		{
		return filename;
		}

	char cwd[ MAX_PATH ];
	#ifdef _WIN32
		getcwd( cwd, MAX_PATH );
	#else 
		#error unsupported platform
	#endif

	static char path[ MAX_PATH ];
	if( filename[0] == '\\' || filename[0] == '/' )
		{
		path[ 0 ] = cwd[ 0 ];
		path[ 1 ] = ':';
		path[ 2 ] = 0;
		strcat( strcat( path, "/"), filename );
		}
	else
		{
		strcat( strcat( strcpy( path, cwd ), "/"), filename );
		}

	char* str = path;
	while( *str )
		{
		if( *str == '/' )
			{
			*str = '\\';
			}
		str++;
		}

    char* end = &path[ strlen( path ) - 1 ];
    if( *end == '\\' ) *end = 0;

    while( stricmp( basename( path, extname( path ) ), "." ) == 0 )
        {
        strcpy( path, dirname( path ) );
        end = &path[ strlen( path ) - 1 ];
        if( *end == '\\' ) *end = 0;
        }

// 	str = strstr( path, "\\.." );
// 	if( str == path
// 	while( str )
// 		{
// 		char* prev = str - 1;
// 		while( prev > path && 
// 		str = strstr( path, ".." );
// 		}
	return path;
*/
	}


void replace_extension( char* output, char const* filename, char const* new_extension )
	{
	strcat( strcat( strcpy( output, c_dirname( filename ) ), c_basename( filename, c_extname( filename ) ) ), new_extension );	
	}


void file_util_mkdir( char const* path )
	{
	#ifdef _WIN32
		CreateDirectoryA( path, NULL );
	#else /* _WIN32 */
		// #error unsupported platform /* TODO */
	#endif
	}


void file_util_rmdir( char const* path )
	{
	#ifdef _WIN32
		RemoveDirectoryA( path );
	#else /* _WIN32 */
		// #error unsupported platform /* TODO */
	#endif
	}


void change_dir( char const* path )
	{
	#ifdef _WIN32
		SetCurrentDirectoryA( path );
	#else /* _WIN32 */
		// #error unsupported platform /* TODO */
	#endif
	}


static int recursive_create_path( char* fullpath,  char* dirname )
	{
	if( dirname )
		{
		char* next_dir = strtok( 0, "/\\" ); 
	    dir_t* dir = dir_open( fullpath );

	    if( dir )
		    {
		    int dir_found = 0;
		    size_t len;
		    
		    dir_entry_t* ent = dir_read( dir );
		    while( ent )
			    {
				#ifdef _WIN32
			    if( stricmp( dir_name( ent ), dirname ) == 0 )
				#else
			    if( strcasecmp( dir_name( ent ), dirname ) == 0 )
				#endif
			        {
			        if( dir_is_folder( ent ) )
				        {
				        dir_found = 1;
				        break;
			            }
			        else if( dir_is_file( ent ) )
			            {
			            dir_close( dir );
			            return 0;
			            }
			        }
				ent = dir_read( dir );
			    }

			dir_close( dir );
		    
			len = strlen( fullpath );
			if( len > 0 && fullpath[ len - 1 ] != '\\' && fullpath[ len - 1 ] != '/')
				{
				strncat( fullpath, "/", MAX_PATH - strlen( fullpath ) );
				}
			strncat( fullpath, dirname, MAX_PATH - strlen( fullpath ) );

		    if( !dir_found )
		        {
		        file_util_mkdir( fullpath );
		        }

			if( next_dir )
				{
				return recursive_create_path( fullpath, next_dir );
				}
			else
				{
				return 1;
				}

		    }    
		}

	return 0;
	}


int create_path( char const* path )
	{
	int retval = 0;
	
	if( path )
		{
		char fullpath[ MAX_PATH ] = "";
		char str[ MAX_PATH ];
		strncpy( str, path, MAX_PATH );
		if( str[1] == ':' )
			{
			strncpy( fullpath, str, 3);
			retval = recursive_create_path( fullpath, strtok( str + 2, "/\\" ) );        
			}
		else
			{
			strcpy( fullpath, "./" );
			retval = recursive_create_path( fullpath, strtok( str, "/\\" ) );        
			}
		}
	
	return retval;
	}


void copy_file( char const* source, char const* destination )
	{
	#ifdef _WIN32
		CopyFileA( source, destination, FALSE );
	#else /* _WIN32 */
		// #error unsupported platform /* TODO */
	#endif
	}


void move_file( char const* source, char const* destination )
	{
	#ifdef _WIN32
		MoveFileA( source, destination );
	#else /* _WIN32 */
		// #error unsupported platform /* TODO */
	#endif
	}


void delete_file( char const* filename )
	{
	#ifdef _WIN32
		BOOL res = DeleteFileA( filename );
		if( !res )
			{
			DWORD error = GetLastError();
			error = error;
			}
	#else /* _WIN32 */
		// #error unsupported platform /* TODO */
	#endif
	}


size_t file_size( char const* filename )
	{
	if( filename )
		{
		struct stat result;
		int ret;   
		ret = stat( filename, &result );
		if( ret  == 0 )
	        {
			return (size_t) result.st_size;
	        }
		}
	    
	return 0;	    
	}


time_t file_last_changed( char const* filename )
	{
	if( filename )
		{
		struct stat result;
		int ret;   
		ret = stat( filename, &result );
		if( ret  == 0 )
	        {
			return result.st_mtime;
	        }
		}
	    
	return 0;	    
	}


time_t most_recent_file_modification( char const* path )
	{
	time_t time = 0;
	
	if( path )
		{
		time = file_last_changed( path );
		dir_t* dir = dir_open( path );
		if( dir )
			{
			dir_entry_t* ent = dir_read( dir );
			while( ent )
				{
				if( dir_is_file( ent ) )
					{
					char filename[ MAX_PATH ];
					strcat( strcat ( strcpy( filename, path ), "/" ), dir_name( ent ) );
					time_t file_time = file_last_changed( filename );
					if (file_time > time )
						{
						time = file_time;
						}                    
					}
			
				ent = dir_read( dir );
				}
			dir_close( dir );
			}
		}

	return time;
	}
		

int file_more_recent( char const* source_path,  char const* output_path  )
	{
	return file_last_changed( source_path ) > file_last_changed( output_path );
	}


int contains_more_recent_file( char const* source_path,  char const* output_path )
	{
	time_t source_time = 0;
	time_t output_time = 0; 
	
	if( is_folder( source_path ) )
		{
		source_time = most_recent_file_modification( source_path );    
		}
	else
		{
		source_time = file_last_changed( source_path );        
		}
	
	if( is_folder( output_path ) )
		{
		output_time = most_recent_file_modification( output_path );    
		}
	else
		{
		output_time = file_last_changed( output_path );        
		}

	return source_time > output_time;
	}


int folder_count( char const* path )
	{
	int count = 0;
	
	if( path )
		{
		dir_t* dir = dir_open( path );
		if( dir )
			{
			dir_entry_t* ent = dir_read( dir );
			while( ent )
				{
				char const* d_name = dir_name( ent );
				int namlen = (int) strlen( d_name );
				if( dir_is_folder( ent ) && namlen > 0 && strcmp( d_name, "." ) != 0 && strcmp( d_name, ".." ) != 0 )
					{
					count++;
					}
			
				ent = dir_read( dir );
				}
			dir_close( dir );
			}
		}

	return count;
	}


int file_count( char const* path )
	{
	int count = 0;
	
	if( path )
		{
		dir_t* dir = dir_open( path );
		if( dir )
			{
			dir_entry_t* ent = dir_read( dir );
			while( ent )
				{
				if( dir_is_file( ent ) )
					{
					count++;
					}
			
				ent = dir_read( dir );
				}
			dir_close( dir );
			}
		}

	return count;
	}
	
	
int folder_exists( char const* path )
	{
	struct stat result;
	int ret;   
	ret = stat( path, &result );
	if( ret == 0 )
		{
		return result.st_mode & S_IFDIR;
		}

	return 0;
	}


int file_exists( char const* filename )
	{
	struct stat result;
	int ret;   
	ret = stat( filename, &result );
	if( ret == 0 )
		{
		return result.st_mode & S_IFREG;
		}

	return 0;
	}


int is_folder( char const* path )
	{
	if( path )
		{
		struct stat result;
		int ret;   
		ret = stat( path, &result );
		if( ret  == 0 )
	        {
			return result.st_mode & S_IFDIR;
	        }
		}
	    
	return 0;	    
	}


int generate_temp_filename( char const* path, char const* prefix, char* filename, size_t capacity )
    {
	#ifdef _WIN32		
		char str[ MAX_PATH ] = "";
		UINT res = GetTempFileNameA( path, prefix, 0, str );
		if( !res ) return 0;
		if( filename )
			{
			strncpy( filename, str, capacity );
			filename[ capacity - 1 ] = '\0';
			}
		return (int)strlen( str );
	#else
		return 0; /* TODO */
	#endif
    }


int wildcard_compare( char const* wild, char const* string )
	{
	char const* cp = NULL;
	char const* mp = NULL;

	while( *string && *wild != '*' )
		{
		if( tolower( *wild ) != tolower( *string ) && *wild != '?' ) return 0;
		wild++;
		string++;
		}

	while( *string )
		{
		if( *wild == '*' )
			{
			if( !*++wild ) return 1;
			mp = wild;
			cp = string + 1;
			}
		else if( tolower( *wild ) == tolower( *string ) || *wild == '?' )
			{
			++wild;
			++string;
			}
		else
			{
			wild = mp;
			string = cp++;
			}
		}

	while (*wild == '*') wild++;
	return !*wild;
	}



#endif /* FILE_UTIL_IMPLEMENTATION */

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
