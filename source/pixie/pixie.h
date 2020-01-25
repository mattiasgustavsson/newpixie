/*
------------------------------------------------------------------------------
          Licensing information can be found at the end of the file.
------------------------------------------------------------------------------

pixie.h - v0.1 - Retro game dev library for C/C++.

Do this:
    #define PIXIE_IMPLEMENTATION
before you include this file in *one* C/C++ file to create the implementation.
*/


#ifndef pixie_h
#define pixie_h

/*
----------
    API 
---------
*/

#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
namespace pixie {
#endif


// Sized types
#ifndef PIXIE_I8 
    typedef signed char i8;
#else
    typedef PIXIE_I8 i8;
#endif
#ifndef PIXIE_I16 
    typedef signed short i16;
#else
    typedef PIXIE_I16 i16;
#endif
#ifndef PIXIE_I32
    typedef signed int i32;
#else
    typedef PIXIE_I32 i32;
#endif
#ifndef PIXIE_I64
    typedef signed long long i64;
#else
    typedef PIXIE_I64 i64;
#endif
#ifndef PIXIE_U8 
    typedef unsigned char u8;
#else
    typedef PIXIE_U8 u8;
#endif
#ifndef PIXIE_U16 
    typedef unsigned short u16;
#else
    typedef PIXIE_U16 u16;
#endif
#ifndef PIXIE_U32
    typedef unsigned int u32;
#else
    typedef PIXIE_U32 u32;
#endif
#ifndef PIXIE_U64
    typedef unsigned long long u64;
#else
    typedef PIXIE_U64 u64;
#endif


// API Functions

int run( int (*main)( int, char** ) );
void end( int return_code );
void wait_vbl( void );
void wait( int jiffys );

int fullscreen( void );
void fullscreen_on( void );
void fullscreen_off( void );

int crt_mode( void);
void crt_mode_on( void );
void crt_mode_off( void );

void print( char const* str );
void load_palette( int asset );

typedef struct rgb_t {
    int r;
    int g;
    int b;
} rgb_t;

void setcol( int index, rgb_t rgb );
rgb_t getcol( int index );

void sprites_off( void );

int sprite( int spr_index, int x, int y, int asset );
void sprite_bitmap( int spr_index, int asset );
int sprite_visible( int spr_index );
void sprite_show( int spr_index );
void sprite_hide( int spr_index );
void sprite_pos( int spr_index, int x, int y );
int sprite_x( int spr_index );
int sprite_y( int spr_index );
void sprite_origin( int spr_index, int x, int y );
int sprite_origin_x( int spr_index );
int sprite_origin_y( int spr_index );
void sprite_cel( int spr_index, int cel );

typedef enum text_align_t { TEXT_ALIGN_LEFT, TEXT_ALIGN_RIGHT, TEXT_ALIGN_CENTER, } text_align_t;
int label( int spr_index, int x, int y, char const* text, int color, int font_asset );
int label_text( int spr_index, char const* text );
int label_align( int spr_index, text_align_t align );
int label_color( int spr_index, int color );
int label_outline( int spr_index, int color );
int label_shadow( int spr_index, int color );
int label_wrap( int spr_index, int wrap );

typedef enum move_type_t {
    MOVE_DELAY, MOVE_LINEAR, MOVE_SMOOTHSTEP, MOVE_SMOOTHERSTEP, MOVE_EASE_OUT_QUAD, MOVE_EASE_OUT_BACK, 
    MOVE_EASE_OUT_BOUNCE, MOVE_EASE_OUT_SINE, MOVE_EASE_OUT_ELASTIC, MOVE_EASE_OUT_EXPO, MOVE_EASE_OUT_CUBIC, 
    MOVE_EASE_OUT_QUART, MOVE_EASE_OUT_QUINT, MOVE_EASE_OUT_CIRCLE, MOVE_EASE_IN_QUAD, MOVE_EASE_IN_BACK, 
    MOVE_EASE_IN_BOUNCE, MOVE_EASE_IN_SINE, MOVE_EASE_IN_ELASTIC, MOVE_EASE_IN_EXPO, MOVE_EASE_IN_CUBIC, 
    MOVE_EASE_IN_QUART, MOVE_EASE_IN_QUINT, MOVE_EASE_IN_CIRCLE, MOVE_EASE_IN_OUT_QUAD, MOVE_EASE_IN_OUT_BACK, 
    MOVE_EASE_IN_OUT_BOUNCE, MOVE_EASE_IN_OUT_SINE, MOVE_EASE_IN_OUT_ELASTIC, MOVE_EASE_IN_OUT_EXPO, 
    MOVE_EASE_IN_OUT_CUBIC, MOVE_EASE_IN_OUT_QUART, MOVE_EASE_IN_OUT_QUINT,MOVE_EASE_IN_OUT_CIRCLE,
} move_type_t;

typedef struct move_t {
    int target;
    int duration;
    move_type_t type;
} move_t;

void move_sprite_x( int spr_index, move_t* moves, int moves_count );
void move_sprite_y( int spr_index, move_t* moves, int moves_count );

#define ARRAY_COUNT( x ) ( (int)( sizeof( x ) / sizeof( *x ) ) )

void set_soundfont( int asset );
void play_song( int asset );
char const* load_text( int asset_id );

int asset_size( int asset_id );
void const* asset_data( int asset_id );

void text( int x, int y, char const* str, int color, int font_asset 
	/*, text_align align, int wrap_width, int hspacing, int vspacing, int limit, bool bold, bool italic, bool underline */);

typedef enum key_t { 
	KEY_INVALID, KEY_LBUTTON, KEY_RBUTTON, KEY_CANCEL, KEY_MBUTTON,  KEY_XBUTTON1, KEY_XBUTTON2, KEY_BACK, KEY_TAB, 
	KEY_CLEAR, KEY_RETURN, KEY_SHIFT,  KEY_CONTROL, KEY_MENU, KEY_PAUSE, KEY_CAPITAL, KEY_KANA, KEY_HANGUL = KEY_KANA, 
	KEY_JUNJA, KEY_FINAL, KEY_HANJA, KEY_KANJI = KEY_HANJA, KEY_ESCAPE, KEY_CONVERT, KEY_NONCONVERT, KEY_ACCEPT, 
	KEY_MODECHANGE, KEY_SPACE, KEY_PRIOR, KEY_NEXT, KEY_END,  KEY_HOME, KEY_LEFT, KEY_UP, KEY_RIGHT, KEY_DOWN, 
	KEY_SELECT, KEY_PRINT, KEY_EXEC, KEY_SNAPSHOT, KEY_INSERT, KEY_DELETE, KEY_HELP, KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, 
	KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, 
	KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, KEY_LWIN, 
	KEY_RWIN, KEY_APPS, KEY_SLEEP, KEY_NUMPAD0, KEY_NUMPAD1, KEY_NUMPAD2, KEY_NUMPAD3, KEY_NUMPAD4, KEY_NUMPAD5, 
	KEY_NUMPAD6, KEY_NUMPAD7, KEY_NUMPAD8, KEY_NUMPAD9, KEY_MULTIPLY, KEY_ADD, KEY_SEPARATOR, KEY_SUBTRACT, KEY_DECIMAL, 
	KEY_DIVIDE, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, 
	KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17, KEY_F18, KEY_F19, KEY_F20, KEY_F21, KEY_F22, KEY_F23, KEY_F24, 
	KEY_NUMLOCK, KEY_SCROLL, KEY_LSHIFT, KEY_RSHIFT, KEY_LCONTROL, KEY_RCONTROL, KEY_LMENU, KEY_RMENU, KEY_BROWSER_BACK, 
	KEY_BROWSER_FORWARD, KEY_BROWSER_REFRESH, KEY_BROWSER_STOP, KEY_BROWSER_SEARCH, KEY_BROWSER_FAVORITES, 
	KEY_BROWSER_HOME, KEY_VOLUME_MUTE, KEY_VOLUME_DOWN, KEY_VOLUME_UP, KEY_MEDIA_NEXT_TRACK, KEY_MEDIA_PREV_TRACK, 
	KEY_MEDIA_STOP, KEY_MEDIA_PLAY_PAUSE, KEY_LAUNCH_MAIL, KEY_LAUNCH_MEDIA_SELECT, KEY_LAUNCH_APP1, KEY_LAUNCH_APP2, 
	KEY_OEM_1, KEY_OEM_PLUS, KEY_OEM_COMMA, KEY_OEM_MINUS, KEY_OEM_PERIOD, KEY_OEM_2, KEY_OEM_3, KEY_OEM_4, KEY_OEM_5, 
	KEY_OEM_6, KEY_OEM_7, KEY_OEM_8, KEY_OEM_102, KEY_PROCESSKEY, KEY_ATTN, KEY_CRSEL, KEY_EXSEL, KEY_EREOF, KEY_PLAY, 
	KEY_ZOOM, KEY_NONAME, KEY_PA1, KEY_OEM_CLEAR, KEY_PEN_TOUCH, KEY_PEN_LOWER_BUTTON, KEY_PEN_UPPER_BUTTON, KEYCOUNT 
} key_t;

int key_is_down( key_t key );
int key_was_pressed( key_t key );
int key_was_released( key_t key );


int min( int a, int b );

// String functions

#ifndef PIXIE_MAX_STRING_LENGTH 
    #define PIXIE_MAX_STRING_LENGTH 256
#endif

typedef struct string {
    char c_str[ PIXIE_MAX_STRING_LENGTH ];
} string;

string str( char const* c_str ); // create a pixie string from a c string
int length( string str ); // number of characters in a string
string concat( string a, string b ); // concatenate string a and string b
int compare( string a, string b ); // returns 0 if strings are equal, <0 if a comes before b, >0 if b comes before a
string trim( string str ); // remove leading and trailing whitespace
string ltrim( string str ); // remove leading whitespace 
string rtrim( string str );  // remove trailing whitespace
string left( string source, int number ); // return the leftmost characters of a string
string right( string source, int number );  // return the rightmost characters of a string
string mid( string source, int offset, int number );  // return a number of characters from the middle of a string
int instr( string haystack, string needle, int start );  // search for occurrences of one string within another string
int any( string haystack, string needles, int start );  // search for the next occurrence of any character of one string within another string
string upper( string str );  // convert a string of text to upper case
string lower( string str ); // convert a string of text to lower case
string string_from_int( int x ); // convert a number into a string
string string_from_float( float x ); // convert a number into a string
float float_from_string( string str ); // convert a string of digits into a floating point value
int int_from_string( string str ); // convert a string of digits into an integer value
string space ( int number ); // space out a string
string flip( string original ); //invert a string
string repeat( string text, int number ); // repeat a string
string chr( int code ); // return the character with a given ASCII code
int asc( string str ); // give the ASCII code of a character
int len( string str ); // give the length of a string
int starts_with( string str, string start );
string format( string format_string, ... ); // printf style formatting


// Hash functions

u32 hash_str( string str );

// TODO: hash funcs for more types



#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
} /* namespace pixie */
#endif

#define LOOP for( ; ; )

