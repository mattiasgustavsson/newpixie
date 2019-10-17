


typedef struct mmap_t mmap_t;

mmap_t* mmap_create( char const* filename, size_t size );

mmap_t* mmap_open( char const* filename, size_t size );

mmap_t* mmap_open_read_only( char const* filename, size_t size );

void mmap_close( mmap_t* mmap );

void* mmap_data( mmap_t* mmap );

size_t mmap_size( mmap_t* mmap );

char const* mmap_filename( mmap_t* mmap );


#ifdef MMAP_IMPLEMENTATION

#pragma warning( push )
#pragma warning( disable: 4365 )
#pragma warning( disable: 4619 )
#pragma warning( disable: 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
#pragma warning( disable: 4768 ) // __declspec attributes before linkage specification are ignored	
#pragma warning( disable: 4255 ) // 'function' : no function prototype given: converting '()' to '(void)'
#define _NTDDSCM_H_
#include <windows.h>
#pragma warning( pop )


struct mmap_t
    {
    char filename[ 256 ];
    void* data;
    size_t size;

    HANDLE file_handle;
    HANDLE mmap_handle;
    };


mmap_t* mmap_create( char const* filename, size_t size )
    {
    mmap_t* mmap = (mmap_t*) malloc( sizeof( mmap_t ) );
    memset( mmap, 0, sizeof( *mmap ) );
    strcpy( mmap->filename, filename );
    mmap->size = size;

    mmap->file_handle = CreateFileA( filename, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if( mmap->file_handle == (HANDLE) INVALID_HANDLE_VALUE )
        {
        free( mmap );
        return 0;
        }

    DWORD size_high = (DWORD)( ( (DWORD64) size ) >> 32 );
    DWORD size_low = (DWORD)( ( (DWORD64) size ) & 0xffffffff );
    mmap->mmap_handle = CreateFileMappingA( mmap->file_handle, NULL, PAGE_READWRITE, size_high, size_low, NULL );
    if( !mmap->mmap_handle )
        {
        CloseHandle( mmap->file_handle );
        free( mmap );
        return 0;
        }

    mmap->data = MapViewOfFile( mmap->mmap_handle, FILE_MAP_WRITE | FILE_MAP_READ, 0,  0, 0);
    if( !mmap->data )
        {
        CloseHandle( mmap->mmap_handle );
        CloseHandle( mmap->file_handle );
        free( mmap );
        return 0;
        }

    return mmap;
    }


mmap_t* mmap_open( char const* filename, size_t size )
    {
    mmap_t* mmap = (mmap_t*) malloc( sizeof( mmap_t ) );
    memset( mmap, 0, sizeof( *mmap ) );
    strcpy( mmap->filename, filename );
    mmap->size = size;

    mmap->file_handle = CreateFileA( filename, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if( mmap->file_handle == (HANDLE) INVALID_HANDLE_VALUE )
        {
        free( mmap );
        return 0;
        }

    DWORD size_high = (DWORD)( ( (DWORD64) size ) >> 32 );
    DWORD size_low = (DWORD)( ( (DWORD64) size ) & 0xffffffff );
    mmap->mmap_handle = CreateFileMappingA( mmap->file_handle, NULL, PAGE_READWRITE, size_high, size_low, NULL );
    if( !mmap->mmap_handle )
        {
        CloseHandle( mmap->file_handle );
        free( mmap );
        return 0;
        }

    mmap->data = MapViewOfFile( mmap->mmap_handle, FILE_MAP_WRITE | FILE_MAP_READ, 0,  0, 0);
    if( !mmap->data )
        {
        CloseHandle( mmap->mmap_handle );
        CloseHandle( mmap->file_handle );
        free( mmap );
        return 0;
        }

    return mmap;
    }


mmap_t* mmap_open_read_only( char const* filename, size_t size )
    {
    mmap_t* mmap = (mmap_t*) malloc( sizeof( mmap_t ) );
    memset( mmap, 0, sizeof( *mmap ) );
    strcpy( mmap->filename, filename );
    mmap->size = size;

    mmap->file_handle = CreateFileA( filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if( mmap->file_handle == (HANDLE) INVALID_HANDLE_VALUE )
        {
        free( mmap );
        return 0;
        }

    DWORD size_high = (DWORD)( ( (DWORD64) size ) >> 32 );
    DWORD size_low = (DWORD)( ( (DWORD64) size ) & 0xffffffff );
    mmap->mmap_handle = CreateFileMappingA( mmap->file_handle, NULL, PAGE_READONLY, size_high, size_low, NULL );
    if( !mmap->mmap_handle )
        {
        CloseHandle( mmap->file_handle );
        free( mmap );
        return 0;
        }

    mmap->data = MapViewOfFile( mmap->mmap_handle, FILE_MAP_READ, 0,  0, 0);
    if( !mmap->data )
        {
        CloseHandle( mmap->mmap_handle );
        CloseHandle( mmap->file_handle );
        free( mmap );
        return 0;
        }

    return mmap;
    }


void mmap_close( mmap_t* mmap )
    {
    if( !mmap ) return;
	UnmapViewOfFile( mmap->data );
    CloseHandle( mmap->mmap_handle );
    CloseHandle( mmap->file_handle );
    free( mmap );
    }



void* mmap_data( mmap_t* mmap )
    {
    return mmap->data;
    }


size_t mmap_size( mmap_t* mmap )
    {
    if( !mmap ) return 0;
    return mmap->size;
    }


char const* mmap_filename( mmap_t* mmap )
    {
    return mmap->filename;
    }


#endif /* MMAP_IMPLEMENTATION */