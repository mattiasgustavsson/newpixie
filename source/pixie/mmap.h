


typedef struct mmap_t mmap_t;

mmap_t* mmap_create( char const* filename, size_t size );

mmap_t* mmap_open( char const* filename, size_t size );

mmap_t* mmap_open_read_only( char const* filename, size_t size );

void mmap_close( mmap_t* map );

void* mmap_data( mmap_t* map );

size_t mmap_size( mmap_t* map );

char const* mmap_filename( mmap_t* map );


#ifdef MMAP_IMPLEMENTATION

#ifdef _WIN32
    #pragma warning( push )
    #pragma warning( disable: 4365 )
    #pragma warning( disable: 4619 )
    #pragma warning( disable: 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
    #pragma warning( disable: 4768 ) // __declspec attributes before linkage specification are ignored	
    #pragma warning( disable: 4255 ) // 'function' : no function prototype given: converting '()' to '(void)'
    #define _NTDDSCM_H_
    #include <windows.h>
    #pragma warning( pop )
#else
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/mman.h>
#endif


struct mmap_t
    {
    char filename[ 256 ];
    void* data;
    size_t size;

    #ifdef _WIN32
        HANDLE file_handle;
        HANDLE mmap_handle;
    #else
        int file_descriptor;
    #endif
    };


mmap_t* mmap_create( char const* filename, size_t size )
    {
    mmap_t* map = (mmap_t*) malloc( sizeof( mmap_t ) );
    memset( map, 0, sizeof( *map ) );
    strcpy( map->filename, filename );
    map->size = size;


    #ifdef _WIN32
        map->file_handle = CreateFileA( filename, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if( map->file_handle == (HANDLE) INVALID_HANDLE_VALUE )
            {
            free( map );
            return 0;
            }

        DWORD size_high = (DWORD)( ( (DWORD64) size ) >> 32 );
        DWORD size_low = (DWORD)( ( (DWORD64) size ) & 0xffffffff );
        map->mmap_handle = CreateFileMappingA( map->file_handle, NULL, PAGE_READWRITE, size_high, size_low, NULL );
        if( !map->mmap_handle )
            {
            CloseHandle( map->file_handle );
            free( map );
            return 0;
            }

        map->data = MapViewOfFile( map->mmap_handle, FILE_MAP_WRITE | FILE_MAP_READ, 0,  0, 0);
        if( !map->data )
            {
            CloseHandle( map->mmap_handle );
            CloseHandle( map->file_handle );
            free( map );
            return 0;
            }
    #else
        map->file_descriptor = open( filename, O_RDWR | O_CREAT, 0 );
        if( !map->file_descriptor )
            {
            free( map );
            return 0;
            }
        map->data = mmap( NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, map->file_descriptor, 0 );
        if( !map->data )
            {
            close( map->file_descriptor );
            free( map );
            return 0;
            }
    #endif

    return map;
    }


mmap_t* mmap_open( char const* filename, size_t size )
    {
    mmap_t* map = (mmap_t*) malloc( sizeof( mmap_t ) );
    memset( map, 0, sizeof( *map ) );
    strcpy( map->filename, filename );
    map->size = size;

    #ifdef _WIN32
        map->file_handle = CreateFileA( filename, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if( map->file_handle == (HANDLE) INVALID_HANDLE_VALUE )
            {
            free( map );
            return 0;
            }

        DWORD size_high = (DWORD)( ( (DWORD64) size ) >> 32 );
        DWORD size_low = (DWORD)( ( (DWORD64) size ) & 0xffffffff );
        map->mmap_handle = CreateFileMappingA( map->file_handle, NULL, PAGE_READWRITE, size_high, size_low, NULL );
        if( !map->mmap_handle )
            {
            CloseHandle( map->file_handle );
            free( map );
            return 0;
            }

        map->data = MapViewOfFile( map->mmap_handle, FILE_MAP_WRITE | FILE_MAP_READ, 0,  0, 0);
        if( !map->data )
            {
            CloseHandle( map->mmap_handle );
            CloseHandle( map->file_handle );
            free( map );
            return 0;
            }
    #else
        map->file_descriptor = open( filename, O_RDWR, 0 );
        if( !map->file_descriptor )
            {
            free( map );
            return 0;
            }
        map->data = mmap( NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, map->file_descriptor, 0 );
        if( !map->data )
            {
            close( map->file_descriptor );
            free( map );
            return 0;
            }
    #endif

    return map;
    }


mmap_t* mmap_open_read_only( char const* filename, size_t size )
    {
    mmap_t* map = (mmap_t*) malloc( sizeof( mmap_t ) );
    memset( map, 0, sizeof( *map ) );
    strcpy( map->filename, filename );
    map->size = size;

    #ifdef _WIN32
        map->file_handle = CreateFileA( filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if( map->file_handle == (HANDLE) INVALID_HANDLE_VALUE )
            {
            free( map );
            return 0;
            }

        DWORD size_high = (DWORD)( ( (DWORD64) size ) >> 32 );
        DWORD size_low = (DWORD)( ( (DWORD64) size ) & 0xffffffff );
        map->mmap_handle = CreateFileMappingA( map->file_handle, NULL, PAGE_READONLY, size_high, size_low, NULL );
        if( !map->mmap_handle )
            {
            CloseHandle( map->file_handle );
            free( map );
            return 0;
            }

        map->data = MapViewOfFile( map->mmap_handle, FILE_MAP_READ, 0,  0, 0);
        if( !map->data )
            {
            CloseHandle( map->mmap_handle );
            CloseHandle( map->file_handle );
            free( map );
            return 0;
            }
    #else
        map->file_descriptor = open( filename, O_RDONLY, 0 );
        if( !map->file_descriptor )
            {
            free( map );
            return 0;
            }
        map->data = mmap( NULL, size, PROT_READ, MAP_SHARED, map->file_descriptor, 0 );
        if( !map->data )
            {
            close( map->file_descriptor );
            free( map );
            return 0;
            }
    #endif

    return map;
    }


void mmap_close( mmap_t* map )
    {
    if( !map ) return;
    #ifdef _WIN32
        UnmapViewOfFile( map->data );
        CloseHandle( map->mmap_handle );
        CloseHandle( map->file_handle );
    #else
        munmap( map->data, map->size );
        close( map->file_descriptor );
    #endif
    free( map );
    }


void* mmap_data( mmap_t* map )
    {
    if( !map ) return NULL;
    return map->data;
    }


size_t mmap_size( mmap_t* map )
    {
    if( !map ) return 0;
    return map->size;
    }


char const* mmap_filename( mmap_t* map )
    {
    if( !map ) return NULL;
    return map->filename;
    }


#endif /* MMAP_IMPLEMENTATION */