#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
    namespace pixie {
        int display_assert_message( char const* expression, char const* message, char const* file, int line, char const* function );
        char const* format_assert_message( char const* format, ... );
    } /* namespace pixie */

	#if defined NDEBUG && !defined( PIXIE_ASSERT_IN_RELEASE_BUILD )
		#define ASSERT( expression, message )
	#elif defined PIXIE_ASSERT
		#define ASSERT( expression, message ) PIXIE_ASSERT( expression, message )
    #else
		#ifdef __TINYC__ 
			#define ASSERT( expression, message ) \
				do { \
					if( !( expression ) ) \
						if( pixie::display_assert_message( #expression, message, __FILE__, __LINE__, __FUNCTION__ ) ) \
							__asm__ volatile("int $0x03"); \
				} while( 0 )
		#else
			#define ASSERT( expression, message ) \
				__pragma( warning( push ) ) \
				__pragma( warning( disable: 4127 ) ) \
				do { \
					if( !( expression ) ) \
						if( pixie::display_assert_message( #expression, message, __FILE__, __LINE__, __FUNCTION__ ) ) \
							__debugbreak(); \
				} while( 0 ) \
				__pragma( warning( pop ) )
		#endif
	#endif

    #define ASSERTF( expression, message ) ASSERT( expression, pixie::format_assert_message message )
#else
    int display_assert_message( char const* expression, char const* message, char const* file, int line, char const* function );
    char const* format_assert_message( char const* format, ... );

	#if defined NDEBUG && !defined( PIXIE_ASSERT_IN_RELEASE_BUILD )
		#define ASSERT( expression, message )
	#elif defined PIXIE_ASSERT
		#define ASSERT( expression, message ) PIXIE_ASSERT( expression, message )
    #else
		#ifdef __TINYC__ 
			#define ASSERT( expression, message ) \
				do { \
					if( !( expression ) ) \
						if( display_assert_message( #expression, message, __FILE__, __LINE__, __FUNCTION__ ) ) \
							__asm__ volatile("int $0x03"); \
				} while( 0 )
		#else
			#define ASSERT( expression, message ) \
				__pragma( warning( push ) ) \
				__pragma( warning( disable: 4127 ) ) \
				do { \
					if( !( expression ) ) \
						if( display_assert_message( #expression, message, __FILE__, __LINE__, __FUNCTION__ ) ) \
							__debugbreak(); \
				} while( 0 ) \
				__pragma( warning( pop ) )
		#endif		
	#endif

    #define ASSERTF( expression, message ) ASSERT( expression, format_assert_message message )
#endif


/*
----------------------------------
    ASSET BUILD/BUNDLE SUPPORT
----------------------------------
*/

#define ASSET_BINARY( id, filename ) id,
#define ASSET_TEXT( id, filename ) id,
#define ASSET_PALETTE( id, filename ) id,
#define ASSET_SPRITE( id, filename ) id,
#define ASSET_SONG( id, filename ) id,
#define ASSET_FONT( id, filename ) id,

#ifdef PIXIE_NO_BUILD
    // If data builds are disabled, we just define the functions to load a bundle, not create it. With/without namespace
    #if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
        #define ASSETS_BEGIN( bundle_filename ) \
            namespace pixie { \
                int load_bundle( char const* filename, char const* time, char const* definitions, int count ); \
                static int load_assets( void ) { return load_bundle( bundle_filename, (void*) 0, (void*) 0, -1 ); } \
                enum assets_t {
        #define ASSETS_END() \
                } /* enum assets_t */; \
            } /* namespace pixie */
    #else
        #define ASSETS_BEGIN( bundle_filename ) \
            int load_bundle( char const* filename, char const* time, char const* definitions, int count ); \
            static int load_assets( void ) { return load_bundle( bundle_filename, (void*) 0, (void*) 0, -1 ); } \
            enum assets_t {
        #define ASSETS_END() \
            } /* enum assets_t */;
    #endif
#else
    // If data builds are enabled, include the build function declarations from external file. Definitions are included
    // in the implementation section with other external library implementations
    #include "pixie_build.h"
#endif



/*
---------------------
    MATH WRAPPERS
---------------------
*/

// As we want pixie to provide everything needed, we want it to expose math functions line `sin` and `cos`, so you don't
// have to include <math.h>. But the good names are already taken, so we use some #defines to "steal" the names which
// are already used by <math.h>. It's a bit iffy though, and if you'd rather just use <math.h>, this whole thing can be
// disabled with `PIXIE_NO_MATH`. 
#ifndef PIXIE_NO_MATH
    #if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
        // If namespace is enabled, we can just declared namespaced functions with the names we want, no macros needed
        namespace pixie {
            float acos( float x );
            float asin( float x );
            float atan( float x );
            float atan2( float x, float y );
            float ceil( float x );
            float cos( float x );
            float cosh( float x );
            float exp( float x );
            float fabs( float x );
            float floor( float x );
            float fmod( float x, float y );
            float log( float x );
            float log10( float x );
            float modf( float x, float* y );
            float pow( float x, float y );
            float sqrt( float x );
            float sin( float x );
            float sinh( float x );
            float tan( float x );
            float tanh( float x );
        } /* namespace pixie */
    #else /* defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE ) */
        // If we are in C or have disabled namespace, we forward declare our functions with unique names, and then use
        // macros to give them the good names. In the implementation, we will temporarily undefine them while including
        // <math.h> to avoid name conflicts, and then redefine them again, so the exposed symbols of `sin`, `cos` etc.
        // will be mapped to `internal_pixie_sin`, `internal_pixie_cos` etc. which will call the <math.h> `sin`, `cos`
        float internal_pixie_acos( float x );
        float internal_pixie_asin( float x );
        float internal_pixie_atan( float x );
        float internal_pixie_atan2( float x, float y );
        float internal_pixie_ceil( float x );
        float internal_pixie_cos( float x );
        float internal_pixie_cosh( float x );
        float internal_pixie_exp( float x );
        float internal_pixie_fabs( float x );
        float internal_pixie_floor( float x );
        float internal_pixie_fmod( float x, float y );
        float internal_pixie_log( float x );
        float internal_pixie_log10( float x );
        float internal_pixie_modf( float x, float* y );
        float internal_pixie_pow( float x, float y );
        float internal_pixie_sqrt( float x );
        float internal_pixie_sin( float x );
        float internal_pixie_sinh( float x );
        float internal_pixie_tan( float x );
        float internal_pixie_tanh( float x );
        #define acos internal_pixie_acos
        #define asin internal_pixie_asin
        #define atan internal_pixie_atan
        #define atan2 internal_pixie_atan2
        #define ceil internal_pixie_ceil
        #define cos internal_pixie_cos 
        #define cosh internal_pixie_cosh
        #define exp internal_pixie_exp 
        #define fabs internal_pixie_fabs
        #define floor internal_pixie_floor
        #define fmod internal_pixie_fmod
        #define log internal_pixie_log 
        #define log10 internal_pixie_log10
        #define modf internal_pixie_modf
        #define pow internal_pixie_pow 
        #define sqrt internal_pixie_sqrt
        #define sin internal_pixie_sin 
        #define sinh internal_pixie_sinh
        #define tan internal_pixie_tan 
        #define tanh internal_pixie_tanh
    #endif /* defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE ) */
#endif /* PIXIE_NO_MATH */

      
/*
----------------------
    DICTIONARY TYPE
----------------------
*/

#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
    #define INTERNAL_DICTIONARY_U32 pixie::u32
#else
    #define INTERNAL_DICTIONARY_U32 u32
#endif

#define DICTIONARY_TYPE( NAME, HASH_FUNC, KEY_TYPE, KEY_COMPARE, ITEM_TYPE, CAPACITY ) \
    typedef struct NAME##_type { \
        int count; \
        struct { \
            INTERNAL_DICTIONARY_U32 key_hash; \
            int item_index; \
            int base_count; \
        } slots[ CAPACITY + CAPACITY / 2 ]; \
        KEY_TYPE items_key[ CAPACITY ]; \
        ITEM_TYPE items_data[ CAPACITY ]; \
        int items_slot[ CAPACITY ]; \
        int item_capacity; \
    } NAME##_type; \
    \
    static void NAME##_clear( NAME##_type* dict ) { \
        dict->count = 0; \
        int const slot_capacity = ( CAPACITY + CAPACITY / 2 ); \
        for( int i = 0; i < slot_capacity; ++i ) { \
            dict->slots[ i ].key_hash = 0; \
            dict->slots[ i ].item_index = 0; \
            dict->slots[ i ].base_count = 0; \
        } \
    } \
    \
    static int NAME##_count( NAME##_type* dict ) { \
        ASSERTF( dict->count >= 0 && dict->count <= CAPACITY, \
            ( "Invalid dictionary count detected when querying size of dictionary of type '%s'.\n\n" \
            "The invalid count is: %d", #NAME, dict->count ) ); \
        if( dict->count >= 0 && dict->count < CAPACITY ) { \
            return dict->count; \
        } else {\
            return 0; \
        } \
    } \
    \
    static void NAME##_insert( NAME##_type* dict, KEY_TYPE key, ITEM_TYPE value ) { \
        ASSERTF( dict->count >= 0 && dict->count <= CAPACITY, \
            ( "Invalid dictionary count detected when querying size of dictionary of type '%s'.\n\n" \
            "The invalid count is: %d", #NAME, dict->count ) ); \
        ASSERTF( dict->count < CAPACITY, \
            ( "Capacity exceed when inserting an item into dictionary of type '%s'.\n\n" \
            "Max CAPACITY is: %d", #NAME, CAPACITY ) ); \
        if( dict->count >= 0 && dict->count < CAPACITY ) { \
            INTERNAL_DICTIONARY_U32 const hash = HASH_FUNC( key ); \
            INTERNAL_DICTIONARY_U32 const slot_capacity = (INTERNAL_DICTIONARY_U32)( CAPACITY + CAPACITY / 2 ); \
            int const base_slot = (int)( hash % slot_capacity ); \
            int base_count = dict->slots[ base_slot ].base_count; \
            int slot = base_slot; \
            int first_free = slot; \
            while( base_count ) { \
                if( dict->slots[ slot ].item_index < 0 && dict->slots[ first_free ].item_index >= 0 ) { \
                    first_free = slot; \
                } \
                INTERNAL_DICTIONARY_U32 const slot_hash = dict->slots[ slot ].key_hash; \
                int slot_base = (int)( slot_hash % slot_capacity ); \
                if( slot_base == base_slot ) { \
                    --base_count; \
                } \
                slot = (int)( ( slot + 1 ) % slot_capacity ); \
            } \
            \
            slot = first_free; \
            while( dict->slots[ slot ].item_index > 0  ) { \
                slot = (int)( ( slot + 1 ) % slot_capacity ); \
            } \
            \
            ASSERTF( dict->slots[ slot ].item_index <= 0 && (int)( hash % slot_capacity ) == (INTERNAL_DICTIONARY_U32) base_slot, \
                ( "Internal error" ) ); \
            dict->slots[ slot ].key_hash = hash; \
            dict->slots[ slot ].item_index = dict->count + 1; \
            ++dict->slots[ base_slot ].base_count; \
            \
            dict->items_key[ dict->count ] = key; \
            dict->items_data[ dict->count ] = value; \
            dict->items_slot[ dict->count ] = slot; \
            ++dict->count; \
        } \
    } \
    \
    KEY_TYPE* NAME##_find( NAME##_type* dict, KEY_TYPE key ) { \
        INTERNAL_DICTIONARY_U32 const hash = HASH_FUNC( key ); \
        INTERNAL_DICTIONARY_U32 const slot_capacity = (INTERNAL_DICTIONARY_U32)( CAPACITY + CAPACITY / 2 ); \
        int const base_slot = (int)( hash % slot_capacity ); \
        \
        int base_count = dict->slots[ base_slot ].base_count; \
        int slot = base_slot; \
        while( base_count > 0 ) { \
            if( dict->slots[ slot ].item_index > 0 ) { \
                INTERNAL_DICTIONARY_U32 slot_hash = dict->slots[ slot ].key_hash; \
                int slot_base = (int)( slot_hash % slot_capacity ); \
                if( slot_base == base_slot ) { \
                    ASSERTF( base_count > 0, ( "Internal error" ) ); \
                    if( slot_hash == hash ) { \
                        if( KEY_COMPARE( dict->items_key[ dict->slots[ slot ].item_index - 1 ], key ) == 0 ) { \
                            int const index = dict->slots[ slot ].item_index - 1; \
                            return &dict->items_data[ index ]; \
                        } \
                    } \
                    --base_count; \
                } \
            } \
            slot = (int)( ( slot + 1 ) % slot_capacity ); \
        } \
        return NULL; \
    }
  
// TODO: remove, try_insert, array access, swap

/*
------------------
    ARRAY TYPE
------------------
*/

#define ARRAY_TYPE( NAME, TYPE, CAPACITY ) \
    typedef struct NAME##_type { \
        int count; \
        TYPE items[ CAPACITY ]; \
    } NAME##_type; \
    \
    static int NAME##_count( NAME##_type* arr ) { \
        ASSERTF( arr->count >= 0 && arr->count <= CAPACITY, \
            ( "Invalid array count detected when querying size of array of TYPE '%s'.\n\n" \
            "The invalid count is: %d", #NAME, arr->count ) ); \
        if( arr->count >= 0 && arr->count < CAPACITY ) { \
            return arr->count; \
        } else {\
            return 0; \
        } \
    } \
    \
    static void NAME##_add( NAME##_type* arr, TYPE value ) { \
        ASSERTF( arr->count >= 0 && arr->count <= CAPACITY, \
            ( "Invalid array count detected when adding an item to array of TYPE '%s'.\n\n" \
            "The invalid count is: %d", #NAME, arr->count ) ); \
        ASSERTF( arr->count < CAPACITY, \
            ( "Capacity exceed when adding an item to array of TYPE '%s'.\n\n" \
            "Max CAPACITY is: %d", #NAME, CAPACITY ) ); \
        if( arr->count >= 0 && arr->count < CAPACITY ) { \
            arr->items[ arr->count++ ] = value; \
        } \
    } \
    \
    static int NAME##_try_add( NAME##_type* arr, TYPE value ) { \
        ASSERTF( arr->count >= 0 && arr->count <= CAPACITY, \
            ( "Invalid array count detected when trying to add an item to array of TYPE '%s'.\n\n" \
            "The invalid count is: %d", #NAME, arr->count ) ); \
        if( arr->count >= 0 && arr->count < CAPACITY ) { \
            arr->items[ arr->count++ ] = value; \
            return 1; \
        } else { \
            return 0; \
        } \
    } \
    \
    static void NAME##_remove( NAME##_type* arr, int index ) { \
        ASSERTF( arr->count >= 0 && arr->count <= CAPACITY, \
            ( "Invalid array count detected when removing an item from array of TYPE '%s'.\n\n" \
            "The invalid count is: %d", #NAME, arr->count ) ); \
        ASSERTF( index >= 0 && index < arr->count, \
            ( "Invalid index when removing an item from array of TYPE '%s'.\n\n" \
            "The invalid index is: %d\nThe current count is: %d", #NAME, index, arr->count ) ); \
        if( arr->count >= 0 && arr->count <= CAPACITY && index >= 0 && index < arr->count ) { \
            if( arr->count == 1 ) { \
                --arr->count; \
            } else { \
                arr->items[ index ] = arr->items[ --arr->count ]; \
            } \
        } \
    } \
    \
    static int NAME##_try_remove( NAME##_type* arr, int index ) { \
        ASSERTF( arr->count >= 0 && arr->count <= CAPACITY, \
            ( "Invalid array count detected when trying to remove an item from array of TYPE '%s'.\n\n" \
            "The invalid count is: %d", #NAME, arr->count ) ); \
        if( arr->count >= 0 && arr->count <= CAPACITY && index >= 0 && index < arr->count ) { \
            if( arr->count == 1 ) { \
                --arr->count; \
            } else { \
                arr->items[ index ] = arr->items[ --arr->count ]; \
            } \
            return 1; \
        } else { \
            return 0; \
        } \
    } \
    \
    static TYPE NAME##_get( NAME##_type const* arr, int index ) { \
        ASSERTF( arr->count >= 0 && arr->count <= CAPACITY, \
            ( "Invalid array count detected when getting an item from array of TYPE '%s'.\n\n" \
            "The invalid count is: %d", #NAME, arr->count ) ); \
        ASSERTF( index >= 0 && index < arr->count, \
            ( "Invalid index when getting an item from array of TYPE '%s'.\n\n" \
            "The invalid index is: %d\nThe current count is: %d", #NAME, index, arr->count ) ); \
        if( arr->count >= 0 && arr->count <= CAPACITY && index >= 0 && index < arr->count ) { \
            return arr->items[ index ]; \
        } else { \
            u8 value[ sizeof( TYPE ) ]; \
            for( int i = 0; i < sizeof( value ); ++i ) value[ i ] = 0; \
            return *(TYPE*)value; \
        } \
    } \
    \
    static TYPE const* NAME##_try_get( NAME##_type const* arr, int index ) { \
        ASSERTF( arr->count >= 0 && arr->count <= CAPACITY, \
            ( "Invalid array count detected when getting an item from array of TYPE '%s'.\n\n" \
            "The invalid count is: %d", #NAME, arr->count ) ); \
        if( arr->count >= 0 && arr->count <= CAPACITY && index >= 0 && index < arr->count ) { \
            return &arr->items[ index ]; \
        } else { \
            return 0; \
        } \
    }


/*
--------------
    SORTING
--------------
*/

#define SORT_FUNCTION( NAME, TYPE, COMPARE ) \
    struct internal_sort_stack_##NAME##_t { \
        int start; \
        int count; \
    }; \
    \
    static int internal_sort_##NAME##_min( int a, int b ) { \
	    return ( ( a < b ) ? a : b ); \
    } \
    \
	static TYPE* internal_sort_##NAME##_med3( TYPE* a, TYPE* b, TYPE* c ) { \
		return ( COMPARE( *a, *b ) < 0 \
			? ( COMPARE( *b, *c ) < 0 ? b : COMPARE( *a, *c ) < 0 ? c : a ) \
			: ( COMPARE( *b, *c ) > 0 ? b : COMPARE( *a, *c ) > 0 ? c : a) ); \
    } \
    \
	static void internal_sort_##NAME##_swap( TYPE* a, TYPE* b ) { \
		TYPE t = *a; \
        *a = *b; \
        *b = t; \
    } \
	\
	static void internal_sort_##NAME##_swap_range( TYPE* a, TYPE* b, int n ) { \
		int sn = (n); \
        TYPE* sa = (a); \
        TYPE* sb = (b); \
        while( sn > 0 ) { \
            internal_sort_##NAME##_swap( sa, sb ); \
            ++sa; \
            ++sb; \
            --sn; \
        } \
    }\
    \
    static void NAME( TYPE* array, int count ) { \
	    struct internal_sort_stack_##NAME##_t stack[ 32 ]; \
	    \
	    int top = 0; \
	    stack[ top ].start = 0; \
	    stack[ top ].count = count; \
        \
	    while ( top >= 0 ) { \
		    TYPE* a = array + stack[ top ].start; \
		    count = stack[ top-- ].count; \
		    \
		    if( count < 24 ) { /* Insertion sort on smallest arrays */ \
			    for( TYPE* pm = a + 1; pm < a + count; ++pm ) {\
				    for( TYPE* pl = pm; pl > a && COMPARE( *( pl - 1 ), *pl ) > 0; --pl ) { \
					    internal_sort_##NAME##_swap( pl, pl - 1 ); \
                    } \
                } \
			    continue; \
			} \
		    TYPE* pm = a + count / 2; /* Small arrays, middle element */ \
		    if( count > 40 ) { /* Big arrays, pseudomedian of 9 */ \
			    TYPE* pl = a; \
			    TYPE* pn = a + count - 1; \
			    int s = count / 8; \
			    pl = internal_sort_##NAME##_med3( pl, pl + s, pl + 2 * s ); \
			    pm = internal_sort_##NAME##_med3( pm - s, pm, pm + s ); \
			    pn = internal_sort_##NAME##_med3( pn - 2 * s, pn - s, pn ); \
			    pm = internal_sort_##NAME##_med3( pl, pm, pn ); /* Mid-size, med of 3 */ \
			} \
		    TYPE* pv = a;  \
            internal_sort_##NAME##_swap( pv, pm ); /* pv points to partition value */ \
		    TYPE* pa = a; \
		    TYPE* pb = a; \
		    TYPE* pc = a + count - 1; \
		    TYPE* pd = pc; \
		    for( ;; ) { \
			    int r; \
			    while( pb <= pc && ( r = COMPARE( *pb, *pv ) ) <= 0 ) { \
				    if( r == 0 ) { \
                        internal_sort_##NAME##_swap( pa, pb ); \
                        ++pa; \
                    } \
				    ++pb; \
				} \
			    while( pc >= pb && ( r = COMPARE( *pc, *pv ) ) >= 0 ) { \
				    if( r == 0 ) { \
                        internal_sort_##NAME##_swap( pc, pd ); \
                        --pd; \
                    } \
				    --pc; \
				} \
			    if( pb > pc ) break; \
			    internal_sort_##NAME##_swap( pb, pc ); \
			    ++pb; \
                --pc; \
            } \
		    TYPE* pn = a + count; \
		    int s = internal_sort_##NAME##_min( (int)( pa - a ), (int)( pb - pa ) ); \
            internal_sort_##NAME##_swap_range( a, pb - s, s ); \
		    s = internal_sort_##NAME##_min( (int)( pd - pc ), (int)( pn - pd - 1 ) ); \
            internal_sort_##NAME##_swap_range( pb, pn - s, s ); \
		    if( ( s = (int)( pb - pa ) ) > 1 ) { \
			    if( ++top >= sizeof( stack) / sizeof( *stack ) ) { \
                    --top; \
                    NAME( a, s ); \
                } \
			    else { \
                    stack[ top ].start = (int)( a - array ); \
                    stack[ top ].count = s; \
                } \
			} \
		    if( ( s = (int)( pd - pc ) ) > 1 ) { \
			    if( ++top >= sizeof( stack) / sizeof( *stack ) ) { \
                    --top; \
                    NAME( pn - s, s ); \
                } else { \
                    stack[ top ].start = (int)( ( pn - s ) - array ); \
                    stack[ top ].count = s; \
                } \
			} \
		} \
	}


#endif /* pixie_h */


/*
----------------------
    IMPLEMENTATION
----------------------
*/

#ifdef PIXIE_IMPLEMENTATION
#undef PIXIE_IMPLEMENTATION


#define _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS

/*
---------------------
    MATH WRAPPERS
---------------------
*/
#if !defined( PIXIE_NO_MATH ) || defined( __TINYC__ )
    #if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
        // Forward declare internal math funtions for use in the implementation of Pixie itself
        float internal_pixie_acos( float x );
        float internal_pixie_asin( float x );
        float internal_pixie_atan( float x );
        float internal_pixie_atan2( float x, float y );
        float internal_pixie_ceil( float x );
        float internal_pixie_cos( float x );
        float internal_pixie_cosh( float x );
        float internal_pixie_exp( float x );
        float internal_pixie_fabs( float x );
        float internal_pixie_floor( float x );
        float internal_pixie_fmod( float x, float y );
        float internal_pixie_log( float x );
        float internal_pixie_log10( float x );
        float internal_pixie_modf( float x, float* y );
        float internal_pixie_pow( float x, float y );
        float internal_pixie_sqrt( float x );
        float internal_pixie_sin( float x );
        float internal_pixie_sinh( float x );
        float internal_pixie_tan( float x );
        float internal_pixie_tanh( float x );
    #else /* defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE ) */
        // Undefine the math macros so they won't conflict with symbols in <math.h>, if it is included by any file
        #undef acos 
        #undef asin 
        #undef atan 
        #undef atan2
        #undef ceil 
        #undef cos  
        #undef cosh 
        #undef exp  
        #undef fabs 
        #undef floor
        #undef fmod 
        #undef log  
        #undef log10
        #undef modf 
        #undef pow  
        #undef sqrt 
        #undef sin  
        #undef sinh 
        #undef tan  
        #undef tanh 
    #endif /* defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE ) */
#endif /* PIXIE_NO_MATH */


// C runtime includes
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


// Libraries includes
#include "app.h"
#include "crtemu.h"
#include "crt_frame.h"
#include "ease.h"
#include "frametimer.h"
#define MID_ENABLE_RAW
#include "mid.h"
#include "mmap.h"
#include "palrle.h"
#define PIXELFONT_FUNC_NAME pixelfont_blit_u8
#include "pixelfont.h"
#undef PIXELFONT_FUNC_NAME 
#include "pixie_data.h"
#include "stb_image.h"
#include "thread.h"
#include "tsf.h"


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





// Forward declares

typedef struct pixie_t pixie_t;
static void pixie_force_exit( pixie_t* pixie );
static void pixie_update_input( pixie_t* pixie, app_input_event_t* events, int count );
static u32* pixie_render_screen( pixie_t* pixie, int* width, int* height, int* fullscreen, int* crt_mode );
static void pixie_render_samples( pixie_t* pixie, i16* sample_pairs, int sample_pairs_count );
static void const* pixie_find_asset( pixie_t* pixie, int id, int* size );


/*
------------------
    APP THREAD
------------------
*/

// Audio playback is started by the app thread, and works with a streaming sound buffer. Every time the stream have 
// played enough to need more samples, it calls this callback function, which just pass the call on to the
// `pixie_render_samples` which renders all currently playing sounds and mix all samples together for the sound buffer.

void app_sound_callback( APP_S16* sample_pairs, int sample_pairs_count, void* user_data ) {
    pixie_t* pixie = (pixie_t*) user_data;
    pixie_render_samples( pixie, sample_pairs, sample_pairs_count ); 
}


// Holds the data needed for communicating between user thread and app thread. Most of this will be in pixie, but there
// are also a few things which only needs to be accessed from within the `run` function and the app thread.

typedef struct app_context_t {
    pixie_t* pixie; // Main engine state
    int sound_buffer_size; // The `run` function defines how big a sound buffer to use, and that value is passed here
    thread_atomic_int_t exit_flag; // Set to 1 by `run` function on user thread to indicate app thread should terminate
    thread_signal_t init_complete; // Raised by app thread before entering main loop, to indicate `run` may continue
} app_context_t;


// Main body for the app thread (invoked by calling `app_run` in the `app_thread` entry point, invoked from `run`).
// The app thread runs independently from the user thread, and handles the main window, rendering, audio and input.
// After all initialization, and just before entering the main loop, it will raise the `init_complete` signal, which 
// lets the user thread (in the `run` function) know that it is safe to call the users entry point.
// Every iteration through the main loop, the signal `vbl.signal` is raised (as part of pixie_render_screen), and the 
// `vbl.count` value is incremented. These are used by the `wait_vbl` function to pause the user thread until the start 
// of the next frame. If the main window is closed (by clicking on the close button), the app thread sets the 
// `force_exit` value to `INT_MAX`, to signal to the user thread that it should exit the user code and terminate. The 
// `force_exit` value is checked in the `pixie_instance` function, which is called at the start of every API call.

static int app_proc( app_t* app, void* user_data ) {
    app_context_t* context = (app_context_t*) user_data;
    pixie_t* pixie = context->pixie;

    int fullscreen = 1;
    int crt_mode = 1;
    
    // Set up initial app parameters
    app_screenmode( app, fullscreen ? APP_SCREENMODE_FULLSCREEN : APP_SCREENMODE_WINDOW );
    app_interpolation( app, APP_INTERPOLATION_NONE );

    // Create and set up the CRT emulation instance
    crtemu_t* crtemu = crtemu_create( NULL );
    CRTEMU_U64 crt_time_us = 0;
    CRT_FRAME_U32* frame = (CRT_FRAME_U32*) malloc( sizeof( CRT_FRAME_U32 ) * CRT_FRAME_WIDTH * CRT_FRAME_HEIGHT );
    crt_frame( frame );
    crtemu_frame( crtemu, frame, CRT_FRAME_WIDTH, CRT_FRAME_HEIGHT );
    free( frame );

    // Start sound playback
    app_sound( app, context->sound_buffer_size * 2, app_sound_callback, pixie );

    // Create the frametimer instance, and set it to fixed 60hz update. This will ensure we never run faster than that,
    // even if the user have disabled vsync in their graphics card settings.
    frametimer_t* frametimer = frametimer_create( NULL );
    frametimer_lock_rate( frametimer, 60 );

    // Signal to the `run` function on the user thread that app initialization is complete, and it can start running
    thread_signal_raise( &context->init_complete );

    // Main loop
    APP_U64 prev_time = app_time_count( app );       
    while( !thread_atomic_int_load( &context->exit_flag ) ) {
        // Run app update (reading inputs etc)
        app_state_t app_state = app_yield( app );
        
        app_input_t input = app_input( app );
        pixie_update_input( pixie, input.events, input.count );

        // Check if the close button on the window was clicked (or Alt+F4 was pressed)
        if( app_state == APP_STATE_EXIT_REQUESTED ) {
            // Signal that we need to force the user thread to exit
            pixie_force_exit( pixie );
            break; 
        }

        // Render screen buffer
        int pixie_fullscreen = fullscreen;
        int pixie_crt_mode = crt_mode;
        int screen_width = 0;
        int screen_height = 0;
        APP_U32* xbgr = pixie_render_screen( pixie, &screen_width, &screen_height, &pixie_fullscreen, &pixie_crt_mode );
    
        if( pixie_fullscreen != fullscreen ) {
            fullscreen = pixie_fullscreen;
            app_screenmode( app, fullscreen ? APP_SCREENMODE_FULLSCREEN : APP_SCREENMODE_WINDOW );
        }

        if( pixie_crt_mode != crt_mode ) {
            crt_mode = pixie_crt_mode;
        }


        // Present the screen buffer to the window
        APP_U64 time = app_time_count( app );
        APP_U64 delta_time_us = ( time - prev_time ) / ( app_time_freq( app ) / 1000000 );
        prev_time = time;
        crt_time_us += delta_time_us;
        if( crt_mode ) {
            crtemu_present( crtemu, crt_time_us, xbgr, screen_width, screen_height, 0xffffff, 0x101010 );
            app_present( app, NULL, 1, 1, 0xffffff, 0x000000 );
        } else {
            app_present( app, xbgr, screen_width, screen_height, 0xffffff, 0x000000 );
        }

        // Ensure we don't run faster than 60 frames per second
        frametimer_update( frametimer );
    }

    // Stop sound playback
    app_sound( app, 0, NULL, NULL );

    frametimer_destroy( frametimer );
    crtemu_destroy( crtemu );
    return 0;
}


// Entry point for the app thread, which is started from the `run` function. Just runs the app with the above `app_proc` 

static int app_thread( void* user_data ) {
    return app_run( app_proc, user_data, NULL, NULL, NULL );
}


/*
-----------------------
    PIXIE INTERNALS
-----------------------
*/


// Main engine state - *everything* is stored here, and data is accessed from both the app thread and the user thread,
// with various mutexes being used to limit concurrent access where necessary. The instance is created within the `run` 
// function, and a pointer to it is stored in thread local storage for the user thread, so that every API method can 
// access it to perform its function. The app thread gets a pointer to it through the user_data parameter to the 
// app_proc.

typedef enum sprite_type_t { TYPE_NONE, TYPE_SPRITE, TYPE_LABEL, } sprite_type_t;

typedef struct pixie_t {
    // Controls the exit of the program, both via the `end` call and the window being closed
    struct {
        jmp_buf exit_jump; // Jump target set in `run` function, to jump back to when `end` is called
        thread_atomic_int_t force_exit; // Signals (when set to `INT_MAX`) that user thread should jump to `exit_jump`
    } exit;

    // Emulates the "vertical blank" event of screens. Every iteration through the render loop on the app thread, the
    // vbl.count value will be incremented and the vbl.signal raised. Used by the `wait_vbl` function.
    struct {
        thread_signal_t signal; // Raised by app thread when a frame is finished and the next frame is starting
        thread_atomic_int_t count; // Incremented for every new frame
    } vbl;

    struct {
        int fullscreen;
        int crt_mode;
    } window;

    #ifndef PIXIE_NO_BUILD
        struct {
            int count;
            struct {
                char name[ 64 ];
                asset_build_function_t func;
            } types[ 256 ];
        } build;
    #endif

    // Assets are loaded through the use of a memory mapped file, mapping to an asset bundle file. The file contains
    // all assets of the game in a ready-to-use format, so they can be used directly from the memory mapping. There is
    // no load operation done, that will be handles by the OS as the data is referenced.
    // TODO: background thread which touch all parts of the mapped memory to preload all data
    struct {
        mmap_t* bundle; // Memory mapped file containing all assets
        int count; // Total number of assets
        struct {
            int id; // The id as given in the enum defined by the user through the ASSET_... macros
            int offset; // Offset, in bytes from the start of the bundle, to this asset
            int size; // Size, in bytes, of the asset
        }* assets; // Index of all assets in the bundle
    } assets;

    struct { 
        thread_mutex_t mutex;
        u32 palette[ 256 ];
        u8* pixels;
        u8* composite;
        u32* xbgr;
        int screen_width;
        int screen_height;
        int border_width;
        int border_height;
    } screen;

    struct {
        int state[ KEYCOUNT ];
        int prev[ KEYCOUNT ];
    } keyboard;

    struct {
        thread_mutex_t mutex;
        int sprite_count;
        struct
            {
            int x;
            int y;
            int origin_x;
            int origin_y;
            int visible;

            sprite_type_t type;
            
            union {
                struct {
                    int asset;
                    int cel;
                } sprite;

                struct {
                    int font;
                    char* text;
		            text_align_t align;
		            int color;
		            int outline;
		            int shadow;
                    int wrap;
                } label;
            } data;

            struct {
                int count;
                move_t moves[ 16 ];
                int index;
                int time;
                int start;
                } move_x, move_y;
            }* sprites;

    } sprites;


    struct {
        int sound_buffer_size;
        i16* mix_buffers;

        thread_mutex_t song_mutex;
        tsf* sound_font;
        struct mid_t current_song;
    } audio;
} pixie_t;


// A global atomic pointer to the TLS instance for storing per-thread `pixie_t` pointers. Created in the `run` function
// unless it has already been created (through a compare-and-swap). The `run` method then sets the TLS value on the 
// instance, for the current thread.

static thread_atomic_ptr_t g_tls_pixie = { NULL }; 


// Retrieves the pointer to the `pixie_t` state for the current thread, as stored in the `g_tls_pixie` TLS storage
// As this has to be called in every API function, it was also a convenient place to check if the app thread is 
// requesting a shutdown of the program, and in that case do a forced exit (using `longjmp`) no matter how deep down a
// call stack `pixie_instance` was called from.

static pixie_t* pixie_instance( void ) { 
    // Get the `pixie_t` pointer for this thread from the global TLS instance `g_tls_pixie`
    pixie_t*  pixie = (pixie_t*) thread_tls_get( thread_atomic_ptr_load( &g_tls_pixie ) );

    // Check if app thread is requesting a forced exit (the user closed the window) and if so, call the exit point
    int force_exit = thread_atomic_int_load( &pixie->exit.force_exit );
    if( force_exit ) longjmp( pixie->exit.exit_jump, force_exit );

    return pixie; 
}


// Create the instance for holding the main engine state. Called from `run` before app thread is started.

static pixie_t* pixie_create( int sound_buffer_size ) {
    // Allocate the state and clear it, to avoid uninitialized varible problems
    pixie_t* pixie = (pixie_t*) malloc( sizeof( pixie_t ) );
    memset( pixie, 0, sizeof( *pixie ) );

    // Set up `exit` field. The `exit_jump` field is initialized from the `run` function at the desired point
    thread_atomic_int_store( &pixie->exit.force_exit, 0 ); 

    // Set up `vbl` field
    thread_signal_init( &pixie->vbl.signal );
    thread_atomic_int_store( &pixie->vbl.count, 0 );


    pixie->window.fullscreen = 1;
    pixie->window.crt_mode = 1;


    // Set up the screen 
    memcpy( pixie->screen.palette, default_palette(), sizeof( pixie->screen.palette ) );
    thread_mutex_init( &pixie->screen.mutex );
    pixie->screen.screen_width = 320;
    pixie->screen.screen_height = 200;
    pixie->screen.border_width = 32;
    pixie->screen.border_height = 44;
    int full_width = pixie->screen.screen_width + pixie->screen.border_width * 2;
    int full_height = pixie->screen.screen_height + pixie->screen.border_height * 2;
    pixie->screen.pixels = (u8*) malloc( sizeof( u8 ) * pixie->screen.screen_width * pixie->screen.screen_height );
    memset( pixie->screen.pixels, 0, sizeof( u8 ) * pixie->screen.screen_width * pixie->screen.screen_height );
    pixie->screen.composite = (u8*) malloc( sizeof( u8 ) * pixie->screen.screen_width * pixie->screen.screen_height );
    memset( pixie->screen.composite, 0, sizeof( u8 ) * pixie->screen.screen_width * pixie->screen.screen_height );
    pixie->screen.xbgr = (u32*) malloc( sizeof( u32 ) * full_width * full_height );
    memset( pixie->screen.xbgr, 0, sizeof( u32 ) * full_width * full_height );

    // Set up sprites
    thread_mutex_init( &pixie->sprites.mutex );
    pixie->sprites.sprite_count = 256;
    pixie->sprites.sprites = VOID_CAST( malloc( sizeof( *pixie->sprites.sprites ) * pixie->sprites.sprite_count ) );
    memset( pixie->sprites.sprites, 0, sizeof( *pixie->sprites.sprites ) * pixie->sprites.sprite_count );

    // Set up audio
    pixie->audio.sound_buffer_size = sound_buffer_size ;
    pixie->audio.mix_buffers = (i16*) malloc( sizeof( i16 ) * sound_buffer_size * 2 * 6 ); // 6 buffers (song, speech + 4 sounds)
    thread_mutex_init( &pixie->audio.song_mutex );

    int soundfont_size = 0;
    u8 const* soundfont = default_soundfont( &soundfont_size );
    pixie->audio.sound_font = tsf_load_memory( soundfont, soundfont_size );
    tsf_channel_set_bank_preset( pixie->audio.sound_font, 9, 128, 0);
    tsf_set_output( pixie->audio.sound_font, TSF_STEREO_INTERLEAVED, 44100, 0.0f );

    return pixie;
}


// Destroy the specified pixie instance. Called by `run` function, after app thread has finished.

static void pixie_destroy( pixie_t* pixie ) {
    // Cleanup `vbl` field
    thread_signal_term( &pixie->vbl.signal );

    // Cleanup screen
    free( pixie->screen.xbgr );
    free( pixie->screen.composite );
    free( pixie->screen.pixels );
    thread_mutex_term( &pixie->screen.mutex );

    // Cleanup sprites
    for( int i = 0; i < pixie->sprites.sprite_count; ++i ) {
        if( pixie->sprites.sprites[ i ].type == TYPE_LABEL ) {
            if( pixie->sprites.sprites[ i ].data.label.text ) {
                free( pixie->sprites.sprites[ i ].data.label.text );
            }
        }
    }
    free( pixie->sprites.sprites );
    thread_mutex_term( &pixie->sprites.mutex );

    // Cleanup audio
    thread_mutex_term( &pixie->audio.song_mutex );
    free( pixie->audio.mix_buffers );
    tsf_close( pixie->audio.sound_font );

    mmap_close( pixie->assets.bundle );


    free( pixie );
}


// When the window is closed, this is called (from app thread) to signal that the program should exit. The `force_exit`
// flag is checked in the `pixie_instance` function, which is called from every public API function.

static void pixie_force_exit( pixie_t* pixie ) {
    thread_atomic_int_store( &pixie->exit.force_exit, INT_MAX ); // INT_MAX is used to signal forced exit
            
    // In case user thread is in `wait_vbl` loop, exit it immediately so we don't have to wait for it to timeout
    thread_atomic_int_inc( &pixie->vbl.count );
    thread_signal_raise( &pixie->vbl.signal );    
}


static key_t pixie_key_from_app_key( app_key_t key ) {
	int index = (int) key;
	if( key < 0 || key >= APP_KEYCOUNT ) return KEY_INVALID;
	
	int map[ APP_KEYCOUNT * 2 ] = { 
		APP_KEY_INVALID, KEY_INVALID, APP_KEY_LBUTTON, KEY_LBUTTON, APP_KEY_RBUTTON, KEY_RBUTTON, APP_KEY_CANCEL, KEY_CANCEL, 
		APP_KEY_MBUTTON, KEY_MBUTTON, APP_KEY_XBUTTON1, KEY_XBUTTON1, APP_KEY_XBUTTON2, KEY_XBUTTON2, APP_KEY_BACK, KEY_BACK, 
		APP_KEY_TAB, KEY_TAB, APP_KEY_CLEAR, KEY_CLEAR, APP_KEY_RETURN, KEY_RETURN, APP_KEY_SHIFT, KEY_SHIFT, APP_KEY_CONTROL, 
		KEY_CONTROL, APP_KEY_MENU, KEY_MENU, APP_KEY_PAUSE, KEY_PAUSE, APP_KEY_CAPITAL, KEY_CAPITAL, APP_KEY_KANA, KEY_KANA, 
		APP_KEY_JUNJA, KEY_JUNJA, APP_KEY_FINAL, KEY_FINAL, APP_KEY_HANJA, KEY_HANJA, APP_KEY_ESCAPE, KEY_ESCAPE, APP_KEY_CONVERT, 
		KEY_CONVERT, APP_KEY_NONCONVERT, KEY_NONCONVERT, APP_KEY_ACCEPT, KEY_ACCEPT, APP_KEY_MODECHANGE, KEY_MODECHANGE, APP_KEY_SPACE, 
		KEY_SPACE, APP_KEY_PRIOR, KEY_PRIOR, APP_KEY_NEXT, KEY_NEXT, APP_KEY_END, KEY_END, APP_KEY_HOME, KEY_HOME, APP_KEY_LEFT, 
		KEY_LEFT, APP_KEY_UP, KEY_UP, APP_KEY_RIGHT, KEY_RIGHT, APP_KEY_DOWN, KEY_DOWN, APP_KEY_SELECT, KEY_SELECT, APP_KEY_PRINT, 
		KEY_PRINT, APP_KEY_EXEC, KEY_EXEC, APP_KEY_SNAPSHOT, KEY_SNAPSHOT, APP_KEY_INSERT, KEY_INSERT,
		APP_KEY_DELETE, KEY_DELETE, APP_KEY_HELP, KEY_HELP, APP_KEY_0, KEY_0, APP_KEY_1, KEY_1, APP_KEY_2, KEY_2, APP_KEY_3, KEY_3, 
		APP_KEY_4, KEY_4, APP_KEY_5, KEY_5, APP_KEY_6, KEY_6, APP_KEY_7, KEY_7, APP_KEY_8, KEY_8, APP_KEY_9, KEY_9, APP_KEY_A, KEY_A, 
		APP_KEY_B, KEY_B, APP_KEY_C, KEY_C, APP_KEY_D, KEY_D, APP_KEY_E, KEY_E, APP_KEY_F, KEY_F, APP_KEY_G, KEY_G, APP_KEY_H, KEY_H,
		APP_KEY_I, KEY_I, APP_KEY_J, KEY_J, APP_KEY_K, KEY_K, APP_KEY_L, KEY_L, APP_KEY_M, KEY_M, APP_KEY_N, KEY_N, APP_KEY_O, KEY_O,
		APP_KEY_P, KEY_P, APP_KEY_Q, KEY_Q, APP_KEY_R, KEY_R, APP_KEY_S, KEY_S, APP_KEY_T, KEY_T, APP_KEY_U, KEY_U, APP_KEY_V, KEY_V,
		APP_KEY_W, KEY_W, APP_KEY_X, KEY_X, APP_KEY_Y, KEY_Y, APP_KEY_Z, KEY_Z, APP_KEY_LWIN, KEY_LWIN, APP_KEY_RWIN, KEY_RWIN, 
		APP_KEY_APPS, KEY_APPS, APP_KEY_SLEEP, KEY_SLEEP, APP_KEY_NUMPAD0, KEY_NUMPAD0, APP_KEY_NUMPAD1, KEY_NUMPAD1, APP_KEY_NUMPAD2, 
		KEY_NUMPAD2, APP_KEY_NUMPAD3, KEY_NUMPAD3, APP_KEY_NUMPAD4, KEY_NUMPAD4, APP_KEY_NUMPAD5, KEY_NUMPAD5, APP_KEY_NUMPAD6, 
		KEY_NUMPAD6, APP_KEY_NUMPAD7, KEY_NUMPAD7, APP_KEY_NUMPAD8, KEY_NUMPAD8, APP_KEY_NUMPAD9, KEY_NUMPAD9, APP_KEY_MULTIPLY, 
		KEY_MULTIPLY, APP_KEY_ADD, KEY_ADD, APP_KEY_SEPARATOR, KEY_SEPARATOR, APP_KEY_SUBTRACT, KEY_SUBTRACT, APP_KEY_DECIMAL, 
		KEY_DECIMAL, APP_KEY_DIVIDE, KEY_DIVIDE, APP_KEY_F1, KEY_F1, APP_KEY_F2, KEY_F2, APP_KEY_F3, KEY_F3, APP_KEY_F4, KEY_F4, 
		APP_KEY_F5, KEY_F5, APP_KEY_F6, KEY_F6, APP_KEY_F7, KEY_F7, APP_KEY_F8, KEY_F8, APP_KEY_F9, KEY_F9, APP_KEY_F10, KEY_F10, 
		APP_KEY_F11, KEY_F11, APP_KEY_F12, KEY_F12, APP_KEY_F13, KEY_F13, APP_KEY_F14, KEY_F14, APP_KEY_F15, KEY_F15, APP_KEY_F16, 
		KEY_F16, APP_KEY_F17, KEY_F17, APP_KEY_F18, KEY_F18, APP_KEY_F19, KEY_F19, APP_KEY_F20, KEY_F20, APP_KEY_F21, KEY_F21, 
		APP_KEY_F22, KEY_F22, APP_KEY_F23, KEY_F23, APP_KEY_F24, KEY_F24, APP_KEY_NUMLOCK, KEY_NUMLOCK, APP_KEY_SCROLL, KEY_SCROLL,
		APP_KEY_LSHIFT, KEY_LSHIFT, APP_KEY_RSHIFT, KEY_RSHIFT, APP_KEY_LCONTROL, KEY_LCONTROL, APP_KEY_RCONTROL, KEY_RCONTROL,
		APP_KEY_LMENU, KEY_LMENU, APP_KEY_RMENU, KEY_RMENU, APP_KEY_BROWSER_BACK, KEY_BROWSER_BACK, APP_KEY_BROWSER_FORWARD, 
		KEY_BROWSER_FORWARD, APP_KEY_BROWSER_REFRESH, KEY_BROWSER_REFRESH, APP_KEY_BROWSER_STOP, KEY_BROWSER_STOP, 
		APP_KEY_BROWSER_SEARCH, KEY_BROWSER_SEARCH, APP_KEY_BROWSER_FAVORITES, KEY_BROWSER_FAVORITES, APP_KEY_BROWSER_HOME, 
		KEY_BROWSER_HOME, APP_KEY_VOLUME_MUTE, KEY_VOLUME_MUTE, APP_KEY_VOLUME_DOWN, KEY_VOLUME_DOWN, APP_KEY_VOLUME_UP, KEY_VOLUME_UP, 
		APP_KEY_MEDIA_NEXT_TRACK, KEY_MEDIA_NEXT_TRACK, APP_KEY_MEDIA_PREV_TRACK, KEY_MEDIA_PREV_TRACK, APP_KEY_MEDIA_STOP, 
		KEY_MEDIA_STOP, APP_KEY_MEDIA_PLAY_PAUSE, KEY_MEDIA_PLAY_PAUSE, APP_KEY_LAUNCH_MAIL, KEY_LAUNCH_MAIL, 
		APP_KEY_LAUNCH_MEDIA_SELECT, KEY_LAUNCH_MEDIA_SELECT, APP_KEY_LAUNCH_APP1, KEY_LAUNCH_APP1, APP_KEY_LAUNCH_APP2, 
		KEY_LAUNCH_APP2, APP_KEY_OEM_1, KEY_OEM_1, APP_KEY_OEM_PLUS, KEY_OEM_PLUS, APP_KEY_OEM_COMMA, KEY_OEM_COMMA, APP_KEY_OEM_MINUS, 
		KEY_OEM_MINUS, APP_KEY_OEM_PERIOD, KEY_OEM_PERIOD, APP_KEY_OEM_2, KEY_OEM_2, APP_KEY_OEM_3, KEY_OEM_3, APP_KEY_OEM_4, KEY_OEM_4,
		APP_KEY_OEM_5, KEY_OEM_5, APP_KEY_OEM_6, KEY_OEM_6, APP_KEY_OEM_7, KEY_OEM_7, APP_KEY_OEM_8, KEY_OEM_8, APP_KEY_OEM_102, 
		KEY_OEM_102, APP_KEY_PROCESSKEY, KEY_PROCESSKEY, APP_KEY_ATTN, KEY_ATTN, APP_KEY_CRSEL, KEY_CRSEL, APP_KEY_EXSEL, KEY_EXSEL,
		APP_KEY_EREOF, KEY_EREOF, APP_KEY_PLAY, KEY_PLAY, APP_KEY_ZOOM, KEY_ZOOM, APP_KEY_NONAME, KEY_NONAME, APP_KEY_PA1, KEY_PA1, 
		APP_KEY_OEM_CLEAR, KEY_OEM_CLEAR, 
    };

	ASSERT( map[ index * 2 ] == key, "Invalid mapping from app_key to pixie key" );
	if( map[ index * 2 ] != key ) return KEY_INVALID;

	return (key_t) map[ index * 2 + 1 ];
}


static void pixie_update_input( pixie_t* pixie, app_input_event_t* events, int count ) {
	ASSERT( sizeof( pixie->keyboard.state ) / sizeof( *pixie->keyboard.state ) == sizeof( pixie->keyboard.prev ) / sizeof( *pixie->keyboard.prev ), "Key states size mismatch" );
	for( int i = 0; i < sizeof( pixie->keyboard.state ) / sizeof( *pixie->keyboard.state ); ++i )
		pixie->keyboard.prev[ i ] = pixie->keyboard.state[ i ];

	for( int i = 0; i < count; ++i ) {
		app_input_event_t* event = &events[ i ];
		switch( event->type ) {
			case APP_INPUT_KEY_DOWN: {
				key_t key = pixie_key_from_app_key( event->data.key );
				if( key >= 0 && key < sizeof( pixie->keyboard.state ) / sizeof( *pixie->keyboard.state ) )
					pixie->keyboard.state[ key ] = 1;                          
			} break;
			case APP_INPUT_KEY_UP: {
				key_t key = pixie_key_from_app_key( event->data.key );
				if( key >= 0 && key < sizeof( pixie->keyboard.state ) / sizeof( *pixie->keyboard.state ) )
					pixie->keyboard.state[ key ] = 0;                          
			} break;
			case APP_INPUT_CHAR: {
            } break;
			case APP_INPUT_MOUSE_MOVE: {
            } break;
			case APP_INPUT_MOUSE_DELTA: {
            } break;
			case APP_INPUT_SCROLL_WHEEL: {
			} break;
			case APP_INPUT_TABLET: {
            } break;

			case APP_INPUT_DOUBLE_CLICK: {
				break;
            }
		}
	}
}


// Render all sprites and convert the screen from palettized to 24-bit XBGR

static u32* pixie_render_screen( pixie_t* pixie, int* width, int* height, int* fullscreen, int* crt_mode )
    {
    u32 palette[ 256 ]; // Temporary local copy of palette to reduce scope of mutex

    // Make a copy of the screen so we can draw sprites on top of it, and copy the palette and width/height as well
    thread_mutex_lock( &pixie->screen.mutex ); // `screen` and `palette` fields are shared, so must protect access
    int screen_width = pixie->screen.screen_width;
    int screen_height = pixie->screen.screen_height;
    int border_width = pixie->screen.border_width;
    int border_height = pixie->screen.border_height;
    int full_width = screen_width + border_width * 2;
    int full_height = screen_height + border_height * 2;
    
    memcpy( pixie->screen.composite, pixie->screen.pixels, sizeof( u8 ) * screen_width * screen_height );
    memcpy( palette, pixie->screen.palette, sizeof( palette ) );
    thread_mutex_unlock( &pixie->screen.mutex );

    if( fullscreen ) *fullscreen = pixie->window.fullscreen;
    if( crt_mode ) *crt_mode = pixie->window.crt_mode;

    float (*easefuncs[])( float ) = { 
        NULL, ease_linear, ease_smoothstep, ease_smootherstep, ease_out_quad, ease_out_back, ease_out_bounce, 
        ease_out_sine, ease_out_elastic, ease_out_expo, ease_out_cubic, ease_out_quart, ease_out_quint, ease_out_circle,
        ease_in_quad, ease_in_back, ease_in_bounce, ease_in_sine, ease_in_elastic, ease_in_expo, ease_in_cubic,
        ease_in_quart, ease_in_quint, ease_in_circle, ease_in_out_quad, ease_in_out_back, ease_in_out_bounce, 
        ease_in_out_sine, ease_in_out_elastic, ease_in_out_expo, ease_in_out_cubic, ease_in_out_quart, 
        ease_in_out_quint, ease_in_out_circle,
    };

    // Process sprites
    thread_mutex_lock( &pixie->sprites.mutex ); /* `sprites` data is shared. `screen.composite` is not, it is a temp
        buffer accessed only here. `assets` is immutable after startup, so don't need to be protected. */

    for( int i = 0; i < pixie->sprites.sprite_count; ++i ) {    
        // Update sprite movement
        if( pixie->sprites.sprites[ i ].move_y.count > 0 ) {
            ++pixie->sprites.sprites[ i ].move_y.time;
            move_t* move = &pixie->sprites.sprites[ i ].move_y.moves[ pixie->sprites.sprites[ i ].move_y.index ];
            if( pixie->sprites.sprites[ i ].move_y.time <= move->duration ) {
                float range = (float)( move->target - pixie->sprites.sprites[ i ].move_y.start );
                float current = ( (float) pixie->sprites.sprites[ i ].move_y.time ) / (float) move->duration;
                if( move->type != MOVE_DELAY ) {
                    float t = easefuncs[ move->type ]( current );
                    int pos = pixie->sprites.sprites[ i ].move_y.start + (int)( t * range );
                    pixie->sprites.sprites[ i ].y = pos;
                }
            } else {
                ++pixie->sprites.sprites[ i ].move_y.index;
                if( pixie->sprites.sprites[ i ].move_y.index >= pixie->sprites.sprites[ i ].move_y.count ) {
                    pixie->sprites.sprites[ i ].move_y.count = 0;
                } else {
                    pixie->sprites.sprites[ i ].move_y.time = 0;
                    if( move->type != MOVE_DELAY ) {
                        pixie->sprites.sprites[ i ].y = move->target;
                    }
                    pixie->sprites.sprites[ i ].move_y.start = pixie->sprites.sprites[ i ].y;
                }
            }
        }
        if( pixie->sprites.sprites[ i ].move_x.count > 0 ) {
            ++pixie->sprites.sprites[ i ].move_x.time;
            move_t* move = &pixie->sprites.sprites[ i ].move_x.moves[ pixie->sprites.sprites[ i ].move_x.index ];
            if( pixie->sprites.sprites[ i ].move_x.time <= move->duration ) {
                float range = (float)( move->target - pixie->sprites.sprites[ i ].move_x.start );
                float current = ( (float) pixie->sprites.sprites[ i ].move_x.time ) / (float) move->duration;
                if( move->type != MOVE_DELAY ) {
                    float t = easefuncs[ move->type ]( current );
                    int pos = pixie->sprites.sprites[ i ].move_x.start + (int)( t * range );
                    pixie->sprites.sprites[ i ].x = pos;
                }
            } else {
                ++pixie->sprites.sprites[ i ].move_x.index;
                if( pixie->sprites.sprites[ i ].move_x.index >= pixie->sprites.sprites[ i ].move_x.count ) {
                    pixie->sprites.sprites[ i ].move_x.count = 0;
                } else {
                    pixie->sprites.sprites[ i ].move_x.time = 0;
                    if( move->type != MOVE_DELAY ) {
                        pixie->sprites.sprites[ i ].x = move->target;
                    }
                    pixie->sprites.sprites[ i ].move_x.start = pixie->sprites.sprites[ i ].x;
                }
            }
        }

        // Render sprites
        
        if( !pixie->sprites.sprites[ i ].visible ) continue;

        if( pixie->sprites.sprites[ i ].type == TYPE_SPRITE ) {
            int asset = pixie->sprites.sprites[ i ].data.sprite.asset;
            if( asset < 1 || asset > pixie->assets.count ) continue;
            --asset;
    
            int cel = pixie->sprites.sprites[ i ].data.sprite.cel;
            // Find the sprite data in the memory mapped file
            u8* frames = (u8*) pixie_find_asset( pixie, asset, NULL );
            int frame_count = *(int*)frames;
            if( frame_count > 0 && cel >= 0 ) {
                int* offsets = (int*)(frames + sizeof( int ) );
                palrle_data_t* data = (palrle_data_t*)( frames + offsets[ cel % frame_count ] );

                // Render pixels
                palrle_blit( data, pixie->sprites.sprites[ i ].x - pixie->sprites.sprites[ i ].origin_x, pixie->sprites.sprites[ i ].y  - pixie->sprites.sprites[ i ].origin_y, 
                    pixie->screen.composite, screen_width, screen_height );
            }
        // Render labels
        } else if( pixie->sprites.sprites[ i ].type == TYPE_LABEL ) {
            int asset = pixie->sprites.sprites[ i ].data.label.font;
            if( asset < 1 || asset > pixie->assets.count ) continue;
            --asset;

            // Find the font data in the memory mapped file
            pixelfont_t const* font = VOID_CAST( pixie_find_asset( pixie, asset, NULL ) );
            pixelfont_align_t pixelfont_align = PIXELFONT_ALIGN_LEFT;
            if( pixie->sprites.sprites[ i ].data.label.align == TEXT_ALIGN_CENTER ) {
                pixelfont_align = PIXELFONT_ALIGN_CENTER;
            } else if( pixie->sprites.sprites[ i ].data.label.align == TEXT_ALIGN_RIGHT ) {
                pixelfont_align = PIXELFONT_ALIGN_RIGHT;
            }

            int wrap = pixie->sprites.sprites[ i ].data.label.wrap;

            int outline = pixie->sprites.sprites[ i ].data.label.outline;

            int shadow = pixie->sprites.sprites[ i ].data.label.shadow;
            int shadow_offset_x = 1;
            int shadow_offset_y = 1;
            if( shadow >= 0 && shadow < 256 ) {
                if( outline >= 0 && outline < 256 ) {
			        for( int y = -1; y <= 1; ++y ) for( int x = -1; x <= 1; ++x ) {
				        if( x == 0 && y == 0 ) continue;

	                    pixelfont_blit_u8( font, pixie->sprites.sprites[ i ].x + shadow_offset_x + x - pixie->sprites.sprites[ i ].origin_x, 
                            pixie->sprites.sprites[ i ].y + shadow_offset_y + y - pixie->sprites.sprites[ i ].origin_y, 
                            pixie->sprites.sprites[ i ].data.label.text, (u8) shadow, 
                            pixie->screen.composite, screen_width, screen_height,
                            pixelfont_align, wrap, 0, 0, -1, PIXELFONT_BOLD_OFF, PIXELFONT_ITALIC_OFF, PIXELFONT_UNDERLINE_OFF, 
                            NULL );
                    }
                } else {
	                pixelfont_blit_u8( font, pixie->sprites.sprites[ i ].x + shadow_offset_x - pixie->sprites.sprites[ i ].origin_x, 
                        pixie->sprites.sprites[ i ].y + shadow_offset_y - pixie->sprites.sprites[ i ].origin_y, 
                        pixie->sprites.sprites[ i ].data.label.text, (u8) shadow, 
                        pixie->screen.composite, screen_width, screen_height,
                        pixelfont_align, wrap, 0, 0, -1, PIXELFONT_BOLD_OFF, PIXELFONT_ITALIC_OFF, PIXELFONT_UNDERLINE_OFF, 
                        NULL );
                }
            }

            if( outline >= 0 && outline < 256 ) {
			    for( int y = -1; y <= 1; ++y ) for( int x = -1; x <= 1; ++x ) {
				    if( x == 0 && y == 0 ) continue;

	                pixelfont_blit_u8( font, pixie->sprites.sprites[ i ].x + x - pixie->sprites.sprites[ i ].origin_x, pixie->sprites.sprites[ i ].y + y - pixie->sprites.sprites[ i ].origin_y, 
                        pixie->sprites.sprites[ i ].data.label.text, (u8) outline, 
                        pixie->screen.composite, screen_width, screen_height,
                        pixelfont_align, wrap, 0, 0, -1, PIXELFONT_BOLD_OFF, PIXELFONT_ITALIC_OFF, PIXELFONT_UNDERLINE_OFF, 
                        NULL );
                }
            }


            int color = pixie->sprites.sprites[ i ].data.label.color;
            if( color >= 0 && color < 256 ) {
	            pixelfont_blit_u8( font, pixie->sprites.sprites[ i ].x - pixie->sprites.sprites[ i ].origin_x, pixie->sprites.sprites[ i ].y - pixie->sprites.sprites[ i ].origin_y, 
                    pixie->sprites.sprites[ i ].data.label.text, (u8) color, 
                    pixie->screen.composite, screen_width, screen_height,
                    pixelfont_align, wrap, 0, 0, -1, PIXELFONT_BOLD_OFF, PIXELFONT_ITALIC_OFF, PIXELFONT_UNDERLINE_OFF, 
                    NULL );
            }

        }
    }
    thread_mutex_unlock( &pixie->sprites.mutex );

    // Signal to the game that the frame is completed, and that we are just starting the next one
    thread_atomic_int_inc( &pixie->vbl.count );
    thread_signal_raise( &pixie->vbl.signal );    

    // Convert palette based screen composite to 24-bit XBGR. Both `xbgr` and `composite` are only used from here
    for( int y = 0; y < screen_height; ++y )
        for( int x = 0; x < screen_width; ++x )
            pixie->screen.xbgr[ x + border_width + ( y + border_height ) * full_width ] = 
                palette[ pixie->screen.composite[ x + y * screen_width ] ];

    *width = full_width;
    *height = full_height;
    return pixie->screen.xbgr;
    }


// Called by audio thread (via app_sound_callback) when it needs new audio samples

static void pixie_render_samples( pixie_t* pixie, i16* sample_pairs, int sample_pairs_count )
    {
    // Render midi song to local buffer
    i16* song = pixie->audio.mix_buffers;
    thread_mutex_lock( &pixie->audio.song_mutex ); 
    if( !pixie->audio.current_song.song.event_count || !pixie->audio.current_song.song.events ) 
        memset( song, 0, sizeof( i16 ) * sample_pairs_count * 2 );
    else    
        mid_render_short( &pixie->audio.current_song, song, sample_pairs_count, pixie->audio.sound_font );
    thread_mutex_unlock( &pixie->audio.song_mutex );

    // Mix all local buffers
    for( int i = 0; i < sample_pairs_count * 2; ++i )
        {
        int sample = song[ i ];
        sample = sample > 32767 ? 32767 : sample < -32727 ? -32727 : sample;
        sample_pairs[ i ] = (i16) sample;
        }
    }


// Retrieves pointer to and size of the specified asset

static void const* pixie_find_asset( pixie_t* pixie, int id, int* size ) {
    if( id < 0 || id >= pixie->assets.count ) {
        if( size ) *size = 0;
        return NULL;
    }

    if( size ) *size = pixie->assets.assets[ id ].size;
    return (void*)( ( (uintptr_t) mmap_data( pixie->assets.bundle ) ) + pixie->assets.assets[ id ].offset );    
}


/*
----------------------------
    ASSET BUNDLE SUPPORT
----------------------------
*/

// Creates a memory mapping for the specified bundle file. If the other parameters are given, it will validate the
// file contents against them - this is the case when data builds are enabled (the default), so that the bundle can be
// rebuilt if the values don't match. If data builds are disabled (through the define PIXIE_NO_BUILD), this checking
// does not take place, and the bundle will be assumed to be valid. Undefined behaviour is to be expected if it is not.

int load_bundle( char const* filename, char const* time, char const* definitions, int count ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    // Get size of bundle file
    struct stat s;
    if( stat( filename, &s ) ) return EXIT_FAILURE;

    // Create memory mapping for the bundle file
    mmap_t* bundle = mmap_open_read_only( filename, (size_t) s.st_size );
    if( !bundle ) return EXIT_FAILURE;
    
    void* data = mmap_data( bundle );

    // Take a look at the header data, by just casting it to expected format 
    // TODO: this is currently duplicated from pixie_build.h. Is that fine or should it be a shared definition?
    struct
    {
        char file_id[ 20 ];
        int header_size;
        int assets_count;
        char bundle_file[ 256 ];
        char definitions_file[ 256 ];
        char build_time[ 64 ];
    }* header = VOID_CAST( data );

    // Check that the bundle is big enough to contain a full header, and that the header is the expected size.
    if( mmap_size( bundle ) < sizeof( *header ) || header->header_size != sizeof( *header ) ) {
        mmap_close( bundle );
        return EXIT_FAILURE;
    }

    // Verify the header data. If `definitions` or `time` are NULL, or `count` is negative, they are not checked against
    if( strcmp( header->file_id, "PIXIE_ASSETS_BUNDLE" ) != 0 || 
        strcmp( header->bundle_file, filename ) != 0 ||  
        ( definitions && strcmp( header->definitions_file, definitions ) != 0 ) ||  
        ( time && strcmp( header->build_time, time ) != 0 ) ||  
        ( count > 0 && header->assets_count != count ) ) {
            mmap_close( bundle );
            return EXIT_FAILURE;
    }

    // Check that the size of all files match the size of the bundle, and that IDs and offsets are as expected.
    struct { int id; int offset; int size; }* assets = VOID_CAST( (void*)( header + 1 ) );
    size_t total_size = sizeof( *header ) + sizeof( *assets ) * header->assets_count;
    int offset = (int) total_size;
    for( int i = 0; i < header->assets_count; ++i ) {
        if( assets[ i ].size < 0 || assets[ i ].offset != offset || assets[ i ].id != i ) {
            mmap_close( bundle );
            return EXIT_FAILURE;
        }
        offset += assets[ i ].size;
        total_size += assets[ i ].size;
    }

    if( total_size != (size_t) s.st_size ) {
        mmap_close( bundle );
        return EXIT_FAILURE;
    }


    pixie->assets.bundle = bundle;
    pixie->assets.count = header->assets_count;
    pixie->assets.assets = VOID_CAST( (void*) assets );

    return EXIT_SUCCESS;
}

/*
---------------------
    API FUNCTIONS
---------------------
*/


// The main starting point for a Pixie program. Called automatically from the standard C main function defined below,
// unless `PIXIE_NO_MAIN` has been defined, in which case the user will have to define their own `main` function and
// call `run` from it, passing in their pixie main function to it. The `run` function creates the `pixie_t` engine state
// and stores a pointer to it in thread local storage. It creates the app thread, and waits for it to be initialized
// before calling the users pixie main function (called `pixmain` if the built-in main is used). It also uses `setjmp`
// to define a jump point in order to be able to exit from the user code no matter where in the call stack it is, and 
// still get back to the `run` function to perform a controlled shutdown.

int run( int (*main)( int, char** ) ) {
    int const SOUND_BUFFER_SIZE = 735 * 3; // Three frames worth of sound buffering

    // Create and initialize the main engine state used by all of pixie
    pixie_t* pixie = pixie_create( SOUND_BUFFER_SIZE );

    // A pointer to the `pixie_t` instance needs to be stored in thread local storage, and before we do, we must create
    // the TLS instance. But only if it is not already created, and as there's nothing stopping a user from calling the
    // `run` function from two parallel threads at the same time, we must avoid a race condition. We do this by always
    // creating a new TLS instance, and then we do a compare-and-swap with the global instance, to only swap if it is 
    // not already set. If the swap does not return NULL (thus indicating it already held a value), then we destroy the
    // TLS instance we just created, as it will no longer be needed (we'll have to use the global one instead).
    thread_tls_t pixie_tls = thread_tls_create();
    if( thread_atomic_ptr_compare_and_swap( &g_tls_pixie, NULL, pixie_tls ) ) thread_tls_destroy( pixie_tls );
    
    // Store the `pixie_t` pointer in the thread local storage for the current thread. It will be retrieved by all API
    // functions so that we don't have to pass around an instance parameter to them.
    thread_tls_set( thread_atomic_ptr_load( &g_tls_pixie ), pixie );
    
    // Define the `app_context_t` instance which will be shared between `run` function and `app_proc` thread.
    app_context_t app_context = { NULL };
    app_context.pixie = pixie;
    app_context.sound_buffer_size = SOUND_BUFFER_SIZE;
    thread_atomic_int_store( &app_context.exit_flag, 0 );
    thread_signal_init( &app_context.init_complete );
    
    // Start the app thread. The entry point `app_thread` will just run `app_proc`
    thread_ptr_t thread = thread_create( app_thread, &app_context, NULL, THREAD_STACK_SIZE_DEFAULT );
    
    // Wait until the app thread have completed its initialization. If it takes more than 5 seconds, we just quit.
    int result = thread_signal_wait( &app_context.init_complete, 5000 ) ? EXIT_SUCCESS : EXIT_FAILURE;
    if( result == EXIT_SUCCESS ) {
        // Set up the jump target for the `end` function
        #pragma warning( push )
        #pragma warning( disable: 4611 ) // interaction between '_setjmp' and C++ object destruction is non-portable
        int jumpres = setjmp( pixie->exit.exit_jump );
        #pragma warning( pop )

        // Run the user provided entry point (will be `pixmain` unless `PIXIE_NO_MAIN` was defined)
        if( jumpres == 0 ) // First time we get here we call `main`. If `exit_jump` was jumped to we will get here again
            result = main( __argc, __argv );
        else // Second time we save the result (`INT_MAX` is mapped to 0, as a jumpres of 0 would call main again)
            result = ( result == INT_MAX ? EXIT_SUCCESS : result );
    }

    // Terminate the app thread
    thread_atomic_int_store( &app_context.exit_flag, 1 );
    thread_join( thread );
    thread_signal_term( &app_context.init_complete );
    
    // Destroy pixie instance, and clear the thread local pointer to it
    pixie_destroy( pixie );
    thread_tls_set( thread_atomic_ptr_load( &g_tls_pixie ), NULL );

    return result;
}


// Terminates the user code, performing a controlled shutdown of the engine. Can be called from deep down a call stack
// as it will use `longjmp` to branch back to the top level of the `run` function.

void end( int return_code ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage
   
    // Jump back into the middle of the `run` function, to where `setjmp` was called, regardless of where we called from
    longjmp( pixie->exit.exit_jump, return_code );
}


// Waits until the start of the next frame. 

void wait_vbl( void ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    // Get the vbl count before we start - we want to wait until it has changed
    int current_vbl_count = thread_atomic_int_load( &pixie->vbl.count );

    // Since signals might suffer spurious wakeups, we want to loop until we get a new vbl count
    while( current_vbl_count == thread_atomic_int_load( &pixie->vbl.count ) ) {
        // Wait until app thread says there is a new frame, or timeout after one second.
        thread_signal_wait( &pixie->vbl.signal, 1000 );

        // Call `pixie_instance` again, just to trigger the check for `force_exit`, so we can terminate if need be
        pixie_instance();
    }
}


void wait( int jiffys ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage
    (void) pixie;
    for( int i = 0; i < jiffys; ++i ) {
        wait_vbl();
    }
}


int fullscreen( void ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    return pixie->window.fullscreen;
}


void fullscreen_on( void ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    pixie->window.fullscreen = 1;
}



void fullscreen_off( void ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    pixie->window.fullscreen = 0;
}



int crt_mode( void ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    return pixie->window.crt_mode;
}



void crt_mode_on( void ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    pixie->window.crt_mode = 1;
}



void crt_mode_off( void ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    pixie->window.crt_mode = 0;
}



// Prints the specified string to the screen using the default font.

void print( char const* str ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    // Very placeholder font rendering
    thread_mutex_lock( &pixie->screen.mutex );
    static int x = 32;
    static int y = 44;
    while( *str ) {
        unsigned long long chr = default_font()[ (u8) *str++ ];
        for( int iy = 0; iy < 8; ++iy )
            for( int ix = 0; ix < 8; ++ix )
                if( chr & ( 1ull << ( ix + iy * 8 ) ) )
                    pixie->screen.pixels[ x + ix + ( y + iy ) * pixie->screen.screen_width ] = 10; 
        x += 8;
        if( x - 32 >= 320 ) {
            x = 32;
            y += 8;
        }
    }
    x = 32;
    y += 8;
    thread_mutex_unlock( &pixie->screen.mutex );
}


// Apply palette from file to the global palette

void load_palette( int asset ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    int size = 0;
    void const* data = pixie_find_asset( pixie, asset, &size );
    if( size == sizeof( pixie->screen.palette ) )
        memcpy( pixie->screen.palette, data, sizeof( pixie->screen.palette ) );
}


void setcol( int index, rgb_t rgb ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    if( index < 0 || index >= 256 ) return;
    u32 r = (u32)( rgb.r < 0 ? 0 : rgb.r > 255 ? 255 : rgb.r );
    u32 g = (u32)( rgb.g < 0 ? 0 : rgb.g > 255 ? 255 : rgb.g );
    u32 b = (u32)( rgb.b < 0 ? 0 : rgb.b > 255 ? 255 : rgb.b );
    pixie->screen.palette[ index ] = ( b << 16 ) | ( g << 8 ) | r;
}


rgb_t getcol( int index ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    if( index < 0 || index >= 256 ) {
        rgb_t rgb = { 0, 0, 0 };
        return rgb;
    }

    u32 color = pixie->screen.palette[ index ];
    rgb_t rgb = { (int)( color & 0xff ), (int)( ( color >> 8 ) & 0xff ), (int)( ( color >> 16 ) & 0xff ) };
    return rgb;
}


void sprites_off( void ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    for( int i = 0; i < pixie->sprites.sprite_count; ++i ) {
        pixie->sprites.sprites[ i ].move_x.count = 0;
        pixie->sprites.sprites[ i ].move_y.count = 0;
        if( pixie->sprites.sprites[ i ].type == TYPE_LABEL ) {
            if( pixie->sprites.sprites[ i ].data.label.text ) {
                free( pixie->sprites.sprites[ i ].data.label.text );
            }
        }
        pixie->sprites.sprites[ i ].type = TYPE_NONE;
    }

    thread_mutex_unlock( &pixie->sprites.mutex );
}


// Assign a bitmap to a sprite, and give it a position

int sprite( int spr_index, int x, int y, int asset ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );
    
    if( asset < 0 || asset >= pixie->assets.count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    // TODO: check that asset is sprite

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }
    
    --spr_index;
    if( pixie->sprites.sprites[ spr_index ].type == TYPE_LABEL ) {
        if( !pixie->sprites.sprites[ spr_index ].data.label.text ) {
            free( pixie->sprites.sprites[ spr_index ].data.label.text );
        }
    }
    pixie->sprites.sprites[ spr_index ].type = TYPE_SPRITE;
    pixie->sprites.sprites[ spr_index ].data.sprite.asset = asset + 1;
    pixie->sprites.sprites[ spr_index ].x = x;
    pixie->sprites.sprites[ spr_index ].y = y;
    pixie->sprites.sprites[ spr_index ].origin_x = 0;
    pixie->sprites.sprites[ spr_index ].origin_y = 0;
    pixie->sprites.sprites[ spr_index ].visible = 1;

    thread_mutex_unlock( &pixie->sprites.mutex );
    return spr_index + 1;
}


void sprite_bitmap( int spr_index, int asset ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( asset < 0 || asset >= pixie->assets.count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return;
    }

    // TODO: check that asset is sprite

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
    }

    --spr_index;

    if( pixie->sprites.sprites[ spr_index ].type != TYPE_SPRITE ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return;
    }

    pixie->sprites.sprites[ spr_index ].data.sprite.asset = asset + 1;
    thread_mutex_unlock( &pixie->sprites.mutex );
}


int sprite_visible( int spr_index ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    --spr_index;
    int visible = pixie->sprites.sprites[ spr_index ].visible;
    thread_mutex_unlock( &pixie->sprites.mutex );
    return visible;
}


void sprite_show( int spr_index ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return;
    }

    --spr_index;

    pixie->sprites.sprites[ spr_index ].visible = 1;
    thread_mutex_unlock( &pixie->sprites.mutex );
}


void sprite_hide( int spr_index ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return;
    }

    --spr_index;

    pixie->sprites.sprites[ spr_index ].visible = 0;
    thread_mutex_unlock( &pixie->sprites.mutex );
}


// Update sprite position without changing bitmap

void sprite_pos( int spr_index, int x, int y ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
    }

    --spr_index;
    pixie->sprites.sprites[ spr_index ].x = x;
    pixie->sprites.sprites[ spr_index ].y = y;
    thread_mutex_unlock( &pixie->sprites.mutex );
}


int sprite_x( int spr_index ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    --spr_index;
    int x = pixie->sprites.sprites[ spr_index ].x;
    thread_mutex_unlock( &pixie->sprites.mutex );
    return x;
}


int sprite_y( int spr_index ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    --spr_index;
    int y = pixie->sprites.sprites[ spr_index ].y;
    thread_mutex_unlock( &pixie->sprites.mutex );
    return y;
}


void sprite_origin( int spr_index, int x, int y ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
    }

    --spr_index;
    pixie->sprites.sprites[ spr_index ].origin_x = x;
    pixie->sprites.sprites[ spr_index ].origin_y = y;
    thread_mutex_unlock( &pixie->sprites.mutex );
}


int sprite_origin_x( int spr_index ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    --spr_index;
    int x = pixie->sprites.sprites[ spr_index ].origin_x;
    thread_mutex_unlock( &pixie->sprites.mutex );
    return x;
}


int sprite_origin_y( int spr_index ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    --spr_index;
    int y = pixie->sprites.sprites[ spr_index ].origin_y;
    thread_mutex_unlock( &pixie->sprites.mutex );
    return y;
}


void sprite_cel( int spr_index, int cel ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
    }

    --spr_index;

    if( pixie->sprites.sprites[ spr_index ].type != TYPE_SPRITE ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return;
    }

    pixie->sprites.sprites[ spr_index ].data.sprite.cel = cel;
    thread_mutex_unlock( &pixie->sprites.mutex );
}


int label( int spr_index, int x, int y, char const* text, int color, int font_asset ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );
    
    if( font_asset < 0 || font_asset >= pixie->assets.count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }
    
    --spr_index;
    if( pixie->sprites.sprites[ spr_index ].type == TYPE_LABEL ) {
        if( !pixie->sprites.sprites[ spr_index ].data.label.text ) {
            free( pixie->sprites.sprites[ spr_index ].data.label.text );
        }
    }
    pixie->sprites.sprites[ spr_index ].type = TYPE_LABEL;
    pixie->sprites.sprites[ spr_index ].data.label.text = strdup( text ? text : "" );
    pixie->sprites.sprites[ spr_index ].data.label.font = font_asset + 1;
    pixie->sprites.sprites[ spr_index ].data.label.color = color;
    pixie->sprites.sprites[ spr_index ].data.label.outline = -1;
    pixie->sprites.sprites[ spr_index ].data.label.shadow = -1;
    pixie->sprites.sprites[ spr_index ].data.label.wrap = -1;
    pixie->sprites.sprites[ spr_index ].data.label.align = TEXT_ALIGN_LEFT;
    pixie->sprites.sprites[ spr_index ].x = x;
    pixie->sprites.sprites[ spr_index ].y = y;
    pixie->sprites.sprites[ spr_index ].origin_x = 0;
    pixie->sprites.sprites[ spr_index ].origin_y = 0;
    pixie->sprites.sprites[ spr_index ].visible = 1;

    thread_mutex_unlock( &pixie->sprites.mutex );
    return spr_index + 1;
}


int label_text( int spr_index, char const* text ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );
    
    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }
    
    --spr_index;

    if( pixie->sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    if( pixie->sprites.sprites[ spr_index ].data.label.text ) free( pixie->sprites.sprites[ spr_index ].data.label.text );
    pixie->sprites.sprites[ spr_index ].data.label.text = strdup( text ? text : "" );
    thread_mutex_unlock( &pixie->sprites.mutex );
    return spr_index + 1;
}


int label_align( int spr_index, text_align_t align ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );
    
    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }
    
    --spr_index;

    if( pixie->sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    pixie->sprites.sprites[ spr_index ].data.label.align = align;

    thread_mutex_unlock( &pixie->sprites.mutex );
    return spr_index + 1;
}


int label_color( int spr_index, int color ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );
    
    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }
    
    --spr_index;

    if( pixie->sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    pixie->sprites.sprites[ spr_index ].data.label.color = color;

    thread_mutex_unlock( &pixie->sprites.mutex );
    return spr_index + 1;
}


int label_outline( int spr_index, int color ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );
    
    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }
    
    --spr_index;

    if( pixie->sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    pixie->sprites.sprites[ spr_index ].data.label.outline = color;

    thread_mutex_unlock( &pixie->sprites.mutex );
    return spr_index + 1;
}


int label_shadow( int spr_index, int color ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );
    
    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }
    
    --spr_index;

    if( pixie->sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    pixie->sprites.sprites[ spr_index ].data.label.shadow = color;

    thread_mutex_unlock( &pixie->sprites.mutex );
    return spr_index + 1;
}


int label_wrap( int spr_index, int wrap ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );
    
    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }
    
    --spr_index;

    if( pixie->sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
        return 0;
    }

    pixie->sprites.sprites[ spr_index ].data.label.wrap = wrap;

    thread_mutex_unlock( &pixie->sprites.mutex );
    return spr_index + 1;
}


void set_soundfont( int asset ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    if( asset < 0 || asset >= pixie->assets.count ) {
        return;
    }

    thread_mutex_lock( &pixie->audio.song_mutex );

    int soundfont_size = 0;
    void const* soundfont = pixie_find_asset( pixie, asset, &soundfont_size );

    tsf_close( pixie->audio.sound_font );
    pixie->audio.sound_font = tsf_load_memory( soundfont, soundfont_size );
    tsf_channel_set_bank_preset( pixie->audio.sound_font, 9, 128, 0 );
    tsf_set_output( pixie->audio.sound_font, TSF_STEREO_INTERLEAVED, 44100, 0.0f );
    thread_mutex_unlock( &pixie->audio.song_mutex );
}


void play_song( int asset ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    if( asset < 0 || asset >= pixie->assets.count ) {
        return;
    }

    thread_mutex_lock( &pixie->audio.song_mutex );

    memset( &pixie->audio.current_song, 0, sizeof( pixie->audio.current_song ) );

    int mid_size = 0;
    void const* mid_data = pixie_find_asset( pixie, asset, &mid_size );
    if( !mid_data ) {
        thread_mutex_unlock( &pixie->audio.song_mutex );
        return;
    }

    if( !mid_init_raw( &pixie->audio.current_song, mid_data, (size_t) mid_size ) ) {
        thread_mutex_unlock( &pixie->audio.song_mutex );
        return;
    }

    tsf_reset( pixie->audio.sound_font );
    tsf_channel_set_bank_preset( pixie->audio.sound_font, 9, 128, 0 );
    tsf_set_output( pixie->audio.sound_font, TSF_STEREO_INTERLEAVED, 44100, 0.0f );
    mid_skip_leading_silence( &pixie->audio.current_song, pixie->audio.sound_font );

    thread_mutex_unlock( &pixie->audio.song_mutex );
}


char const* load_text( int asset ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    int size = 0;
    void const* data = pixie_find_asset( pixie, asset, &size );

    return (char const*) data;
}


int asset_size( int asset ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    int size = 0;
    pixie_find_asset( pixie, asset, &size );

    return size;
}


void const* asset_data( int asset ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    int size = 0;
    return pixie_find_asset( pixie, asset, &size );
}


void move_sprite_x( int spr_index, move_t* moves, int moves_count ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
    }

    --spr_index;
    int max_count = ARRAY_COUNT( pixie->sprites.sprites[ spr_index ].move_x.moves );
    if( moves_count > max_count ) {
        moves_count = max_count;
    }
    memcpy( pixie->sprites.sprites[ spr_index ].move_x.moves, moves, sizeof( *moves ) * moves_count );
    pixie->sprites.sprites[ spr_index ].move_x.count = moves_count;
    pixie->sprites.sprites[ spr_index ].move_x.index = 0;
    pixie->sprites.sprites[ spr_index ].move_x.time = 0;
    pixie->sprites.sprites[ spr_index ].move_x.start = pixie->sprites.sprites[ spr_index ].y;
    thread_mutex_unlock( &pixie->sprites.mutex );
}


void move_sprite_y( int spr_index, move_t* moves, int moves_count ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    thread_mutex_lock( &pixie->sprites.mutex );

    if( spr_index < 1 || spr_index > pixie->sprites.sprite_count ) {
        thread_mutex_unlock( &pixie->sprites.mutex );
    }

    --spr_index;
    int max_count = ARRAY_COUNT( pixie->sprites.sprites[ spr_index ].move_y.moves );
    if( moves_count > max_count ) {
        moves_count = max_count;
    }
    memcpy( pixie->sprites.sprites[ spr_index ].move_y.moves, moves, sizeof( *moves ) * moves_count );
    pixie->sprites.sprites[ spr_index ].move_y.count = moves_count;
    pixie->sprites.sprites[ spr_index ].move_y.index = 0;
    pixie->sprites.sprites[ spr_index ].move_y.time = 0;
    pixie->sprites.sprites[ spr_index ].move_y.start = pixie->sprites.sprites[ spr_index ].y;
    thread_mutex_unlock( &pixie->sprites.mutex );
}


void text( int x, int y, char const* str, int color, int font_asset 
	/*, text_align align, int wrap_width, int hspacing, int vspacing, int limit, bool bold, bool italic, bool underline */) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

    pixelfont_align_t pixelfont_align = PIXELFONT_ALIGN_LEFT;
	//if( align == ALIGNMENT_RIGHT ) pixelfont_align = PIXELFONT_ALIGN_RIGHT;
	//if( align == ALIGNMENT_CENTER ) pixelfont_align = PIXELFONT_ALIGN_CENTER;

	pixelfont_bounds_t pixelfont_bounds;
	pixelfont_bounds.width = 0;
	pixelfont_bounds.height = 0;

    pixelfont_t* pixelfont = (pixelfont_t*) pixie_find_asset( pixie, font_asset, NULL );
	pixelfont_blit_u8( pixelfont, x, y, str, (u8) color, 
        pixie->screen.pixels, pixie->screen.screen_width, pixie->screen.screen_height,
        pixelfont_align, -1, 0, 0, -1, PIXELFONT_BOLD_OFF, PIXELFONT_ITALIC_OFF, PIXELFONT_UNDERLINE_OFF, 
        &pixelfont_bounds );

		//pixelfont_align, wrap_width, hspacing, vspacing, limit, bold ? PIXELFONT_BOLD_ON : PIXELFONT_BOLD_OFF, 
		//italic ? PIXELFONT_ITALIC_ON : PIXELFONT_ITALIC_OFF, underline ? PIXELFONT_UNDERLINE_ON : PIXELFONT_UNDERLINE_OFF, 
		//bounds ? &pixelfont_bounds : 0 );

/*
	if( bounds )
		{
		bounds->width = pixelfont_bounds.width;
		bounds->height = pixelfont_bounds.height;
		}
*/
}


int key_is_down( key_t key ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

	if( key < 0 || key >= sizeof( pixie->keyboard.state ) / sizeof( *pixie->keyboard.state ) ) return 0;
	return pixie->keyboard.state[ key ]; 
}


int key_was_pressed( key_t key ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

	if( key < 0 || key >= sizeof( pixie->keyboard.state ) / sizeof( *pixie->keyboard.state ) ) return 0;
	if( key < 0 || key >= sizeof( pixie->keyboard.prev ) / sizeof( *pixie->keyboard.prev ) ) return 0;
    int a = pixie->keyboard.state[ key ] ;
    int b = pixie->keyboard.prev[ key ] ;
    (void) a,b;
	return pixie->keyboard.state[ key ] && !pixie->keyboard.prev[ key ]; 
}


int key_was_released( key_t key ) {
    pixie_t* pixie = pixie_instance(); // Get `pixie_t` instance from thread local storage

	if( key < 0 || key >= sizeof( pixie->keyboard.state ) / sizeof( *pixie->keyboard.state ) ) return 0;
	if( key < 0 || key >= sizeof( pixie->keyboard.prev ) / sizeof( *pixie->keyboard.prev ) ) return 0;
	return !pixie->keyboard.state[ key ] && pixie->keyboard.prev[ key ]; 
}


#undef min
int min( int a, int b ) {
    return a < b ? a : b;
}


string str( char const* c_str ) {
    ASSERT( c_str, "The 'c_str' parameter is NULL" );
    ASSERTF( strlen( c_str ) < PIXIE_MAX_STRING_LENGTH, 
        ( "The 'c_str' parameter is a string of length %d, which is longer than the max length of %d.\n\n"
          "The string contents are:\n %s", (int) strlen( c_str ) + 1, PIXIE_MAX_STRING_LENGTH, c_str ) );

    string str = { "" } ;
    if( c_str ) {
        size_t len = strlen( c_str );
        if( len < PIXIE_MAX_STRING_LENGTH ) {
            memcpy( str.c_str, c_str, len + 1 ); 
        }
    }
    return str;
}


int length( string str ) {
    return (int) strlen( str.c_str );
}


string concat( string a, string b ) {
    ASSERTF( strlen( a.c_str ) + strlen( b.c_str ) < PIXIE_MAX_STRING_LENGTH, 
        ( "The combined length of string a (length %d) and string b (length %d) is longer than the max length of %d.\n\n"
          "The contents of string a are:\n%s\n\nThe contents of string b are:\n%s", 
          (int) strlen( a.c_str ) + 1, (int) strlen( b.c_str ) + 1, PIXIE_MAX_STRING_LENGTH, a.c_str, b.c_str ) );

    string str = { "" };
    if( strlen( a.c_str ) + strlen( b.c_str ) < PIXIE_MAX_STRING_LENGTH ) {
        strcpy( str.c_str, a.c_str );
        strcat( str.c_str, b.c_str );
    }
    return str;
}


int compare( string a, string b ) {
    return strcmp( a.c_str, b.c_str );
}


string trim( string str ) {
	char const* start = str.c_str;
	while( *start && *start <= ' ' ) 
		++start;
	char const* end = str.c_str + strlen( str.c_str ) - 1;
	while( end > start && *end <= ' ' )
		--end;
    string out = { "" };
	strncpy( out.c_str, start, (size_t)( end - start + 1 ) );
    out.c_str[ end - start + 1] = '\0';
    return out;
}
/*

string ltrim( string str ) {
}


string rtrim( string str ) {
}


string left( string source, int number ) {
}


string right( string source, int number ) {
}

*/

string mid( string source, int offset, int number ) {
	int const len = (int) strlen( source.c_str );
	if( offset < 0 ) offset = 0;
	if( offset > len ) offset = len;
	if( number < 0 ) number = len - offset;
	if( offset + number > len ) number = len - offset;
    string out = { "" };
	strncpy( out.c_str, source.c_str + offset, (size_t) number );
    out.c_str[ number ] = '\0';
    return out;
}

/*
int instr( string haystack, string needle, int start ) {
}


int any( string haystack, string needles, int start ) {
}


string upper( string str ) {
}


string lower( string str ) {
}


string string_from_int( int x ) {
}


string string_from_float( float x ) {
}


float float_from_string( string str ) {
}


int int_from_string( string str ) {
}


string space ( int number ) {
}


string flip( string original ) {
}


string repeat( string text, int number ) {
}


string chr( int code ) {
}


int asc( string str ) {
}


int len( string str ) {
}
*/

int starts_with( string str, string start ) {
    return strnicmp( str.c_str, start.c_str, strlen( start.c_str ) ) == 0;
}
/*

string format( string format_string, ... ) {
}

*/



u32 hash_str( string str ) {
    u32 hash = 5381u; 
    char const* s = str.c_str;
    while( *s ) hash = ( ( hash << 5u ) + hash) ^ *s++;
    return hash;
}


#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
} /* namespace pixie */
#endif


/*
-------------------------
    ENTRY POINT (MAIN)
-------------------------
*/     

#ifndef PIXIE_NO_MAIN

    #if defined( _WIN32 ) && !defined( __TINYC__ )
        #ifndef NDEBUG
            #pragma warning( push ) 
            #pragma warning( disable: 4619 ) // pragma warning : there is no warning number 'number'
            #pragma warning( disable: 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
            #include <crtdbg.h>
            #pragma warning( pop ) 
        #endif
    #endif /* _WIN32 && !__TINYC__ */

    int pixmain( int argc, char** argv );

    int main( int argc, char** argv ) {
        (void) argc, argv;
		#if defined( _WIN32 ) && !defined( __TINYC__ )
            #ifndef NDEBUG
                int flag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ); // Get current flag
                flag ^= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
                _CrtSetDbgFlag( flag ); // Set flag to the new value
                _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG );
                _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
//                _CrtSetBreakAlloc( 0 );
            #endif
        #endif

        #if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
            using pixie::run;
        #endif

        return run( pixmain );
    } 

    #ifdef _WIN32
        // pass-through so the program will build with either /SUBSYSTEM:WINDOWS or /SUBSYSTEN:CONSOLE
        struct HINSTANCE__;
        #ifdef __cplusplus
        extern "C" {
        #endif
            int __stdcall WinMain( struct HINSTANCE__* a, struct HINSTANCE__* b, char* c, int d ) { (void) a, b, c, d; return main( __argc, __argv ); }
        #ifdef __cplusplus
        }
        #endif
    #endif /* _WIN32 */

#endif /* PIXIE_NO_MAIN */


/*
---------------------------------
    LIBRARIES IMPLEMENTATIONS
---------------------------------
*/
      
#define APP_IMPLEMENTATION
#ifdef _WIN32
    #define APP_WINDOWS
#endif
#define APP_LOG( ctx, level, message )
#include "app.h"

#define CRTEMU_IMPLEMENTATION
#include "crtemu.h"

#define CRT_FRAME_IMPLEMENTATION
#include "crt_frame.h"

#define EASE_IMPLEMENTATION
#if !defined( PIXIE_NO_MATH ) || defined( __TINYC__ )
    #if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
        #define EASE_ACOS( x ) pixie::acos( x )
        #define EASE_COS( x ) pixie::cos( x )
        #define EASE_POW( x, y ) pixie::pow( x, y )
        #define EASE_SIN( x ) pixie::sin( x )
        #define EASE_SQRT( x ) pixie::sqrt( x )
    #else
        #define EASE_ACOS( x ) internal_pixie_acos( x )
        #define EASE_COS( x ) internal_pixie_cos( x )
        #define EASE_POW( x, y ) internal_pixie_pow( x, y )
        #define EASE_SIN( x ) internal_pixie_sin( x )
        #define EASE_SQRT( x ) internal_pixie_sqrt( x )
    #endif
#endif
#include "ease.h"
        
#define FRAMETIMER_IMPLEMENTATION
#include "frametimer.h"

#define MID_IMPLEMENTATION
#define MID_NO_TSF_IMPLEMENTATION
#include "mid.h"

#define MMAP_IMPLEMENTATION
#include "mmap.h"

#define PALRLE_IMPLEMENTATION
#include "palrle.h"

#define PIXELFONT_IMPLEMENTATION
#define PIXELFONT_FUNC_NAME pixelfont_blit_u8
#include "pixelfont.h"
#undef PIXELFONT_FUNC_NAME 

#ifndef PIXIE_NO_BUILD
    #define PIXIE_BUILD_IMPLEMENTATION
    #include "pixie_build.h"
#endif

#define PIXIE_DATA_IMPLEMENTATION
#include "pixie_data.h"
        
#define THREAD_IMPLEMENTATION
#include "thread.h"

#pragma warning( push )
#pragma warning( disable: 4242 )
#pragma warning( disable: 4244 )
#pragma warning( disable: 4365 )
#pragma warning( disable: 4668 )
#pragma warning( disable: 4701 )
#pragma warning( disable: 4703 )
#define TSF_NO_STDIO
#define TSF_IMPLEMENTATION
#if !defined( PIXIE_NO_MATH ) || defined( __TINYC__ )
    #if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
        #define TSF_POW pixie::pow
        #define TSF_POWF pixie::pow
        #define TSF_EXPF pixie::exp
        #define TSF_LOG pixie::log
        #define TSF_TAN pixie::tan
        #define TSF_LOG10 pixie::log10
        #define TSF_SQRT pixie::sqrt
        #define TSF_SQRTF pixie::sqrt
    #else
        #define TSF_POW internal_pixie_pow
        #define TSF_POWF internal_pixie_pow
        #define TSF_EXPF internal_pixie_exp
        #define TSF_LOG internal_pixie_log
        #define TSF_TAN internal_pixie_tan
        #define TSF_LOG10 internal_pixie_log10
        #define TSF_SQRT internal_pixie_sqrt
        #define TSF_SQRTF internal_pixie_sqrt
    #endif
#endif
#include "tsf.h"
#pragma warning( pop )


/*
-------------
    ASSERT
-------------
*/

#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
namespace pixie {
#endif

char const* format_assert_message( char const* format, ... ) {
	static char buffer[ 4096 ];
	va_list args;
	va_start( args, format );
	_vsnprintf( buffer, sizeof( buffer ), format, args );
	va_end( args );
	return buffer;
}


#ifdef _WIN32
	int display_assert_message( char const* expression, char const* message, char const* file, int line, char const* function ) {
	    char buf[ 4096 + 64 ];
	    _snprintf( buf, 4095, "ASSERTION FAILED!\n\n%s\n\nExpression: %s\n\nFunction: %s\n\n%s(%d)\n", 
            message, expression, function, file, line );
	    OutputDebugString( "\n**********************************************************************\n" );
	    OutputDebugString( buf );
	    OutputDebugString( "**********************************************************************\n\n" );
	    printf( "\n**********************************************************************\n" );
	    printf( "%s", buf );
	    printf( "**********************************************************************\n\n" );
		strcat( buf, "\nBreak into debugger?\n" );
	    int result = MessageBox( 0, buf, "Pixie Assert", MB_ICONERROR | MB_YESNOCANCEL | MB_SYSTEMMODAL | MB_SETFOREGROUND );
	    switch( result ) {
		    case IDCANCEL: {
			    // Turn off memory leak reports for faster exit
                #ifndef __TINYC__
			        #ifndef NDEBUG
				        int flag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ); // Get current flag
				        flag ^= _CRTDBG_LEAK_CHECK_DF; // Turn off leak-checking bit
				        _CrtSetDbgFlag( flag ); // Set flag to the new value
			        #endif
                #endif
			    _exit( 3 ); // Exit application immediately, without calling crt's _atexit
			} break;
		    case IDYES: {
			    return 1; // Break to editor
			} break;
		    case IDNO: {
			    return 0; // Continue execution
			} break;
		}
		return 0;
	}
#else /* _WIN32 */
	#error Platform not supported
#endif /* _WIN32 */


#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
} /* namespace pixie */
#endif



/*
---------------------
    MATH WRAPPERS
---------------------
*/

#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )

    #pragma warning( push )
    #pragma warning( disable: 4577 )
    #include <cmath>
    #pragma warning( pop )

    float internal_pixie_acos( float x ) { return (float) std::acos( (double) x ); }
    float internal_pixie_asin( float x ) { return (float) std::asin( (double) x ); }
    float internal_pixie_atan( float x ) { return (float) std::atan( (double) x ); }
    float internal_pixie_atan2( float x, float y ) { return (float) std::atan2( (double) x, (double)y ); }
    float internal_pixie_ceil( float x ) { return (float) std::ceil( (double) x ); }
    float internal_pixie_cos( float x ) { return (float) std::cos( (double) x ); }
    float internal_pixie_cosh( float x ) { return (float) std::cosh( (double) x ); }
    float internal_pixie_exp( float x ) { return (float) std::exp( (double) x ); }
    float internal_pixie_fabs( float x ) { return (float) std::fabs( (double) x ); }
    float internal_pixie_floor( float x ) { return (float) std::floor( (double) x ); }
    float internal_pixie_fmod( float x, float y ) { return (float) std::fmod( (double) x, (double)y ); }
    float internal_pixie_log( float x ) { return (float) std::log( (double) x ); }
    float internal_pixie_log10( float x ) { return (float) std::log10( (double) x ); }
    float internal_pixie_modf( float x, float* y ) { return (float) std::modf( (double) x, (double*)y ); }
    float internal_pixie_pow( float x, float y ) { return (float) std::pow( (double) x, (double)y ); }
    float internal_pixie_sqrt( float x ) { return (float) std::sqrt( (double) x ); }
    float internal_pixie_sin( float x ) { return (float) std::sin( (double) x ); }
    float internal_pixie_sinh( float x ) { return (float) std::sinh( (double) x ); }
    float internal_pixie_tan( float x ) { return (float) std::tan( (double) x ); }
    float internal_pixie_tanh( float x ) { return (float) std::tanh( (double) x ); }

    namespace pixie {
        float acos( float x ) { return internal_pixie_acos( x ); }
        float asin( float x ) { return internal_pixie_asin( x ); }
        float atan( float x ) { return internal_pixie_atan( x ); }
        float atan2( float x, float y ) { return internal_pixie_atan2( x, y ); }
        float ceil( float x ) { return internal_pixie_ceil( x ); }
        float cos( float x ) { return internal_pixie_cos( x ); }
        float cosh( float x ) { return internal_pixie_cosh( x ); }
        float exp( float x ) { return internal_pixie_exp( x ); }
        float fabs( float x ) { return internal_pixie_fabs( x ); }
        float floor( float x ) { return internal_pixie_floor( x ); }
        float fmod( float x, float y ) { return internal_pixie_fmod( x, y ); }
        float log( float x ) { return internal_pixie_log( x ); }
        float log10( float x ) { return internal_pixie_log10( x ); }
        float modf( float x, float* y ) { return internal_pixie_modf( x, y ); }
        float pow( float x, float y ) { return internal_pixie_pow( x, y ); }
        float sqrt( float x ) { return internal_pixie_sqrt( x ); }
        float sin( float x ) { return internal_pixie_sin( x ); }
        float sinh( float x ) { return internal_pixie_sinh( x ); }
        float tan( float x ) { return internal_pixie_tan( x ); }
        float tanh( float x ) { return internal_pixie_tanh( x ); }
    } /* namespace pixie */

#else /* defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE ) */

    #pragma warning( push )
    #pragma warning( disable: 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
    #include <math.h>
    #pragma warning( pop )

    #ifdef __TINYC__
        float internal_pixie_acos( float x ) { return (float)acos( (double) x ); }
        float internal_pixie_asin( float x ) { return (float)asin( (double) x ); }
        float internal_pixie_atan( float x ) { return (float)atan( (double) x ); }
        float internal_pixie_atan2( float x, float y ) { return (float)atan2( (double) x, (double)y ); }
        float internal_pixie_ceil( float x ) { return (float)ceil( (double) x ); }
        float internal_pixie_cos( float x ) { return (float)cos( (double) x ); }
        float internal_pixie_cosh( float x ) { return (float)cosh( (double) x ); }
        float internal_pixie_exp( float x ) { return (float)exp( (double) x ); }
        float internal_pixie_fabs( float x ) { return (float)fabs( (double) x ); }
        float internal_pixie_floor( float x ) { return (float)floor( (double) x ); }
        float internal_pixie_fmod( float x, float y ) { return (float)fmod( (double) x, (double)y ); }
        float internal_pixie_log( float x ) { return (float)log( (double) x ); }
        float internal_pixie_log10( float x ) { return (float)log10( (double) x ); }
        float internal_pixie_modf( float x, float* y ) { return (float)modf( (double) x, (double*)y ); }
        float internal_pixie_pow( float x, float y ) { return (float)pow( (double) x, (double)y ); }
        float internal_pixie_sqrt( float x ) { return (float)sqrt( (double) x ); }
        float internal_pixie_sin( float x ) { return (float)sin( (double) x ); }
        float internal_pixie_sinh( float x ) { return (float)sinh( (double) x ); }
        float internal_pixie_tan( float x ) { return (float)tan( (double) x ); }
        float internal_pixie_tanh( float x ) { return (float)tanh( (double) x ); }
    #else /* __TINYC__ */
        float internal_pixie_acos( float x ) { return acosf( x ); }
        float internal_pixie_asin( float x ) { return asinf( x ); }
        float internal_pixie_atan( float x ) { return atanf( x ); }
        float internal_pixie_atan2( float x, float y ) { return atan2f( x, y ); }
        float internal_pixie_ceil( float x ) { return ceilf( x ); }
        float internal_pixie_cos( float x ) { return cosf( x ); }
        float internal_pixie_cosh( float x ) { return coshf( x ); }
        float internal_pixie_exp( float x ) { return expf( x ); }
        float internal_pixie_fabs( float x ) { return fabsf( x ); }
        float internal_pixie_floor( float x ) { return floorf( x ); }
        float internal_pixie_fmod( float x, float y ) { return fmodf( x, y ); }
        float internal_pixie_log( float x ) { return logf( x ); }
        float internal_pixie_log10( float x ) { return log10f( x ); }
        float internal_pixie_modf( float x, float* y ) { return modff( x, y ); }
        float internal_pixie_pow( float x, float y ) { return powf( x, y ); }
        float internal_pixie_sqrt( float x ) { return sqrtf( x ); }
        float internal_pixie_sin( float x ) { return sinf( x ); }
        float internal_pixie_sinh( float x ) { return sinhf( x ); }
        float internal_pixie_tan( float x ) { return tanf( x ); }
        float internal_pixie_tanh( float x ) { return tanhf( x ); }
    #endif

    // Redefine math wrappers again so they can be used by the file that included implementation
    #define acos internal_pixie_acos
    #define asin internal_pixie_asin
    #define atan internal_pixie_atan
    #define atan2 internal_pixie_atan2
    #define ceil internal_pixie_ceil
    #define cos internal_pixie_cos 
    #define cosh internal_pixie_cosh
    #define exp internal_pixie_exp 
    #define fabs internal_pixie_fabs
    #define floor internal_pixie_floor
    #define fmod internal_pixie_fmod
    #define log internal_pixie_log 
    #define log10 internal_pixie_log10
    #define modf internal_pixie_modf
    #define pow internal_pixie_pow 
    #define sqrt internal_pixie_sqrt
    #define sin internal_pixie_sin 
    #define sinh internal_pixie_sinh
    #define tan internal_pixie_tan 
    #define tanh internal_pixie_tanh
#endif /* defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE ) */



#endif /* PIXIE_IMPLEMENTATION */



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
