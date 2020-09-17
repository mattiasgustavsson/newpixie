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

// Sized int types
#ifndef PIXIE_I8 
    #define PIXIE_I8 signed char
#endif
#ifndef PIXIE_I16 
    #define PIXIE_I16 signed short
#endif
#ifndef PIXIE_I32
    #define PIXIE_I32 signed int
#endif
#ifndef PIXIE_I64
    #define PIXIE_I64 signed long long
#endif
#ifndef PIXIE_U8 
    #define PIXIE_U8 unsigned char
#endif
#ifndef PIXIE_U16 
    #define PIXIE_U16 unsigned short
#endif
#ifndef PIXIE_U32
    #define PIXIE_U32 unsigned int
#endif
#ifndef PIXIE_U64
    #define PIXIE_U64 unsigned long long
#endif

// Sized int typedefs
typedef PIXIE_I8 i8;
typedef PIXIE_I16 i16;
typedef PIXIE_I32 i32;
typedef PIXIE_I64 i64;
typedef PIXIE_U8 u8;
typedef PIXIE_U16 u16;
typedef PIXIE_U32 u32;
typedef PIXIE_U64 u64;


// API Functions

int run( int (*main)( int, char** ), int argc, char** argv );
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

typedef struct move_t { u32 data[ 4 ]; /* opaque struct, 16 bytes long */ } move_t;

move_t move_loop( void );
move_t move_end( void );
move_t move_delay( int duration ); 
move_t move_linear( int duration, int target ); 
move_t move_smoothstep( int duration, int target ); 
move_t move_smootherstep( int duration, int target ); 
move_t move_ease_out_quad( int duration, int target ); 
move_t move_ease_out_back( int duration, int target ); 
move_t move_ease_out_bounce( int duration, int target ); 
move_t move_ease_out_sine( int duration, int target ); 
move_t move_ease_out_elastic( int duration, int target ); 
move_t move_ease_out_expo( int duration, int target ); 
move_t move_ease_out_cubic( int duration, int target ); 
move_t move_ease_out_quart( int duration, int target ); 
move_t move_ease_out_quint( int duration, int target ); 
move_t move_ease_out_circle( int duration, int target ); 
move_t move_ease_in_quad( int duration, int target ); 
move_t move_ease_in_back( int duration, int target ); 
move_t move_ease_in_bounce( int duration, int target ); 
move_t move_ease_in_sine( int duration, int target ); 
move_t move_ease_in_elastic( int duration, int target ); 
move_t move_ease_in_expo( int duration, int target ); 
move_t move_ease_in_cubic( int duration, int target ); 
move_t move_ease_in_quart( int duration, int target ); 
move_t move_ease_in_quint( int duration, int target ); 
move_t move_ease_in_circle( int duration, int target ); 
move_t move_ease_in_out_quad( int duration, int target ); 
move_t move_ease_in_out_back( int duration, int target ); 
move_t move_ease_in_out_bounce( int duration, int target ); 
move_t move_ease_in_out_sine( int duration, int target ); 
move_t move_ease_in_out_elastic( int duration, int target ); 
move_t move_ease_in_out_expo( int duration, int target ); 
move_t move_ease_in_out_cubic( int duration, int target ); 
move_t move_ease_in_out_quart( int duration, int target ); 
move_t move_ease_in_out_quint( int duration, int target );
move_t move_ease_in_out_circle( int duration, int target );

void sprite_move_x( int spr_index, move_t moves, ... );
void sprite_move_y( int spr_index, move_t moves, ... );

#define ARRAY_COUNT( x ) ( (int)( sizeof( x ) / sizeof( *x ) ) )

void set_soundfont( int asset );
void play_song( int asset );
char const* load_text( int asset_id );

int asset_size( int asset_id );
void const* asset_data( int asset_id );

void text( int x, int y, char const* str, int color, int font_asset 
	/*, text_align align, int wrap_width, int hspacing, int vspacing, int limit, bool bold, bool italic, 
    bool underline */ );

typedef enum keys_t { 
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
} keys_t;

int key_is_down( keys_t key );
int key_was_pressed( keys_t key );
int key_was_released( keys_t key );


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
int any( string haystack, string needles, int start );  // search haystack for next occurrence of any char from needles
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

u32 hash_string( string str );

// TODO: hash funcs for more types


#define LOOP for( ; ; )


int internal_pixie_display_assert_message( char const* expression, char const* message, char const* file, int line, 
    char const* function );
char const* internal_pixie_format_assert_message( char const* format, ... );

#if defined NDEBUG && !defined( PIXIE_ASSERT_IN_RELEASE_BUILD )
	#define ASSERT( expression, message )
#elif defined PIXIE_ASSERT
	#define ASSERT( expression, message ) PIXIE_ASSERT( expression, message )
#else
	#ifndef _MSC_VER 
		#define ASSERT( expression, message ) \
			do { \
				if( !( expression ) ) { \
					if( internal_pixie_display_assert_message( #expression, message, __FILE__, __LINE__, \
                        __FUNCTION__ ) ) { \
							__asm__ volatile("int $0x03"); \
                    } \
                } \
			} while( 0 )
	#else
		#define ASSERT( expression, message ) \
			__pragma( warning( push ) ) \
			__pragma( warning( disable: 4127 ) ) \
			do { \
				if( !( expression ) ) { \
					if( internal_pixie_display_assert_message( #expression, message, __FILE__, __LINE__, \
                        __FUNCTION__ ) ) { \
							__debugbreak(); \
                    } \
                } \
			} while( 0 ) \
			__pragma( warning( pop ) )
	#endif		
#endif

#define ASSERTF( expression, message ) ASSERT( expression, internal_pixie_format_assert_message message )


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
    // If data builds are disabled, we just define the functions to load a bundle, not create it.
    #define ASSETS_BEGIN( bundle_filename ) \
        int internal_pixie_load_bundle( char const* filename, char const* time, char const* definitions, \
            int count ); \
        static int load_assets( void ) { return internal_pixie_load_bundle( bundle_filename, (void*) 0, \
            (void*) 0, -1 ); } \
        enum internal_pixie_assets_t {

    #define ASSETS_END() \
        } /* enum internal_pixie_assets_t */;
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
    // Forward declare our functions with unique names, and then use macros to give them the good names. In the 
    // implementation, we will temporarily undefine them while including <math.h> to avoid name conflicts, and then 
    // redefine them again, so the exposed symbols of `sin`, `cos` etc. will be mapped to `internal_pixie_sin`, 
    // `internal_pixie_cos` etc. which will call the <math.h> `sin`, `cos`
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
#endif /* PIXIE_NO_MATH */

      
/*
----------------------
    DICTIONARY TYPE
----------------------
*/

#define INTERNAL_DICTIONARY_U32 u32

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
            ASSERTF( dict->slots[ slot ].item_index <= 0 && \
                (int)( hash % slot_capacity ) == (INTERNAL_DICTIONARY_U32) base_slot, ( "Internal error" ) ); \
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

#define DECLARE_ARRAY( NAME, TYPE, CAPACITY ) \
    typedef struct NAME##_array { \
        int count; \
        TYPE items[ CAPACITY ]; \
    } NAME##_array; \
    \
    static int NAME##_count( NAME##_array* arr ) { \
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
    static void NAME##_add( NAME##_array* arr, TYPE value ) { \
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
    static int NAME##_try_add( NAME##_array* arr, TYPE value ) { \
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
    static void NAME##_remove( NAME##_array* arr, int index ) { \
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
    static int NAME##_try_remove( NAME##_array* arr, int index ) { \
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
    static TYPE NAME##_get( NAME##_array const* arr, int index ) { \
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
    static TYPE const* NAME##_try_get( NAME##_array const* arr, int index ) { \
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
    struct internal_pixie_sort_stack_##NAME##_t { \
        int start; \
        int count; \
    }; \
    \
    static int internal_pixie_sort_##NAME##_min( int a, int b ) { \
	    return ( ( a < b ) ? a : b ); \
    } \
    \
	static TYPE* internal_pixie_sort_##NAME##_med3( TYPE* a, TYPE* b, TYPE* c ) { \
		return ( COMPARE( *a, *b ) < 0 \
			? ( COMPARE( *b, *c ) < 0 ? b : COMPARE( *a, *c ) < 0 ? c : a ) \
			: ( COMPARE( *b, *c ) > 0 ? b : COMPARE( *a, *c ) > 0 ? c : a) ); \
    } \
    \
	static void internal_pixie_sort_##NAME##_swap( TYPE* a, TYPE* b ) { \
		TYPE t = *a; \
        *a = *b; \
        *b = t; \
    } \
	\
	static void internal_pixie_sort_##NAME##_swap_range( TYPE* a, TYPE* b, int n ) { \
		int sn = (n); \
        TYPE* sa = (a); \
        TYPE* sb = (b); \
        while( sn > 0 ) { \
            internal_pixie_sort_##NAME##_swap( sa, sb ); \
            ++sa; \
            ++sb; \
            --sn; \
        } \
    }\
    \
    static void NAME( TYPE* array, int count ) { \
	    struct internal_pixie_sort_stack_##NAME##_t stack[ 32 ]; \
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
					    internal_pixie_sort_##NAME##_swap( pl, pl - 1 ); \
                    } \
                } \
			    continue; \
			} \
		    TYPE* pm = a + count / 2; /* Small arrays, middle element */ \
		    if( count > 40 ) { /* Big arrays, pseudomedian of 9 */ \
			    TYPE* pl = a; \
			    TYPE* pn = a + count - 1; \
			    int s = count / 8; \
			    pl = internal_pixie_sort_##NAME##_med3( pl, pl + s, pl + 2 * s ); \
			    pm = internal_pixie_sort_##NAME##_med3( pm - s, pm, pm + s ); \
			    pn = internal_pixie_sort_##NAME##_med3( pn - 2 * s, pn - s, pn ); \
			    pm = internal_pixie_sort_##NAME##_med3( pl, pm, pn ); /* Mid-size, med of 3 */ \
			} \
		    TYPE* pv = a;  \
            internal_pixie_sort_##NAME##_swap( pv, pm ); /* pv points to partition value */ \
		    TYPE* pa = a; \
		    TYPE* pb = a; \
		    TYPE* pc = a + count - 1; \
		    TYPE* pd = pc; \
		    for( ;; ) { \
			    int r; \
			    while( pb <= pc && ( r = COMPARE( *pb, *pv ) ) <= 0 ) { \
				    if( r == 0 ) { \
                        internal_pixie_sort_##NAME##_swap( pa, pb ); \
                        ++pa; \
                    } \
				    ++pb; \
				} \
			    while( pc >= pb && ( r = COMPARE( *pc, *pv ) ) >= 0 ) { \
				    if( r == 0 ) { \
                        internal_pixie_sort_##NAME##_swap( pc, pd ); \
                        --pd; \
                    } \
				    --pc; \
				} \
			    if( pb > pc ) break; \
			    internal_pixie_sort_##NAME##_swap( pb, pc ); \
			    ++pb; \
                --pc; \
            } \
		    TYPE* pn = a + count; \
		    int s = internal_pixie_sort_##NAME##_min( (int)( pa - a ), (int)( pb - pa ) ); \
            internal_pixie_sort_##NAME##_swap_range( a, pb - s, s ); \
		    s = internal_pixie_sort_##NAME##_min( (int)( pd - pc ), (int)( pn - pd - 1 ) ); \
            internal_pixie_sort_##NAME##_swap_range( pb, pn - s, s ); \
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
#endif /* PIXIE_NO_MATH */


// C runtime includes
#include <setjmp.h>
#define wait PIXIE_STD_WAIT /* osx defines a conflicting "wait" */
#include <stdlib.h>
#undef wait
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdarg.h>

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


/*
-----------------------
    PIXIE INTERNALS
-----------------------
*/

typedef enum internal_pixie_move_type_t {
    INTERNAL_PIXIE_MOVE_LOOP, INTERNAL_PIXIE_MOVE_END, INTERNAL_PIXIE_MOVE_DELAY, INTERNAL_PIXIE_MOVE_LINEAR, 
    INTERNAL_PIXIE_MOVE_SMOOTHSTEP, INTERNAL_PIXIE_MOVE_SMOOTHERSTEP, INTERNAL_PIXIE_MOVE_EASE_OUT_QUAD, 
    INTERNAL_PIXIE_MOVE_EASE_OUT_BACK, INTERNAL_PIXIE_MOVE_EASE_OUT_BOUNCE, INTERNAL_PIXIE_MOVE_EASE_OUT_SINE, 
    INTERNAL_PIXIE_MOVE_EASE_OUT_ELASTIC, INTERNAL_PIXIE_MOVE_EASE_OUT_EXPO, INTERNAL_PIXIE_MOVE_EASE_OUT_CUBIC, 
    INTERNAL_PIXIE_MOVE_EASE_OUT_QUART, INTERNAL_PIXIE_MOVE_EASE_OUT_QUINT, INTERNAL_PIXIE_MOVE_EASE_OUT_CIRCLE, 
    INTERNAL_PIXIE_MOVE_EASE_IN_QUAD, INTERNAL_PIXIE_MOVE_EASE_IN_BACK, INTERNAL_PIXIE_MOVE_EASE_IN_BOUNCE, 
    INTERNAL_PIXIE_MOVE_EASE_IN_SINE, INTERNAL_PIXIE_MOVE_EASE_IN_ELASTIC, INTERNAL_PIXIE_MOVE_EASE_IN_EXPO, 
    INTERNAL_PIXIE_MOVE_EASE_IN_CUBIC, INTERNAL_PIXIE_MOVE_EASE_IN_QUART, INTERNAL_PIXIE_MOVE_EASE_IN_QUINT, 
    INTERNAL_PIXIE_MOVE_EASE_IN_CIRCLE, INTERNAL_PIXIE_MOVE_EASE_IN_OUT_QUAD, INTERNAL_PIXIE_MOVE_EASE_IN_OUT_BACK, 
    INTERNAL_PIXIE_MOVE_EASE_IN_OUT_BOUNCE, INTERNAL_PIXIE_MOVE_EASE_IN_OUT_SINE, 
    INTERNAL_PIXIE_MOVE_EASE_IN_OUT_ELASTIC, INTERNAL_PIXIE_MOVE_EASE_IN_OUT_EXPO, 
    INTERNAL_PIXIE_MOVE_EASE_IN_OUT_CUBIC, INTERNAL_PIXIE_MOVE_EASE_IN_OUT_QUART, INTERNAL_PIXIE_MOVE_EASE_IN_OUT_QUINT,
    INTERNAL_PIXIE_MOVE_EASE_IN_OUT_CIRCLE, 
    INTERNAL_PIXIE_MOVECOUNT
} internal_pixie_move_type_t;


typedef struct internal_pixie_move_command_t {
    i32 type;
    i32 target;
    i32 duration;
    u32 hash;
} internal_pixie_move_command_t;


typedef struct internal_pixie_move_t {
    internal_pixie_move_type_t type;
    int target;
    int duration;
} internal_pixie_move_t;


// Main engine state - *everything* is stored here, and data is accessed from both the app thread and the user thread,
// with various mutexes being used to limit concurrent access where necessary. The instance is created within the `run` 
// function, and a pointer to it is stored in thread local storage for the user thread, so that every API method can 
// access it to perform its function. The app thread gets a pointer to it through the user_data parameter to the 
// internal_pixie_app_proc.

typedef enum internal_pixie_sprite_type_t { TYPE_NONE, TYPE_SPRITE, TYPE_LABEL, } internal_pixie_sprite_type_t;


typedef struct internal_pixie_sprite_moves_t {
    int count;
    internal_pixie_move_t moves[ 16 ];
    int index;
    int time;
    int start;
    int loop;
} internal_pixie_sprite_moves_t;


typedef struct internal_pixie_sprite_t {
    int x;
    int y;
    int origin_x;
    int origin_y;
    int visible;

    internal_pixie_sprite_type_t type;
            
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

    internal_pixie_sprite_moves_t move_x;
    internal_pixie_sprite_moves_t move_y;

} internal_pixie_sprite_t;


typedef struct internal_pixie_user_thread_data_t {
    struct {
        int state[ KEYCOUNT ];
        int prev[ KEYCOUNT ];
    } keyboard;

    struct {
        int fullscreen;
        int crt_mode;
    } window;

    struct { 
        u32 palette[ 256 ];
        int screen_width;
        int screen_height;
        int border_width;
        int border_height;
        u8* pixels;
    } screen;

    struct {
        int sprite_count;
        internal_pixie_sprite_t* sprites;
    } sprites;

} internal_pixie_user_thread_data_t;


typedef struct internal_pixie_t {
    thread_atomic_ptr_t acquired_instance;
    thread_mutex_t instance_mutex;

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

    // Assets are loaded through the use of a memory mapped file, mapping to an asset bundle file. The file contains
    // all assets of the game in a ready-to-use format, so they can be used directly from the memory mapping. There is
    // no load operation done, that will be handles by the OS as the data is referenced.
    // TODO: background thread which touch all parts of the mapped memory to preload all data
    // TODO: Mutex to protect concurrent access when initializing
    struct {
        mmap_t* bundle; // Memory mapped file containing all assets
        char build_time[ 64 ];
        int count; // Total number of assets
        struct {
            int id; // The id as given in the enum defined by the user through the ASSET_... macros
            u32 crc; // Checksum of the source data this asset was built from
            int offset; // Offset, in bytes from the start of the bundle, to this asset
            int size; // Size, in bytes, of the asset
        }* assets; // Index of all assets in the bundle
    } assets;


    internal_pixie_user_thread_data_t user_thread;

    struct {
        struct {
            int state[ KEYCOUNT ];
            int prev[ KEYCOUNT ];
        } keyboard;

        struct { 
            u32* xbgr;
        } screen;

        internal_pixie_user_thread_data_t copy_of_user_thread;

    } app_thread;

    struct {
        int sound_buffer_size;
        i16* mix_buffers;

        thread_mutex_t song_mutex;
        tsf* sound_font;
        struct mid_t current_song;
    } audio;

    #ifndef PIXIE_NO_BUILD
        struct {
            int count;
            struct {
                char name[ 64 ];
                asset_build_function_t func;
            } types[ 256 ];
        } build;
    #endif

} internal_pixie_t;


// A global atomic pointer to the TLS instance for storing per-thread `internal_pixie_t` pointers. Created in the user
// thread `internal_pixie_user_thread` unless it has already been created (through a compare-and-swap). The user thread
// then sets the TLS value on the instance.

static thread_atomic_ptr_t g_internal_pixie_tls = { NULL }; 


// Retrieves the pointer to the `internal_pixie_t` state for the current thread, as stored in the `g_internal_pixie_tls` 
// TLS storage. As this has to be called in every API function, it was also a convenient place to check if the app 
// thread is requesting a shutdown of the program, and in that case do a forced exit (using `longjmp`) no matter how 
// deep down a call stack `internal_pixie_instance` was called from.

static internal_pixie_t* internal_pixie_instance( void ) { 
    // Get the `internal_pixie_t` pointer for this thread from the global TLS instance `g_internal_pixie_tls`
    internal_pixie_t*  pixie = (internal_pixie_t*) thread_tls_get( thread_atomic_ptr_load( &g_internal_pixie_tls ) );

    // Check if app thread is requesting a forced exit (the user closed the window) and if so, call the exit point
    int force_exit = thread_atomic_int_load( &pixie->exit.force_exit );
    if( force_exit ) longjmp( pixie->exit.exit_jump, force_exit );

    return pixie; 
}


static internal_pixie_t* internal_pixie_acquire( void ) { 
    // Get the `internal_pixie_t` pointer for this thread from the global TLS instance `g_internal_pixie_tls`
    internal_pixie_t*  pixie = (internal_pixie_t*) thread_tls_get( thread_atomic_ptr_load( &g_internal_pixie_tls ) );

    void* instance = thread_atomic_ptr_compare_and_swap( &pixie->acquired_instance, pixie, NULL );
    if( !instance ) {
        thread_mutex_lock( &pixie->instance_mutex );
        instance = thread_atomic_ptr_compare_and_swap( &pixie->acquired_instance, pixie, NULL );
        thread_mutex_unlock( &pixie->instance_mutex );
    }

    // Check if app thread is requesting a forced exit (the user closed the window) and if so, call the exit point
    int force_exit = thread_atomic_int_load( &pixie->exit.force_exit );
    if( force_exit ) longjmp( pixie->exit.exit_jump, force_exit );

    return (internal_pixie_t*) instance; 
}


static void internal_pixie_release( internal_pixie_t* instance ) { 
    // Get the `internal_pixie_t` pointer for this thread from the global TLS instance `g_internal_pixie_tls`
    internal_pixie_t*  pixie = (internal_pixie_t*) thread_tls_get( thread_atomic_ptr_load( &g_internal_pixie_tls ) );

    thread_atomic_ptr_compare_and_swap( &pixie->acquired_instance, NULL, instance );
}


// Create the instance for holding the main engine state. Called from `run` before app thread is started.

static internal_pixie_t* internal_pixie_create( int sound_buffer_size ) {
    // Allocate the state and clear it, to avoid uninitialized varible problems
    internal_pixie_t* pixie = (internal_pixie_t*) malloc( sizeof( internal_pixie_t ) );
    memset( pixie, 0, sizeof( *pixie ) );

    thread_atomic_ptr_store( &pixie->acquired_instance, pixie );
    thread_mutex_init( &pixie->instance_mutex );

    // Set up `exit` field. The `exit_jump` field is initialized from the `run` function at the desired point
    thread_atomic_int_store( &pixie->exit.force_exit, 0 ); 

    // Set up `vbl` field
    thread_signal_init( &pixie->vbl.signal );
    thread_atomic_int_store( &pixie->vbl.count, 0 );


    // Set up window
    int const initial_fullscreen = 1;
    int const initial_crt_mode = 1;
    pixie->user_thread.window.fullscreen = initial_fullscreen;
    pixie->user_thread.window.crt_mode = initial_crt_mode;
    pixie->app_thread.copy_of_user_thread.window.fullscreen = initial_fullscreen;
    pixie->app_thread.copy_of_user_thread.window.crt_mode = initial_crt_mode;

    // Set up the screen 

    size_t palette_size = sizeof( u32 ) * 256;
    memcpy( pixie->user_thread.screen.palette, default_palette(), palette_size );
    memcpy( pixie->app_thread.copy_of_user_thread.screen.palette, default_palette(), palette_size );
    
    int const initial_screen_width = 320;
    int const initial_screen_height = 200;
    int const initial_border_width = 32;
    int const initial_border_height = 44;
    int const full_width = initial_screen_width + initial_border_width * 2;
    int const full_height = initial_screen_height + initial_border_height * 2;

    pixie->user_thread.screen.screen_width = initial_screen_width;
    pixie->user_thread.screen.screen_height = initial_screen_height;
    pixie->user_thread.screen.border_width = initial_border_width;
    pixie->user_thread.screen.border_height = initial_border_height;

    pixie->app_thread.copy_of_user_thread.screen.screen_width = initial_screen_width;
    pixie->app_thread.copy_of_user_thread.screen.screen_height = initial_screen_height;
    pixie->app_thread.copy_of_user_thread.screen.border_width = initial_border_width;
    pixie->app_thread.copy_of_user_thread.screen.border_height = initial_border_height;

    size_t pixels_size = sizeof( u8 ) * initial_screen_width * initial_screen_height;

    pixie->user_thread.screen.pixels = (u8*) malloc( pixels_size );
    memset( pixie->user_thread.screen.pixels, 0, pixels_size );

    pixie->app_thread.copy_of_user_thread.screen.pixels = (u8*) malloc( pixels_size );
    memset( pixie->app_thread.copy_of_user_thread.screen.pixels, 0, pixels_size );

    size_t xbgr_size = sizeof( u32 ) * full_width * full_height;
    pixie->app_thread.screen.xbgr = (u32*) malloc( xbgr_size );
    memset( pixie->app_thread.screen.xbgr, 0, xbgr_size );


    // Set up sprites

    int const initial_sprite_count = 256;
    pixie->user_thread.sprites.sprite_count = initial_sprite_count;
    pixie->app_thread.copy_of_user_thread.sprites.sprite_count = initial_sprite_count;

    size_t sprites_size = sizeof( *pixie->user_thread.sprites.sprites ) * initial_sprite_count;

    pixie->user_thread.sprites.sprites = VOID_CAST( malloc( sprites_size ) );
    memset( pixie->user_thread.sprites.sprites, 0, sprites_size );

    pixie->app_thread.copy_of_user_thread.sprites.sprites = VOID_CAST( malloc( sprites_size ) );
    memset( pixie->app_thread.copy_of_user_thread.sprites.sprites, 0, sprites_size );


    // Set up audio
    
    pixie->audio.sound_buffer_size = sound_buffer_size ;
    int const mix_buffer_count = 6; // 6 buffers (song, speech + 4 sounds);
    pixie->audio.mix_buffers = (i16*) malloc( sizeof( i16 ) * sound_buffer_size * 2 * mix_buffer_count ); 
    thread_mutex_init( &pixie->audio.song_mutex );

    int soundfont_size = 0;
    u8 const* soundfont = default_soundfont( &soundfont_size );
    pixie->audio.sound_font = tsf_load_memory( soundfont, soundfont_size );
    tsf_channel_set_bank_preset( pixie->audio.sound_font, 9, 128, 0);
    tsf_set_output( pixie->audio.sound_font, TSF_STEREO_INTERLEAVED, 44100, 0.0f );

    return pixie;
}


// Destroy the specified pixie instance. Called by `run` function, after app thread has finished.

static void internal_pixie_destroy( internal_pixie_t* pixie ) {
    // Cleanup `vbl` field
    thread_signal_term( &pixie->vbl.signal );

    // Cleanup screen
    free( pixie->app_thread.screen.xbgr );

    free( pixie->app_thread.copy_of_user_thread.screen.pixels );
    free( pixie->user_thread.screen.pixels );

    
    // Cleanup sprites

    for( int i = 0; i < pixie->user_thread.sprites.sprite_count; ++i ) {
        if( pixie->user_thread.sprites.sprites[ i ].type == TYPE_LABEL ) {
            if( pixie->user_thread.sprites.sprites[ i ].data.label.text ) {
                free( pixie->user_thread.sprites.sprites[ i ].data.label.text );
            }
        }
    }
    free( pixie->user_thread.sprites.sprites );

    for( int i = 0; i < pixie->app_thread.copy_of_user_thread.sprites.sprite_count; ++i ) {
        if( pixie->app_thread.copy_of_user_thread.sprites.sprites[ i ].type == TYPE_LABEL ) {
            if( pixie->app_thread.copy_of_user_thread.sprites.sprites[ i ].data.label.text ) {
                free( pixie->app_thread.copy_of_user_thread.sprites.sprites[ i ].data.label.text );
            }
        }
    }
    free( pixie->app_thread.copy_of_user_thread.sprites.sprites );


    // Cleanup audio
    thread_mutex_term( &pixie->audio.song_mutex );
    free( pixie->audio.mix_buffers );
    tsf_close( pixie->audio.sound_font );

    if( pixie->assets.bundle ) {
        mmap_close( pixie->assets.bundle );
    }

    thread_mutex_term( &pixie->instance_mutex );

    free( pixie );
}


// Retrieves pointer to and size of the specified asset

static void const* internal_pixie_find_asset( internal_pixie_t* pixie, int id, int* size ) {
    if( id < 0 || id >= pixie->assets.count ) {
        if( size ) *size = 0;
        return NULL;
    }

    if( size ) *size = pixie->assets.assets[ id ].size;
    return (void*)( ( (uintptr_t) mmap_data( pixie->assets.bundle ) ) + pixie->assets.assets[ id ].offset );    
}


// When the window is closed, this is called (from app thread) to signal that the program should exit. The `force_exit`
// flag is checked in the `internal_pixie_instance` function, which is called from every public API function.

static void internal_pixie_force_exit( internal_pixie_t* pixie ) {
    thread_atomic_int_store( &pixie->exit.force_exit, INT_MAX ); // INT_MAX is used to signal forced exit
            
    // In case user thread is in `wait_vbl` loop, exit it immediately so we don't have to wait for it to timeout
    thread_atomic_int_inc( &pixie->vbl.count );
    thread_signal_raise( &pixie->vbl.signal );    
}


static keys_t internal_pixie_key_from_app_key( app_key_t key ) {
	int index = (int) key;
	if( key < 0 || key >= APP_KEYCOUNT ) return KEY_INVALID;
	
	int map[ APP_KEYCOUNT * 2 ] = { 
		APP_KEY_INVALID, KEY_INVALID, APP_KEY_LBUTTON, KEY_LBUTTON, APP_KEY_RBUTTON, KEY_RBUTTON, APP_KEY_CANCEL, 
        KEY_CANCEL, APP_KEY_MBUTTON, KEY_MBUTTON, APP_KEY_XBUTTON1, KEY_XBUTTON1, APP_KEY_XBUTTON2, KEY_XBUTTON2, 
        APP_KEY_BACK, KEY_BACK, APP_KEY_TAB, KEY_TAB, APP_KEY_CLEAR, KEY_CLEAR, APP_KEY_RETURN, KEY_RETURN, 
        APP_KEY_SHIFT, KEY_SHIFT, APP_KEY_CONTROL, KEY_CONTROL, APP_KEY_MENU, KEY_MENU, APP_KEY_PAUSE, KEY_PAUSE, 
        APP_KEY_CAPITAL, KEY_CAPITAL, APP_KEY_KANA, KEY_KANA, APP_KEY_JUNJA, KEY_JUNJA, APP_KEY_FINAL, KEY_FINAL, 
        APP_KEY_HANJA, KEY_HANJA, APP_KEY_ESCAPE, KEY_ESCAPE, APP_KEY_CONVERT, KEY_CONVERT, APP_KEY_NONCONVERT, 
        KEY_NONCONVERT, APP_KEY_ACCEPT, KEY_ACCEPT, APP_KEY_MODECHANGE, KEY_MODECHANGE, APP_KEY_SPACE, KEY_SPACE, 
        APP_KEY_PRIOR, KEY_PRIOR, APP_KEY_NEXT, KEY_NEXT, APP_KEY_END, KEY_END, APP_KEY_HOME, KEY_HOME, APP_KEY_LEFT, 
		KEY_LEFT, APP_KEY_UP, KEY_UP, APP_KEY_RIGHT, KEY_RIGHT, APP_KEY_DOWN, KEY_DOWN, APP_KEY_SELECT, KEY_SELECT, 
        APP_KEY_PRINT, KEY_PRINT, APP_KEY_EXEC, KEY_EXEC, APP_KEY_SNAPSHOT, KEY_SNAPSHOT, APP_KEY_INSERT, KEY_INSERT,
		APP_KEY_DELETE, KEY_DELETE, APP_KEY_HELP, KEY_HELP, APP_KEY_0, KEY_0, APP_KEY_1, KEY_1, APP_KEY_2, KEY_2, 
        APP_KEY_3, KEY_3, APP_KEY_4, KEY_4, APP_KEY_5, KEY_5, APP_KEY_6, KEY_6, APP_KEY_7, KEY_7, APP_KEY_8, KEY_8, 
        APP_KEY_9, KEY_9, APP_KEY_A, KEY_A, APP_KEY_B, KEY_B, APP_KEY_C, KEY_C, APP_KEY_D, KEY_D, APP_KEY_E, KEY_E, 
        APP_KEY_F, KEY_F, APP_KEY_G, KEY_G, APP_KEY_H, KEY_H,APP_KEY_I, KEY_I, APP_KEY_J, KEY_J, APP_KEY_K, KEY_K, 
        APP_KEY_L, KEY_L, APP_KEY_M, KEY_M, APP_KEY_N, KEY_N, APP_KEY_O, KEY_O,APP_KEY_P, KEY_P, APP_KEY_Q, KEY_Q, 
        APP_KEY_R, KEY_R, APP_KEY_S, KEY_S, APP_KEY_T, KEY_T, APP_KEY_U, KEY_U, APP_KEY_V, KEY_V, APP_KEY_W, KEY_W, 
        APP_KEY_X, KEY_X, APP_KEY_Y, KEY_Y, APP_KEY_Z, KEY_Z, APP_KEY_LWIN, KEY_LWIN, APP_KEY_RWIN, KEY_RWIN, 
        APP_KEY_APPS, KEY_APPS, APP_KEY_SLEEP, KEY_SLEEP, APP_KEY_NUMPAD0, KEY_NUMPAD0, APP_KEY_NUMPAD1, KEY_NUMPAD1, 
        APP_KEY_NUMPAD2, KEY_NUMPAD2, APP_KEY_NUMPAD3, KEY_NUMPAD3, APP_KEY_NUMPAD4, KEY_NUMPAD4, APP_KEY_NUMPAD5, 
        KEY_NUMPAD5, APP_KEY_NUMPAD6, KEY_NUMPAD6, APP_KEY_NUMPAD7, KEY_NUMPAD7, APP_KEY_NUMPAD8, KEY_NUMPAD8, 
        APP_KEY_NUMPAD9, KEY_NUMPAD9, APP_KEY_MULTIPLY, KEY_MULTIPLY, APP_KEY_ADD, KEY_ADD, APP_KEY_SEPARATOR, 
        KEY_SEPARATOR, APP_KEY_SUBTRACT, KEY_SUBTRACT, APP_KEY_DECIMAL, KEY_DECIMAL, APP_KEY_DIVIDE, KEY_DIVIDE, 
        APP_KEY_F1, KEY_F1, APP_KEY_F2, KEY_F2, APP_KEY_F3, KEY_F3, APP_KEY_F4, KEY_F4, APP_KEY_F5, KEY_F5, APP_KEY_F6, 
        KEY_F6, APP_KEY_F7, KEY_F7, APP_KEY_F8, KEY_F8, APP_KEY_F9, KEY_F9, APP_KEY_F10, KEY_F10, APP_KEY_F11, KEY_F11, 
        APP_KEY_F12, KEY_F12, APP_KEY_F13, KEY_F13, APP_KEY_F14, KEY_F14, APP_KEY_F15, KEY_F15, APP_KEY_F16, KEY_F16, 
        APP_KEY_F17, KEY_F17, APP_KEY_F18, KEY_F18, APP_KEY_F19, KEY_F19, APP_KEY_F20, KEY_F20, APP_KEY_F21, KEY_F21, 
		APP_KEY_F22, KEY_F22, APP_KEY_F23, KEY_F23, APP_KEY_F24, KEY_F24, APP_KEY_NUMLOCK, KEY_NUMLOCK, APP_KEY_SCROLL, 
        KEY_SCROLL, APP_KEY_LSHIFT, KEY_LSHIFT, APP_KEY_RSHIFT, KEY_RSHIFT, APP_KEY_LCONTROL, KEY_LCONTROL, 
        APP_KEY_RCONTROL, KEY_RCONTROL, APP_KEY_LMENU, KEY_LMENU, APP_KEY_RMENU, KEY_RMENU, APP_KEY_BROWSER_BACK, 
        KEY_BROWSER_BACK, APP_KEY_BROWSER_FORWARD, KEY_BROWSER_FORWARD, APP_KEY_BROWSER_REFRESH, KEY_BROWSER_REFRESH, 
        APP_KEY_BROWSER_STOP, KEY_BROWSER_STOP, APP_KEY_BROWSER_SEARCH, KEY_BROWSER_SEARCH, APP_KEY_BROWSER_FAVORITES, 
        KEY_BROWSER_FAVORITES, APP_KEY_BROWSER_HOME, KEY_BROWSER_HOME, APP_KEY_VOLUME_MUTE, KEY_VOLUME_MUTE, 
        APP_KEY_VOLUME_DOWN, KEY_VOLUME_DOWN, APP_KEY_VOLUME_UP, KEY_VOLUME_UP, APP_KEY_MEDIA_NEXT_TRACK, 
        KEY_MEDIA_NEXT_TRACK, APP_KEY_MEDIA_PREV_TRACK, KEY_MEDIA_PREV_TRACK, APP_KEY_MEDIA_STOP, KEY_MEDIA_STOP, 
        APP_KEY_MEDIA_PLAY_PAUSE, KEY_MEDIA_PLAY_PAUSE, APP_KEY_LAUNCH_MAIL, KEY_LAUNCH_MAIL, 
        APP_KEY_LAUNCH_MEDIA_SELECT, KEY_LAUNCH_MEDIA_SELECT, APP_KEY_LAUNCH_APP1, KEY_LAUNCH_APP1, 
        APP_KEY_LAUNCH_APP2, KEY_LAUNCH_APP2, APP_KEY_OEM_1, KEY_OEM_1, APP_KEY_OEM_PLUS, KEY_OEM_PLUS, 
        APP_KEY_OEM_COMMA, KEY_OEM_COMMA, APP_KEY_OEM_MINUS, KEY_OEM_MINUS, APP_KEY_OEM_PERIOD, KEY_OEM_PERIOD, 
        APP_KEY_OEM_2, KEY_OEM_2, APP_KEY_OEM_3, KEY_OEM_3, APP_KEY_OEM_4, KEY_OEM_4,APP_KEY_OEM_5, KEY_OEM_5, 
        APP_KEY_OEM_6, KEY_OEM_6, APP_KEY_OEM_7, KEY_OEM_7, APP_KEY_OEM_8, KEY_OEM_8, APP_KEY_OEM_102, KEY_OEM_102, 
        APP_KEY_PROCESSKEY, KEY_PROCESSKEY, APP_KEY_ATTN, KEY_ATTN, APP_KEY_CRSEL, KEY_CRSEL, APP_KEY_EXSEL, KEY_EXSEL, 
        APP_KEY_EREOF, KEY_EREOF, APP_KEY_PLAY, KEY_PLAY, APP_KEY_ZOOM, KEY_ZOOM, APP_KEY_NONAME, KEY_NONAME, 
        APP_KEY_PA1, KEY_PA1, APP_KEY_OEM_CLEAR, KEY_OEM_CLEAR, 
    };

	ASSERT( map[ index * 2 ] == key, "Invalid mapping from app_key to pixie key" );
	if( map[ index * 2 ] != key ) return KEY_INVALID;

	return (keys_t) map[ index * 2 + 1 ];
}


static void internal_pixie_update_input( internal_pixie_t* pixie, app_input_event_t* events, int count ) {
    int const keyboard_array_size = 
        sizeof( pixie->app_thread.keyboard.state ) / sizeof( *pixie->app_thread.keyboard.state );
    
    int const prev_array_size = 
        sizeof( pixie->app_thread.keyboard.prev ) / sizeof( *pixie->app_thread.keyboard.prev );
    (void) prev_array_size;
	ASSERT( keyboard_array_size == prev_array_size, "Key states size mismatch" );

	for( int i = 0; i < keyboard_array_size; ++i ) {
		pixie->app_thread.keyboard.prev[ i ] = pixie->app_thread.keyboard.state[ i ];
    }

	for( int i = 0; i < count; ++i ) {
		app_input_event_t* event = &events[ i ];
		switch( event->type ) {
			case APP_INPUT_KEY_DOWN: {
				keys_t key = internal_pixie_key_from_app_key( event->data.key );
				if( key >= 0 && key < keyboard_array_size ) {
					pixie->app_thread.keyboard.state[ key ] = 1;                          
                }
			} break;
			case APP_INPUT_KEY_UP: {
				keys_t key = internal_pixie_key_from_app_key( event->data.key );
				if( key >= 0 && key < keyboard_array_size ) {
					pixie->app_thread.keyboard.state[ key ] = 0;                          
                }
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


void internal_pixie_copy_user_thread_data( internal_pixie_user_thread_data_t* dest, 
    internal_pixie_user_thread_data_t* source ) {

    dest->window = source->window;
    
    ASSERT( sizeof( source->screen.palette ) == sizeof( dest->screen.palette ), "Palette size mismatch" );
    memcpy( dest->screen.palette, source->screen.palette, sizeof( dest->screen.palette ) );
    
    dest->screen.screen_width = source->screen.screen_width;
    dest->screen.screen_height = source->screen.screen_height;
    dest->screen.border_width = source->screen.border_width;
    dest->screen.border_height = source->screen.border_height;

    size_t dest_pixels_size = sizeof( u8 ) * dest->screen.screen_width * dest->screen.screen_height;
    size_t source_pixels_size = sizeof( u8 ) * source->screen.screen_width * source->screen.screen_height;
    if( dest_pixels_size != source_pixels_size ) {
        free( dest->screen.pixels );
        dest->screen.pixels = (u8*) malloc( source_pixels_size );
    }
    memcpy( dest->screen.pixels, source->screen.pixels, source_pixels_size );

    for( int i = 0; i < source->sprites.sprite_count; ++i ) {
        dest->sprites.sprites[ i ].x = source->sprites.sprites[ i ].x;
        dest->sprites.sprites[ i ].y = source->sprites.sprites[ i ].y;
        dest->sprites.sprites[ i ].origin_x = source->sprites.sprites[ i ].origin_x;
        dest->sprites.sprites[ i ].origin_y = source->sprites.sprites[ i ].origin_y;
        dest->sprites.sprites[ i ].visible = source->sprites.sprites[ i ].visible;
        switch( source->sprites.sprites[ i ].type ) {
            case TYPE_NONE: {
            } break;
            case TYPE_SPRITE: {
                if( dest->sprites.sprites[ i ].type == TYPE_LABEL && dest->sprites.sprites[ i ].data.label.text ) {
                    free( dest->sprites.sprites[ i ].data.label.text );
                    dest->sprites.sprites[ i ].data.label.text = 0;
                }
                dest->sprites.sprites[ i ].data.sprite.asset = source->sprites.sprites[ i ].data.sprite.asset;
                dest->sprites.sprites[ i ].data.sprite.cel = source->sprites.sprites[ i ].data.sprite.cel;
            } break;
            case TYPE_LABEL: {
                char const* dest_text = dest->sprites.sprites[ i ].type == TYPE_LABEL ? 
                    dest->sprites.sprites[ i ].data.label.text : NULL;

                size_t src_len = strlen( source->sprites.sprites[ i ].data.label.text );

                if( !dest_text || strlen( dest_text ) < src_len ) {
                    if( dest->sprites.sprites[ i ].data.label.text ) {
                        free( dest->sprites.sprites[ i ].data.label.text );
                    }
                    dest->sprites.sprites[ i ].data.label.text = (char*) malloc( src_len + 1 );
                }
                memcpy( dest->sprites.sprites[ i ].data.label.text, source->sprites.sprites[ i ].data.label.text, 
                    src_len + 1 );

                dest->sprites.sprites[ i ].data.label.font = source->sprites.sprites[ i ].data.label.font;
                dest->sprites.sprites[ i ].data.label.align = source->sprites.sprites[ i ].data.label.align;
                dest->sprites.sprites[ i ].data.label.color = source->sprites.sprites[ i ].data.label.color;
                dest->sprites.sprites[ i ].data.label.outline = source->sprites.sprites[ i ].data.label.outline;
                dest->sprites.sprites[ i ].data.label.shadow = source->sprites.sprites[ i ].data.label.shadow;
                dest->sprites.sprites[ i ].data.label.wrap = source->sprites.sprites[ i ].data.label.wrap;
            } break;
        }
        dest->sprites.sprites[ i ].type = source->sprites.sprites[ i ].type;

        // Intentionally not copying `move_x` and `move_y` data, as it is not used by app thread
    }
}


void internal_pixie_update_sprite_movement( int* value, internal_pixie_sprite_moves_t* moves ) {
    static float (* const easefuncs[])( float ) = { 
        ease_linear, ease_linear, ease_linear, ease_linear, ease_smoothstep, ease_smootherstep, ease_out_quad, 
        ease_out_back, ease_out_bounce, ease_out_sine, ease_out_elastic, ease_out_expo, ease_out_cubic, ease_out_quart, 
        ease_out_quint, ease_out_circle, ease_in_quad, ease_in_back, ease_in_bounce, ease_in_sine, ease_in_elastic, 
        ease_in_expo,  ease_in_cubic, ease_in_quart, ease_in_quint, ease_in_circle, ease_in_out_quad, 
        ease_in_out_back, ease_in_out_bounce, ease_in_out_sine, ease_in_out_elastic, ease_in_out_expo, 
        ease_in_out_cubic, ease_in_out_quart, ease_in_out_quint, ease_in_out_circle,
    };

    if( moves->count > 0 ) {
        ++moves->time;
        internal_pixie_move_t* move = &moves->moves[ moves->index ];
        if( moves->time <= move->duration ) {
            float range = (float)( move->target - moves->start );
            float current = ( (float) moves->time ) / (float) move->duration;
            if( move->type >= INTERNAL_PIXIE_MOVE_LINEAR ) {
                float t = easefuncs[ move->type ]( current );
                int pos = moves->start + (int)( t * range );
                *value = pos;
            }
        } else {
            ++moves->index;
            if( moves->index >= moves->count ) {
                if( moves->loop ) {
                    moves->index = 0;
                } else {
                    moves->count = 0;
                }
            } else {
                moves->time = 0;
                if( move->type >= INTERNAL_PIXIE_MOVE_LINEAR ) {
                    *value = move->target;
                }
                moves->start = *value;
            }
        }
    }
}


void internal_pixie_render_sprite( internal_pixie_t* pixie, internal_pixie_user_thread_data_t* data, 
    internal_pixie_sprite_t* sprite ) {

    if( !sprite->visible ) return;

    if( sprite->type == TYPE_SPRITE ) {
        int asset = sprite->data.sprite.asset;
        if( asset < 1 || asset > pixie->assets.count ) return;
        --asset;
    
        int cel = sprite->data.sprite.cel;
        // Find the sprite data in the memory mapped file
        u8* frames = (u8*) internal_pixie_find_asset( pixie, asset, NULL );
        int frame_count = *(int*)frames;
        if( frame_count > 0 && cel >= 0 ) {
            int* offsets = (int*)(frames + sizeof( int ) );
            palrle_data_t* rledata = (palrle_data_t*)( frames + offsets[ cel % frame_count ] );

            // Render pixels
            palrle_blit( rledata, sprite->x - sprite->origin_x, 
                sprite->y  - sprite->origin_y, data->screen.pixels, 
                data->screen.screen_width, data->screen.screen_height );
        }

    // Render labels
    } else if( sprite->type == TYPE_LABEL ) {
        int asset = sprite->data.label.font;
        if( asset < 1 || asset > pixie->assets.count ) return;
        --asset;

        // Find the font data in the memory mapped file
        pixelfont_t const* font = VOID_CAST( internal_pixie_find_asset( pixie, asset, NULL ) );
        pixelfont_align_t pixelfont_align = PIXELFONT_ALIGN_LEFT;
        if( sprite->data.label.align == TEXT_ALIGN_CENTER ) {
            pixelfont_align = PIXELFONT_ALIGN_CENTER;
        } else if( sprite->data.label.align == TEXT_ALIGN_RIGHT ) {
            pixelfont_align = PIXELFONT_ALIGN_RIGHT;
        }

        int wrap = sprite->data.label.wrap;
        int outline = sprite->data.label.outline;

        int shadow = sprite->data.label.shadow;
        int shadow_offset_x = 1;
        int shadow_offset_y = 1;
        if( shadow >= 0 && shadow < 256 ) {
            if( outline >= 0 && outline < 256 ) {
			    for( int y = -1; y <= 1; ++y ) for( int x = -1; x <= 1; ++x ) {
				    if( x == 0 && y == 0 ) continue;

	                pixelfont_blit_u8( font, 
                        sprite->x + shadow_offset_x + x - sprite->origin_x, 
                        sprite->y + shadow_offset_y + y - sprite->origin_y, 
                        sprite->data.label.text, (u8) shadow, data->screen.pixels, 
                        data->screen.screen_width, data->screen.screen_height, pixelfont_align, wrap, 0, 0, -1,
                        PIXELFONT_BOLD_OFF, PIXELFONT_ITALIC_OFF, PIXELFONT_UNDERLINE_OFF, NULL );
                }
            } else {
	            pixelfont_blit_u8( font, 
                    sprite->x + shadow_offset_x - sprite->origin_x, 
                    sprite->y + shadow_offset_y - sprite->origin_y, 
                    sprite->data.label.text, (u8) shadow, data->screen.pixels, 
                    data->screen.screen_width, data->screen.screen_height, pixelfont_align, wrap, 0, 0, -1, 
                    PIXELFONT_BOLD_OFF, PIXELFONT_ITALIC_OFF, PIXELFONT_UNDERLINE_OFF,  NULL );
            }
        }

        if( outline >= 0 && outline < 256 ) {
			for( int y = -1; y <= 1; ++y ) for( int x = -1; x <= 1; ++x ) {
				if( x == 0 && y == 0 ) continue;

	            pixelfont_blit_u8( font, 
                    sprite->x + x - sprite->origin_x, 
                    sprite->y + y - sprite->origin_y, 
                    sprite->data.label.text, (u8) outline, data->screen.pixels, 
                    data->screen.screen_width, data->screen.screen_height, pixelfont_align, wrap, 0, 0, -1, 
                    PIXELFONT_BOLD_OFF, PIXELFONT_ITALIC_OFF, PIXELFONT_UNDERLINE_OFF, NULL );
            }
        }

        int color = sprite->data.label.color;
        if( color >= 0 && color < 256 ) {
	        pixelfont_blit_u8( font, 
                sprite->x - sprite->origin_x, 
                sprite->y - sprite->origin_y, 
                sprite->data.label.text, (u8) color, data->screen.pixels, 
                data->screen.screen_width, data->screen.screen_height, pixelfont_align, wrap, 0, 0, -1, 
                PIXELFONT_BOLD_OFF, PIXELFONT_ITALIC_OFF, PIXELFONT_UNDERLINE_OFF, NULL );
        }
    }
}


// Render all sprites and convert the screen from palettized to 24-bit XBGR

static u32* internal_pixie_frame_update( internal_pixie_t* pixie, int* out_width, int* out_height, int* out_fullscreen, 
    int* out_crt_mode ) {

    // Block user thread while we access data
    thread_mutex_lock( &pixie->instance_mutex );
    void* instance = thread_atomic_ptr_compare_and_swap( &pixie->acquired_instance, pixie, NULL );
    while( !instance ) {
        thread_yield();
        instance = thread_atomic_ptr_compare_and_swap( &pixie->acquired_instance, pixie, NULL );
    }

    internal_pixie_user_thread_data_t* data_user = &pixie->user_thread;
    internal_pixie_user_thread_data_t* data_copy = &pixie->app_thread.copy_of_user_thread;

    // Copy keyboard state to user thread
    ASSERT( sizeof( data_user->keyboard ) == sizeof( pixie->app_thread.keyboard ), "Keyboard struct mismatch" );
    memcpy( &data_user->keyboard, &pixie->app_thread.keyboard, sizeof( data_user->keyboard ) );

    // Update sprite movement
    for( int i = 0; i < data_user->sprites.sprite_count; ++i ) {    
        internal_pixie_sprite_t* sprite = &data_user->sprites.sprites[ i ];
        internal_pixie_update_sprite_movement( &sprite->x, &sprite->move_x );
        internal_pixie_update_sprite_movement( &sprite->y, &sprite->move_y );
    }

    // Copy user thread data to app thread
    internal_pixie_copy_user_thread_data( data_copy, data_user );
    data_user = NULL; // We should not touch user data after this point, only the copy

    // Release lock - we now have all data we need
    thread_atomic_ptr_compare_and_swap( &pixie->acquired_instance, NULL, instance );
    thread_mutex_unlock( &pixie->instance_mutex );

    // Signal to the game that the frame is completed, and that we are just starting the next one
    thread_atomic_int_inc( &pixie->vbl.count );
    thread_signal_raise( &pixie->vbl.signal );    


    // Update and render
    if( out_fullscreen ) *out_fullscreen = data_copy->window.fullscreen;
    if( out_crt_mode ) *out_crt_mode = data_copy->window.crt_mode;

    // Render sprites
    for( int i = 0; i < data_copy->sprites.sprite_count; ++i ) {    
        internal_pixie_render_sprite( pixie, data_copy, &data_copy->sprites.sprites[ i ] );       
    }


    // Convert palette based screen composite to 24-bit XBGR. Both `xbgr` and `composite` are only used from here
    int screen_width = data_copy->screen.screen_width;
    int screen_height = data_copy->screen.screen_height;
    int border_width = data_copy->screen.border_width;
    int border_height = data_copy->screen.border_height;
    int full_width = screen_width + border_width * 2;
    int full_height = screen_height + border_height * 2;

    for( int y = 0; y < screen_height; ++y ) {
        for( int x = 0; x < screen_width; ++x ) {
            pixie->app_thread.screen.xbgr[ x + border_width + ( y + border_height ) * full_width ] = 
                    data_copy->screen.palette[ data_copy->screen.pixels[ x + y * screen_width ] ];
        }
    }

    if( out_width ) *out_width = full_width;
    if( out_height ) *out_height = full_height;
    return pixie->app_thread.screen.xbgr;
    }


// Called by audio thread (via `internal_pixie_app_sound_callback`) when it needs new audio samples

static void internal_pixie_render_samples( internal_pixie_t* pixie, i16* sample_pairs, int sample_pairs_count )
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
        sample = sample > 32767 ? 32767 : sample < -32727 ? -32727 : sample; // TODO: soft clip?
        sample_pairs[ i ] = (i16) sample;
        }
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

int internal_pixie_load_bundle( char const* filename, char const* time, char const* definitions, int count ) {
    internal_pixie_t* pixie = internal_pixie_instance(); // Get `internal_pixie_t` instance from thread local storage

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
        ( ( strlen( filename ) < 4 || strcmp( filename + strlen( filename ) - 4, ".tmp" ) != 0 ) && strcmp( header->bundle_file, filename ) != 0 ) ||  
        ( definitions && strcmp( header->definitions_file, definitions ) != 0 ) ||  
        ( time && strcmp( header->build_time, time ) != 0 ) ||  
        ( count > 0 && header->assets_count != count ) ) {
            mmap_close( bundle );
            return EXIT_FAILURE;
    }

    // Check that the size of all files match the size of the bundle, and that IDs and offsets are as expected.
    struct { int id; u32 crc; int offset; int size; }* assets = VOID_CAST( (void*)( header + 1 ) );
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
    strcpy( pixie->assets.build_time, header->build_time );
    pixie->assets.count = header->assets_count;
    pixie->assets.assets = VOID_CAST( (void*) assets );

    return EXIT_SUCCESS;
}



/*
------------------
    USER THREAD
------------------
*/


// Holds the parameters passed to `run`, so they can be forwarded through `internal_pixie_app_proc` to the user thread
// `internal_pixie_user_thread` where the user supplied `main` function will be called with the provided arguments.

struct internal_pixie_run_context_t {
    int (*main)( int, char** );
    int argc;
    char** argv;
};


// Holds the data needed for communicating between user thread and app thread. The instance of this is defined and
// initialized in `internal_pixie_app_proc`. The field `out_pixie` is filled out by the user thread and picked up by 
// `internal_pixie_app_proc` after the `user_thread_initialized` signal has been raised.

struct internal_pixie_user_thread_context_t {
    struct internal_pixie_run_context_t* run_context; // The user supplied main function and arguments
    int sound_buffer_size; // The size of the streaming sound buffer, defined in `internal_pixie_app_proc`
    thread_signal_t user_thread_initialized; // Signals that user thread is running and pixie instance has been created
    thread_atomic_int_t user_thread_finished; // Flags that user thread is done executing and that app loop should stop
    thread_signal_t app_loop_finished; // Signals that app loop has finished, and it is safe to destroy pixie instance
    internal_pixie_t* out_pixie; // Communicates the pixie instance back to `internal_pixie_app_proc`
};


// Entry point for the user thread, which is started from `internal_pixie_app_proc` on the app thread. Creates the
// pixie instance and stores a pointer to it in thread local storage `g_internal_pixie_tls`. Once this is finished,
// it stores the pixie instance pointer in the `out_pixie` field of the user thread context, and raises the signal
// `user_thread_initialized` which the app thread starts waiting for as soon as it has started the user thread.
// Then `main` is invoked, and the user code starts running. When it is finished (controlled or forced exit) the
// `user_thread_finished` atomic is set, and then the user thread waits for the app thread to raise the signal
// `app_loop_finished`, to let the user thread know it is safe to destroy the pixie instance.

static int internal_pixie_user_thread( void* user_data ) {
    struct internal_pixie_user_thread_context_t* context = (struct internal_pixie_user_thread_context_t*) user_data;
        
    // Create and initialize the main engine state used by all of pixie
    internal_pixie_t* pixie = internal_pixie_create( context->sound_buffer_size );

    // A pointer to the `internal_pixie_t` instance needs to be stored in thread local storage, and before we do, we 
    // must create the TLS instance. But only if it is not already created, and as there's nothing stopping a user from 
    // calling the `run` function from two parallel threads at the same time, we must avoid a race condition. We do 
    // this by always creating a new TLS instance, and then we do a compare-and-swap with the global instance, to only 
    // swap if it is not already set. If the swap does not return NULL (thus indicating it already held a value), then 
    // we destroy the TLS instance we just created, as it will no longer be needed (we'll have to use the global one 
    // instead).
    thread_tls_t pixie_tls = thread_tls_create();
    if( thread_atomic_ptr_compare_and_swap( &g_internal_pixie_tls, NULL, pixie_tls ) ) thread_tls_destroy( pixie_tls );
    
    // Store the `internal_pixie_t` pointer in the thread local storage for the current thread. It will be retrieved by 
    // all API functions so that we don't have to pass around an instance parameter to them.
    thread_tls_set( thread_atomic_ptr_load( &g_internal_pixie_tls ), pixie );
  
    // Signal to the `internal_pixie_app_proc` function on the app thread that user thread initialization is complete, 
    // and the pixie instance is created, so the app thread may enter its main loop.
    context->out_pixie = pixie;
    thread_signal_raise( &context->user_thread_initialized );

    // Set up the jump target for the `end` function
    #pragma warning( push )
    #pragma warning( disable: 4611 ) // interaction between '_setjmp' and C++ object destruction is non-portable
    int jumpres = setjmp( pixie->exit.exit_jump );
    #pragma warning( pop )

    // Run the user provided entry point (will be `pixmain` unless `PIXIE_NO_MAIN` was defined)
    int result = EXIT_SUCCESS;
    if( jumpres == 0 ) // First time we get here we call `main`. If `exit_jump` was jumped to we will get here again
        result = context->run_context->main( context->run_context->argc, context->run_context->argv );
    else // Second time we save the result (`INT_MAX` is mapped to 0, as a jumpres of 0 would call main again)
        result = ( result == INT_MAX ? EXIT_SUCCESS : result );

    // Signal to the app thread that user thread has completed its execution, and it should exit its main loop
    thread_atomic_int_store( &context->user_thread_finished, 1 );
    
    // Wait until app thread have completed its main loop, so that is is safe to destroy the pixie instance
    thread_signal_wait( &context->app_loop_finished, 5000 ); // Time out after 5 seconds - something's gone very wrong

    // Destroy pixie instance, and clear the thread local pointer to it
    internal_pixie_destroy( pixie );
    thread_tls_set( thread_atomic_ptr_load( &g_internal_pixie_tls ), NULL );

    return result;
}


/*
-----------------
    APP THREAD
-----------------
*/

// Audio playback is started by `internal_pixie_app_proc`, and works with a streaming sound buffer. Every time the 
// stream have played enough to need more samples, it calls this callback function, which just pass the call on to 
// `internal_pixie_render_samples` which renders all currently playing sounds and mix all samples together for the 
// sound buffer.

void internal_pixie_app_sound_callback( APP_S16* sample_pairs, int sample_pairs_count, void* user_data ) {
    internal_pixie_t* pixie = (internal_pixie_t*) user_data;
    internal_pixie_render_samples( pixie, sample_pairs, sample_pairs_count ); 
}


// Main body for the app (main) thread (invoked by calling `app_run` from the public API `run` function). The app 
// thread starts the user thread, running `internal_pixie_user_thread`, which runs independently from the app thread.
// The app thread handles the main window, rendering, audio and input. After performing all initialization, and the
// user thread is started, it waits for the `user_thread_initialized` signal to be raised on the user thread, 
// signalling that the user thread is done and the pixie instance has been created. 
// Every iteration through the main loop, the signal `vbl.signal` is raised (as part of internal_pixie_frame_update), 
// and the `vbl.count` value is incremented. These are used by the `wait_vbl` function to pause the user thread until 
// the start of the next frame. If the main window is closed (by clicking on the close button), the app thread sets the 
// `force_exit` value to `INT_MAX`, to signal to the user thread that it should exit the user code and terminate. The 
// `force_exit` value is checked in the `internal_pixie_instance` function, which is called at the start of every API 
// call.

static int internal_pixie_app_proc( app_t* app, void* user_data ) {
    struct internal_pixie_run_context_t* run_context = (struct internal_pixie_run_context_t*) user_data;

    int const SOUND_BUFFER_SIZE = 735 * 3; // Three frames worth of sound buffering

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

    // Set up the shared data between user thread and app thread
    struct internal_pixie_user_thread_context_t user_thread_context;
    user_thread_context.run_context = run_context;
    user_thread_context.sound_buffer_size = SOUND_BUFFER_SIZE;
    thread_signal_init( &user_thread_context.user_thread_initialized );
    thread_atomic_int_store( &user_thread_context.user_thread_finished, 0 );
    thread_signal_init( &user_thread_context.app_loop_finished );
    user_thread_context.out_pixie = NULL;

    // Start the user thread. The entry point `internal_pixie_user_thread` will create the pixie instance and invoke
    // the user supplied `main` function, which defaults to `pixmain` unless PIXIE_NO_MAIN is defined
    thread_ptr_t user_thread = thread_create( internal_pixie_user_thread, &user_thread_context, 
        THREAD_STACK_SIZE_DEFAULT );

    // Wait up to 5 seconds for user thread - if it takes longer than this, everything has gone wrong so just bail
    if( !thread_signal_wait( &user_thread_context.user_thread_initialized, 5000 ) ) {
        thread_signal_raise( &user_thread_context.app_loop_finished );
        thread_signal_term( &user_thread_context.user_thread_initialized );
        thread_signal_term( &user_thread_context.app_loop_finished );
        crtemu_destroy( crtemu );
        return EXIT_FAILURE;
    }    

    
    internal_pixie_t* pixie = user_thread_context.out_pixie;

    // Start sound playback
    app_sound( app, SOUND_BUFFER_SIZE * 2, internal_pixie_app_sound_callback, pixie );

    // Create the frametimer instance, and set it to fixed 60hz update. This will ensure we never run faster than that,
    // even if the user have disabled vsync in their graphics card settings.
    frametimer_t* frametimer = frametimer_create( NULL );
    frametimer_lock_rate( frametimer, 60 );

    // Main loop
    APP_U64 prev_time = app_time_count( app );       
    while( !thread_atomic_int_load( &user_thread_context.user_thread_finished ) ) {
       
        // Run app update (reading inputs etc)
        app_state_t app_state = app_yield( app );
        
        app_input_t input = app_input( app );
        internal_pixie_update_input( pixie, input.events, input.count );

        // Check if the close button on the window was clicked (or Alt+F4 was pressed)
        if( app_state == APP_STATE_EXIT_REQUESTED ) {
            // Signal that we need to force the user thread to exit
            internal_pixie_force_exit( pixie );
            break; 
        }

        // Render screen buffer
        int pixie_fullscreen = fullscreen;
        int pixie_crt_mode = crt_mode;
        int screen_width = 0;
        int screen_height = 0;
        APP_U32* xbgr = internal_pixie_frame_update( pixie, &screen_width, &screen_height, &pixie_fullscreen, 
            &pixie_crt_mode );
    
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

    // Signal to the user thread that app loop has terminated, so it can destroy the pixie instance and exit
    thread_signal_raise( &user_thread_context.app_loop_finished );

    // Cleanup and exit
    int result = thread_join( user_thread );
    thread_signal_term( &user_thread_context.user_thread_initialized );
    thread_signal_term( &user_thread_context.app_loop_finished );
    frametimer_destroy( frametimer );
    crtemu_destroy( crtemu );

    return result;
}



/*
---------------------
    API FUNCTIONS
---------------------
*/


// The main starting point for a Pixie program. Called automatically from the standard C main function defined below,
// unless `PIXIE_NO_MAIN` has been defined, in which case the user will have to define their own `main` function and
// call `run` from it, passing in their pixie main function to it. 


// The `run` function creates the `internal_pixie_t` 
// engine state and stores a pointer to it in thread local storage. It creates the app thread, and waits for it to be 
// initialized before calling the users pixie main function (called `pixmain` if the built-in main is used). It also 
// uses `setjmp` to define a jump point in order to be able to exit from the user code no matter where in the call 
// stack it is, and still get back to the `run` function to perform a controlled shutdown.

int run( int (*main)( int, char** ), int argc, char** argv ) {
    struct internal_pixie_run_context_t run_context;
    run_context.main = main;
    run_context.argc = argc;
    run_context.argv = argv;
    return app_run( internal_pixie_app_proc, &run_context, NULL, NULL, NULL );
}


// Terminates the user code, performing a controlled shutdown of the engine. Can be called from deep down a call stack
// as it will use `longjmp` to branch back to the top level of the user thread and perform a controlled exit.
// Note that any resources allocated manually outside of Pixie will leak unless manually released before calling `end`.

void end( int return_code ) {
    internal_pixie_t* pixie = internal_pixie_instance(); // Get `internal_pixie_t` instance from thread local storage
   
    // Jump back into the middle of the `run` function, to where `setjmp` was called, regardless of where we called from
    longjmp( pixie->exit.exit_jump, return_code == 0 ? INT_MAX : return_code ); 
}


// Waits until the start of the next frame. 

void wait_vbl( void ) {
    internal_pixie_t* pixie = internal_pixie_instance(); // Get `internal_pixie_t` instance from thread local storage

    // Get the vbl count before we start - we want to wait until it has changed
    int current_vbl_count = thread_atomic_int_load( &pixie->vbl.count );

    // Since signals might suffer spurious wakeups, we want to loop until we get a new vbl count
    while( current_vbl_count == thread_atomic_int_load( &pixie->vbl.count ) ) {
        // Wait until app thread says there is a new frame, or timeout after one second.
        thread_signal_wait( &pixie->vbl.signal, 1000 );

        // Call `internal_pixie_instance` again, to trigger the check for `force_exit`, so we can terminate if need be
        internal_pixie_instance();
    }
}


void wait( int jiffys ) {
    for( int i = 0; i < jiffys; ++i ) {
        wait_vbl();
    }
}


int fullscreen( void ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    int ret = pixie->user_thread.window.fullscreen;
    
    internal_pixie_release( pixie ); 
    return ret;
}


void fullscreen_on( void ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    pixie->user_thread.window.fullscreen = 1;

    internal_pixie_release( pixie ); 
}



void fullscreen_off( void ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    pixie->user_thread.window.fullscreen = 0;

    internal_pixie_release( pixie ); 
}



int crt_mode( void ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    int ret = pixie->user_thread.window.crt_mode;

    internal_pixie_release( pixie ); 
    return ret;
}



void crt_mode_on( void ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    pixie->user_thread.window.crt_mode = 1;

    internal_pixie_release( pixie ); 
}



void crt_mode_off( void ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    pixie->user_thread.window.crt_mode = 0;

    internal_pixie_release( pixie ); 
}



// Prints the specified string to the screen using the default font.

void print( char const* str ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    // Very placeholder font rendering
    static int x = 32;
    static int y = 44;
    while( *str ) {
        unsigned long long chr = default_font()[ (u8) *str++ ];
        for( int iy = 0; iy < 8; ++iy )
            for( int ix = 0; ix < 8; ++ix )
                if( chr & ( 1ull << ( ix + iy * 8 ) ) )
                    pixie->user_thread.screen.pixels[ x + ix + ( y + iy ) * pixie->user_thread.screen.screen_width ] = 10; 
        x += 8;
        if( x - 32 >= 320 ) {
            x = 32;
            y += 8;
        }
    }
    x = 32;
    y += 8;

    internal_pixie_release( pixie ); 
}


// Apply palette from file to the global palette

void load_palette( int asset ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    int size = 0;
    void const* data = internal_pixie_find_asset( pixie, asset, &size );
    if( size == sizeof( pixie->user_thread.screen.palette ) )
        memcpy( pixie->user_thread.screen.palette, data, sizeof( pixie->user_thread.screen.palette ) );

    internal_pixie_release( pixie ); 
}


void setcol( int index, rgb_t rgb ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( index < 0 || index >= 256 ) return;
    u32 r = (u32)( rgb.r < 0 ? 0 : rgb.r > 255 ? 255 : rgb.r );
    u32 g = (u32)( rgb.g < 0 ? 0 : rgb.g > 255 ? 255 : rgb.g );
    u32 b = (u32)( rgb.b < 0 ? 0 : rgb.b > 255 ? 255 : rgb.b );
    pixie->user_thread.screen.palette[ index ] = ( b << 16 ) | ( g << 8 ) | r;

    internal_pixie_release( pixie ); 
}


rgb_t getcol( int index ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( index < 0 || index >= 256 ) {
        rgb_t rgb = { 0, 0, 0 };
        internal_pixie_release( pixie ); 
        return rgb;
    }

    u32 color = pixie->user_thread.screen.palette[ index ];
    rgb_t rgb = { (int)( color & 0xff ), (int)( ( color >> 8 ) & 0xff ), (int)( ( color >> 16 ) & 0xff ) };

    internal_pixie_release( pixie ); 
    return rgb;
}


void sprites_off( void ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    for( int i = 0; i < pixie->user_thread.sprites.sprite_count; ++i ) {
        pixie->user_thread.sprites.sprites[ i ].move_x.count = 0;
        pixie->user_thread.sprites.sprites[ i ].move_y.count = 0;
        if( pixie->user_thread.sprites.sprites[ i ].type == TYPE_LABEL ) {
            if( pixie->user_thread.sprites.sprites[ i ].data.label.text ) {
                free( pixie->user_thread.sprites.sprites[ i ].data.label.text );
            }
        }
        pixie->user_thread.sprites.sprites[ i ].type = TYPE_NONE;
    }


    internal_pixie_release( pixie ); 
}


// Assign a bitmap to a sprite, and give it a position

int sprite( int spr_index, int x, int y, int asset ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage
    
    if( asset < 0 || asset >= pixie->assets.count ) {
        internal_pixie_release( pixie );
        return 0;
    }

    // TODO: check that asset is sprite

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }
    
    --spr_index;
    if( pixie->user_thread.sprites.sprites[ spr_index ].type == TYPE_LABEL ) {
        if( !pixie->user_thread.sprites.sprites[ spr_index ].data.label.text ) {
            free( pixie->user_thread.sprites.sprites[ spr_index ].data.label.text );
        }
    }
    pixie->user_thread.sprites.sprites[ spr_index ].type = TYPE_SPRITE;
    pixie->user_thread.sprites.sprites[ spr_index ].data.sprite.asset = asset + 1;
    pixie->user_thread.sprites.sprites[ spr_index ].x = x;
    pixie->user_thread.sprites.sprites[ spr_index ].y = y;
    pixie->user_thread.sprites.sprites[ spr_index ].origin_x = 0;
    pixie->user_thread.sprites.sprites[ spr_index ].origin_y = 0;
    pixie->user_thread.sprites.sprites[ spr_index ].visible = 1;

    internal_pixie_release( pixie );
    return spr_index + 1;
}


void sprite_bitmap( int spr_index, int asset ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( asset < 0 || asset >= pixie->assets.count ) {
        internal_pixie_release( pixie );
        return;
    }

    // TODO: check that asset is sprite

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
    }

    --spr_index;

    if( pixie->user_thread.sprites.sprites[ spr_index ].type != TYPE_SPRITE ) {
        internal_pixie_release( pixie );
        return;
    }

    pixie->user_thread.sprites.sprites[ spr_index ].data.sprite.asset = asset + 1;
    internal_pixie_release( pixie );
}


int sprite_visible( int spr_index ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }

    --spr_index;
    int visible = pixie->user_thread.sprites.sprites[ spr_index ].visible;
    internal_pixie_release( pixie );
    return visible;
}


void sprite_show( int spr_index ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return;
    }

    --spr_index;

    pixie->user_thread.sprites.sprites[ spr_index ].visible = 1;
    internal_pixie_release( pixie );
}


void sprite_hide( int spr_index ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return;
    }

    --spr_index;

    pixie->user_thread.sprites.sprites[ spr_index ].visible = 0;
    internal_pixie_release( pixie );
}


// Update sprite position without changing bitmap

void sprite_pos( int spr_index, int x, int y ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
    }

    --spr_index;
    pixie->user_thread.sprites.sprites[ spr_index ].x = x;
    pixie->user_thread.sprites.sprites[ spr_index ].y = y;
    internal_pixie_release( pixie );
}


int sprite_x( int spr_index ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }

    --spr_index;
    int x = pixie->user_thread.sprites.sprites[ spr_index ].x;
    internal_pixie_release( pixie );
    return x;
}


int sprite_y( int spr_index ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }

    --spr_index;
    int y = pixie->user_thread.sprites.sprites[ spr_index ].y;
    internal_pixie_release( pixie );
    return y;
}


void sprite_origin( int spr_index, int x, int y ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
    }

    --spr_index;
    pixie->user_thread.sprites.sprites[ spr_index ].origin_x = x;
    pixie->user_thread.sprites.sprites[ spr_index ].origin_y = y;
    internal_pixie_release( pixie );
}


int sprite_origin_x( int spr_index ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }

    --spr_index;
    int x = pixie->user_thread.sprites.sprites[ spr_index ].origin_x;
    internal_pixie_release( pixie );
    return x;
}


int sprite_origin_y( int spr_index ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }

    --spr_index;
    int y = pixie->user_thread.sprites.sprites[ spr_index ].origin_y;
    internal_pixie_release( pixie );
    return y;
}


void sprite_cel( int spr_index, int cel ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
    }

    --spr_index;

    if( pixie->user_thread.sprites.sprites[ spr_index ].type != TYPE_SPRITE ) {
        internal_pixie_release( pixie );
        return;
    }

    pixie->user_thread.sprites.sprites[ spr_index ].data.sprite.cel = cel;
    internal_pixie_release( pixie );
}


int label( int spr_index, int x, int y, char const* text, int color, int font_asset ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage
    
    if( font_asset < 0 || font_asset >= pixie->assets.count ) {
        internal_pixie_release( pixie );
        return 0;
    }

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }
    
    --spr_index;
    if( pixie->user_thread.sprites.sprites[ spr_index ].type == TYPE_LABEL ) {
        if( !pixie->user_thread.sprites.sprites[ spr_index ].data.label.text ) {
            free( pixie->user_thread.sprites.sprites[ spr_index ].data.label.text );
        }
    }
    pixie->user_thread.sprites.sprites[ spr_index ].type = TYPE_LABEL;
    pixie->user_thread.sprites.sprites[ spr_index ].data.label.text = strdup( text ? text : "" );
    pixie->user_thread.sprites.sprites[ spr_index ].data.label.font = font_asset + 1;
    pixie->user_thread.sprites.sprites[ spr_index ].data.label.color = color;
    pixie->user_thread.sprites.sprites[ spr_index ].data.label.outline = -1;
    pixie->user_thread.sprites.sprites[ spr_index ].data.label.shadow = -1;
    pixie->user_thread.sprites.sprites[ spr_index ].data.label.wrap = -1;
    pixie->user_thread.sprites.sprites[ spr_index ].data.label.align = TEXT_ALIGN_LEFT;
    pixie->user_thread.sprites.sprites[ spr_index ].x = x;
    pixie->user_thread.sprites.sprites[ spr_index ].y = y;
    pixie->user_thread.sprites.sprites[ spr_index ].origin_x = 0;
    pixie->user_thread.sprites.sprites[ spr_index ].origin_y = 0;
    pixie->user_thread.sprites.sprites[ spr_index ].visible = 1;

    internal_pixie_release( pixie );
    return spr_index + 1;
}


int label_text( int spr_index, char const* text ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage
    
    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }
    
    --spr_index;

    if( pixie->user_thread.sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        internal_pixie_release( pixie );
        return 0;
    }

    if( pixie->user_thread.sprites.sprites[ spr_index ].data.label.text ) {
        free( pixie->user_thread.sprites.sprites[ spr_index ].data.label.text );
    }
    pixie->user_thread.sprites.sprites[ spr_index ].data.label.text = strdup( text ? text : "" );
    internal_pixie_release( pixie );
    return spr_index + 1;
}


int label_align( int spr_index, text_align_t align ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage
    
    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }
    
    --spr_index;

    if( pixie->user_thread.sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        internal_pixie_release( pixie );
        return 0;
    }

    pixie->user_thread.sprites.sprites[ spr_index ].data.label.align = align;

    internal_pixie_release( pixie );
    return spr_index + 1;
}


int label_color( int spr_index, int color ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage
    
    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }
    
    --spr_index;

    if( pixie->user_thread.sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        internal_pixie_release( pixie );
        return 0;
    }

    pixie->user_thread.sprites.sprites[ spr_index ].data.label.color = color;

    internal_pixie_release( pixie );
    return spr_index + 1;
}


int label_outline( int spr_index, int color ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage
    
    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }
    
    --spr_index;

    if( pixie->user_thread.sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        internal_pixie_release( pixie );
        return 0;
    }

    pixie->user_thread.sprites.sprites[ spr_index ].data.label.outline = color;

    internal_pixie_release( pixie );
    return spr_index + 1;
}


int label_shadow( int spr_index, int color ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage
    
    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }
    
    --spr_index;

    if( pixie->user_thread.sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        internal_pixie_release( pixie );
        return 0;
    }

    pixie->user_thread.sprites.sprites[ spr_index ].data.label.shadow = color;

    internal_pixie_release( pixie );
    return spr_index + 1;
}


int label_wrap( int spr_index, int wrap ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage
    
    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
        return 0;
    }
    
    --spr_index;

    if( pixie->user_thread.sprites.sprites[ spr_index ].type != TYPE_LABEL ) {
        internal_pixie_release( pixie );
        return 0;
    }

    pixie->user_thread.sprites.sprites[ spr_index ].data.label.wrap = wrap;

    internal_pixie_release( pixie );
    return spr_index + 1;
}


void set_soundfont( int asset ) {
    internal_pixie_t* pixie = internal_pixie_instance(); // Get `internal_pixie_t` instance from thread local storage

    if( asset < 0 || asset >= pixie->assets.count ) {
        return;
    }

    thread_mutex_lock( &pixie->audio.song_mutex );

    int soundfont_size = 0;
    void const* soundfont = internal_pixie_find_asset( pixie, asset, &soundfont_size );

    tsf_close( pixie->audio.sound_font );
    pixie->audio.sound_font = tsf_load_memory( soundfont, soundfont_size );
    tsf_channel_set_bank_preset( pixie->audio.sound_font, 9, 128, 0 );
    tsf_set_output( pixie->audio.sound_font, TSF_STEREO_INTERLEAVED, 44100, 0.0f );
    thread_mutex_unlock( &pixie->audio.song_mutex );
}


void play_song( int asset ) {
    internal_pixie_t* pixie = internal_pixie_instance(); // Get `internal_pixie_t` instance from thread local storage

    if( asset < 0 || asset >= pixie->assets.count ) {
        return;
    }

    thread_mutex_lock( &pixie->audio.song_mutex );

    memset( &pixie->audio.current_song, 0, sizeof( pixie->audio.current_song ) );

    int mid_size = 0;
    void const* mid_data = internal_pixie_find_asset( pixie, asset, &mid_size );
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
    internal_pixie_t* pixie = internal_pixie_instance(); // Get `internal_pixie_t` instance from thread local storage

    int size = 0;
    void const* data = internal_pixie_find_asset( pixie, asset, &size );

    return (char const*) data;
}


int asset_size( int asset ) {
    internal_pixie_t* pixie = internal_pixie_instance(); // Get `internal_pixie_t` instance from thread local storage

    int size = 0;
    internal_pixie_find_asset( pixie, asset, &size );

    return size;
}


void const* asset_data( int asset ) {
    internal_pixie_t* pixie = internal_pixie_instance(); // Get `internal_pixie_t` instance from thread local storage

    int size = 0;
    return internal_pixie_find_asset( pixie, asset, &size );
}


u32 internal_pixie_move_hash( u8* data, int len ) {
    u32 hash = 0xda442d24U;
    while( --len ) {
        hash = (*data++) + ( hash << 6 ) + ( hash << 16 ) - hash;
    }

    return hash;
}


move_t internal_pixie_make_move( internal_pixie_move_type_t type, int duration, int target ) {
    internal_pixie_move_command_t move;
    move.type = type;
    move.duration = duration;
    move.target = target;
    move.hash = internal_pixie_move_hash( (u8*) &move, sizeof( i32 ) * 3 );
    return *(move_t*)&move;
}


move_t move_loop( void ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_LOOP, 0, 0 ); 
}


move_t move_end( void ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_END, 0, 0 ); 
}


move_t move_delay( int duration ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_DELAY, duration, 0 ); 
}

 
move_t move_linear( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_LINEAR, duration, target ); 
}

  
move_t move_smoothstep( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_SMOOTHSTEP, duration, target ); 
}

   
move_t move_smootherstep( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_SMOOTHERSTEP, duration, target ); 
}

   
move_t move_ease_out_quad( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_OUT_QUAD, duration, target ); 
}

   
move_t move_ease_out_back( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_OUT_BACK, duration, target ); 
}

   
move_t move_ease_out_bounce( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_OUT_BOUNCE, duration, target ); 
}

   
move_t move_ease_out_sine( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_OUT_SINE, duration, target ); 
}

   
move_t move_ease_out_elastic( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_OUT_ELASTIC, duration, target ); 
}

   
move_t move_ease_out_expo( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_OUT_EXPO, duration, target ); 
}

   
move_t move_ease_out_cubic( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_OUT_CUBIC, duration, target ); 
}

   
move_t move_ease_out_quart( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_OUT_QUART, duration, target ); 
}

   
move_t move_ease_out_quint( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_OUT_QUINT, duration, target ); 
}

   
move_t move_ease_out_circle( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_OUT_CIRCLE, duration, target ); 
}

   
move_t move_ease_in_quad( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_QUAD, duration, target ); 
}

   
move_t move_ease_in_back( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_BACK, duration, target ); 
}

   
move_t move_ease_in_bounce( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_BOUNCE, duration, target ); 
}

   
move_t move_ease_in_sine( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_SINE, duration, target ); 
}

   
move_t move_ease_in_elastic( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_ELASTIC, duration, target ); 
}

   
move_t move_ease_in_expo( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_EXPO, duration, target ); 
}

   
move_t move_ease_in_cubic( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_CUBIC, duration, target ); 
}

   
move_t move_ease_in_quart( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_QUART, duration, target ); 
}

   
move_t move_ease_in_quint( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_QUINT, duration, target ); 
}

   
move_t move_ease_in_circle( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_CIRCLE, duration, target ); 
}

   
move_t move_ease_in_out_quad( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_OUT_QUAD, duration, target ); 
}

   
move_t move_ease_in_out_back( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_OUT_BACK, duration, target ); 
}

   
move_t move_ease_in_out_bounce( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_OUT_BOUNCE, duration, target ); 
}

   
move_t move_ease_in_out_sine( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_OUT_SINE, duration, target ); 
}

   
move_t move_ease_in_out_elastic( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_OUT_ELASTIC, duration, target ); 
}

   
move_t move_ease_in_out_expo( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_OUT_EXPO, duration, target ); 
}

   
move_t move_ease_in_out_cubic( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_OUT_CUBIC, duration, target ); 
}

   
move_t move_ease_in_out_quart( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_OUT_QUART, duration, target ); 
}

   
move_t move_ease_in_out_quint( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_OUT_QUINT, duration, target ); 
}

  
move_t move_ease_in_out_circle( int duration, int target ) { 
    return internal_pixie_make_move( INTERNAL_PIXIE_MOVE_EASE_IN_OUT_CIRCLE, duration, target ); 
}

  
void sprite_move_x( int spr_index, move_t moves, ... ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
    }

    --spr_index;

    int max_count = ARRAY_COUNT( pixie->user_thread.sprites.sprites[ spr_index ].move_x.moves );
    int count = 0;
    
    internal_pixie_move_command_t move = *(internal_pixie_move_command_t*)&moves;
    va_list args;
    va_start( args, moves );
    for( ; ; ) {
        ASSERT( move.type >= 0 || move.type < INTERNAL_PIXIE_MOVECOUNT, 
            "Invalid move command. Move command lists must end with a `move_loop` or a `move_end`." );
        if( move.type < 0 || move.type >= INTERNAL_PIXIE_MOVECOUNT || move.type == INTERNAL_PIXIE_MOVE_END ) {
            pixie->user_thread.sprites.sprites[ spr_index ].move_x.loop = 0;
            break;
        }
        
        u32 hash = internal_pixie_move_hash( (u8*) &move, sizeof( i32 ) * 3 );
        ASSERT( hash == move.hash, 
            "Invalid move command. Move command lists must end with a `move_loop` or a `move_end`." );
        if( hash != move.hash ) {
            break;
        }

        if( move.type == INTERNAL_PIXIE_MOVE_LOOP ) {
            pixie->user_thread.sprites.sprites[ spr_index ].move_x.loop = count > 0 ? 1 : 0;
            break;
        }

        pixie->user_thread.sprites.sprites[ spr_index ].move_x.moves[ count ].type = move.type;
        pixie->user_thread.sprites.sprites[ spr_index ].move_x.moves[ count ].duration = move.duration;
        pixie->user_thread.sprites.sprites[ spr_index ].move_x.moves[ count ].target = move.target;

        ++count;
        ASSERTF( count <= max_count, 
            ( "Too many arguments in move command list. A maximum of %d commands supported", max_count ) );
        if( count > max_count ) {
            break;
        }

        move = va_arg( args, internal_pixie_move_command_t );
    }
    va_end( args );

    pixie->user_thread.sprites.sprites[ spr_index ].move_x.count = count;
    pixie->user_thread.sprites.sprites[ spr_index ].move_x.index = 0;
    pixie->user_thread.sprites.sprites[ spr_index ].move_x.time = 0;
    pixie->user_thread.sprites.sprites[ spr_index ].move_x.start = pixie->user_thread.sprites.sprites[ spr_index ].x;

    internal_pixie_release( pixie );
}


void sprite_move_y( int spr_index, move_t moves, ... ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    if( spr_index < 1 || spr_index > pixie->user_thread.sprites.sprite_count ) {
        internal_pixie_release( pixie );
    }

    --spr_index;

    int max_count = ARRAY_COUNT( pixie->user_thread.sprites.sprites[ spr_index ].move_x.moves );
    int count = 0;
    
    internal_pixie_move_command_t move = *(internal_pixie_move_command_t*)&moves;
    va_list args;
    va_start( args, moves );
    for( ; ; ) {
        ASSERT( move.type >= 0 || move.type < INTERNAL_PIXIE_MOVECOUNT, 
            "Invalid move command. Move command lists must end with a `move_loop` or a `move_end`." );
        if( move.type < 0 || move.type >= INTERNAL_PIXIE_MOVECOUNT || move.type == INTERNAL_PIXIE_MOVE_END ) {
            pixie->user_thread.sprites.sprites[ spr_index ].move_y.loop = 0;
            break;
        }
        
        u32 hash = internal_pixie_move_hash( (u8*) &move, sizeof( i32 ) * 3 );
        ASSERT( hash == move.hash, 
            "Invalid move command. Move command lists must end with a `move_loop` or a `move_end`." );
        if( hash != move.hash ) {
            break;
        }

        if( move.type == INTERNAL_PIXIE_MOVE_LOOP ) {
            pixie->user_thread.sprites.sprites[ spr_index ].move_y.loop = count > 0 ? 1 : 0;
            break;
        }

        pixie->user_thread.sprites.sprites[ spr_index ].move_y.moves[ count ].type = move.type;
        pixie->user_thread.sprites.sprites[ spr_index ].move_y.moves[ count ].duration = move.duration;
        pixie->user_thread.sprites.sprites[ spr_index ].move_y.moves[ count ].target = move.target;

        ++count;
        ASSERTF( count <= max_count, 
            ( "Too many arguments in move command list. A maximum of %d commands supported", max_count ) );
        if( count > max_count ) {
            break;
        }

        move = va_arg( args, internal_pixie_move_command_t );
    }
    va_end( args );

    pixie->user_thread.sprites.sprites[ spr_index ].move_y.count = count;
    pixie->user_thread.sprites.sprites[ spr_index ].move_y.index = 0;
    pixie->user_thread.sprites.sprites[ spr_index ].move_y.time = 0;
    pixie->user_thread.sprites.sprites[ spr_index ].move_y.start = pixie->user_thread.sprites.sprites[ spr_index ].y;

    internal_pixie_release( pixie );
}


void text( int x, int y, char const* str, int color, int font_asset 
	/*, text_align align, int wrap_width, int hspacing, int vspacing, int limit, bool bold, bool italic, 
    bool underline */ ) {

    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

    pixelfont_align_t pixelfont_align = PIXELFONT_ALIGN_LEFT;
	//if( align == ALIGNMENT_RIGHT ) pixelfont_align = PIXELFONT_ALIGN_RIGHT;
	//if( align == ALIGNMENT_CENTER ) pixelfont_align = PIXELFONT_ALIGN_CENTER;

	pixelfont_bounds_t pixelfont_bounds;
	pixelfont_bounds.width = 0;
	pixelfont_bounds.height = 0;

    pixelfont_t* pixelfont = (pixelfont_t*) internal_pixie_find_asset( pixie, font_asset, NULL );
	pixelfont_blit_u8( pixelfont, x, y, str, (u8) color, 
        pixie->user_thread.screen.pixels, pixie->user_thread.screen.screen_width, pixie->user_thread.screen.screen_height,
        pixelfont_align, -1, 0, 0, -1, PIXELFONT_BOLD_OFF, PIXELFONT_ITALIC_OFF, PIXELFONT_UNDERLINE_OFF, 
        &pixelfont_bounds );

		//pixelfont_align, wrap_width, hspacing, vspacing, limit, bold ? PIXELFONT_BOLD_ON : PIXELFONT_BOLD_OFF, 
		//italic ? PIXELFONT_ITALIC_ON : PIXELFONT_ITALIC_OFF, 
        // underline ? PIXELFONT_UNDERLINE_ON : PIXELFONT_UNDERLINE_OFF, 
		//bounds ? &pixelfont_bounds : 0 );

/*
	if( bounds )
		{
		bounds->width = pixelfont_bounds.width;
		bounds->height = pixelfont_bounds.height;
		}
*/
    internal_pixie_release( pixie );
}


int key_is_down( keys_t key ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

	if( key < 0 || key >= sizeof( pixie->user_thread.keyboard.state ) / sizeof( *pixie->user_thread.keyboard.state ) ) {
        internal_pixie_release( pixie );
        return 0;
    }

	int ret =  pixie->user_thread.keyboard.state[ key ]; 

    internal_pixie_release( pixie );
    return ret;
}


int key_was_pressed( keys_t key ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

	if( key < 0 || key >= sizeof( pixie->user_thread.keyboard.state ) / sizeof( *pixie->user_thread.keyboard.state ) ) {
        internal_pixie_release( pixie );
        return 0;
    }
	if( key < 0 || key >= sizeof( pixie->user_thread.keyboard.prev ) / sizeof( *pixie->user_thread.keyboard.prev ) ) {
        internal_pixie_release( pixie );
        return 0;
    }
	
    int ret = pixie->user_thread.keyboard.state[ key ] && !pixie->user_thread.keyboard.prev[ key ]; 

    internal_pixie_release( pixie );
    return ret;
}


int key_was_released( keys_t key ) {
    internal_pixie_t* pixie = internal_pixie_acquire(); // Get `internal_pixie_t` instance from thread local storage

	if( key < 0 || key >= sizeof( pixie->user_thread.keyboard.state ) / sizeof( *pixie->user_thread.keyboard.state ) ) {
        internal_pixie_release( pixie );
        return 0;
    }
	if( key < 0 || key >= sizeof( pixie->user_thread.keyboard.prev ) / sizeof( *pixie->user_thread.keyboard.prev ) ) {
        internal_pixie_release( pixie );
        return 0;
    }

	int ret = !pixie->user_thread.keyboard.state[ key ] && pixie->user_thread.keyboard.prev[ key ]; 

    internal_pixie_release( pixie );
    return ret;
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
    ASSERTF( strlen( a.c_str ) + strlen( b.c_str ) < PIXIE_MAX_STRING_LENGTH, (
        "The combined length of string a (length %d) and string b (length %d) is longer than the max length of %d.\n\n"
        "The contents of string a are:\n%s\n\nThe contents of string b are:\n%s", 
        (int) strlen( a.c_str ) + 1, (int) strlen( b.c_str ) + 1, PIXIE_MAX_STRING_LENGTH, a.c_str, b.c_str 
        ) 
    );

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
    #ifdef _WIN32 
        return strnicmp( str.c_str, start.c_str, strlen( start.c_str ) ) == 0;
    #else
        return strncasecmp( str.c_str, start.c_str, strlen( start.c_str ) ) == 0;
    #endif
}
/*

string format( string format_string, ... ) {
}

*/



u32 hash_string( string str ) {
    u32 hash = 5381u; 
    char const* s = str.c_str;
    while( *s ) hash = ( ( hash << 5u ) + hash) ^ *s++;
    return hash;
}


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
        (void) argc, (void) argv;
		#if defined( _WIN32 ) && !defined( __TINYC__ )
            #ifndef NDEBUG
                int flag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ); // Get current flag
                flag ^= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
                _CrtSetDbgFlag( flag ); // Set flag to the new value
                _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG );
                _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
                //_CrtSetBreakAlloc( 0 );
            #endif
        #endif

        return run( pixmain, argc, argv );
    } 

    #ifdef _WIN32
        // pass-through so the program will build with either /SUBSYSTEM:WINDOWS or /SUBSYSTEM:CONSOLE
        struct HINSTANCE__;
        #ifdef __cplusplus
            extern "C" {
        #endif

            int __stdcall WinMain( struct HINSTANCE__* a, struct HINSTANCE__* b, char* c, int d ) { 
                (void) a, (void) b, (void) c, (void) d; 
                return main( __argc, __argv ); 
            }

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
    #ifndef PIXIE_WIN_SDL
        #define APP_WINDOWS
    #else
        #define APP_SDL
    #endif
#else
        #define APP_SDL    
#endif
#define APP_LOG( ctx, level, message )
#include "app.h"

#define CRTEMU_IMPLEMENTATION
#include "crtemu.h"

#define CRT_FRAME_IMPLEMENTATION
#include "crt_frame.h"

#define EASE_IMPLEMENTATION
#if !defined( PIXIE_NO_MATH ) || defined( __TINYC__ )
    #define EASE_ACOS( x ) internal_pixie_acos( x )
    #define EASE_COS( x ) internal_pixie_cos( x )
    #define EASE_POW( x, y ) internal_pixie_pow( x, y )
    #define EASE_SIN( x ) internal_pixie_sin( x )
    #define EASE_SQRT( x ) internal_pixie_sqrt( x )
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
    #define TSF_POW internal_pixie_pow
    #define TSF_POWF internal_pixie_pow
    #define TSF_EXPF internal_pixie_exp
    #define TSF_LOG internal_pixie_log
    #define TSF_TAN internal_pixie_tan
    #define TSF_LOG10 internal_pixie_log10
    #define TSF_SQRT internal_pixie_sqrt
    #define TSF_SQRTF internal_pixie_sqrt
#endif
#include "tsf.h"
#pragma warning( pop )


/*
-------------
    ASSERT
-------------
*/

char const* internal_pixie_format_assert_message( char const* format, ... ) {
	static char buffer[ 4096 ];
	va_list args;
	va_start( args, format );
    #ifdef _WIN32
	_vsnprintf( buffer, sizeof( buffer ), format, args );
    #else
    vsnprintf( buffer, sizeof( buffer ), format, args );
    #endif
	va_end( args );
	return buffer;
}


#ifdef _WIN32
	int internal_pixie_display_assert_message( char const* expression, char const* message, char const* file, int line, 
        char const* function ) {

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
	    int result = MessageBox( 0, buf, "Pixie Assert", 
            MB_ICONERROR | MB_YESNOCANCEL | MB_SYSTEMMODAL | MB_SETFOREGROUND );
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
    /*TODO*/
	int internal_pixie_display_assert_message( char const* expression, char const* message, char const* file, int line, 
        char const* function ) {

	    char buf[ 4096 + 64 ];
	    snprintf( buf, 4095, "ASSERTION FAILED!\n\n%s\n\nExpression: %s\n\nFunction: %s\n\n%s(%d)\n", 
            message, expression, function, file, line );
	    printf( "\n**********************************************************************\n" );
	    printf( "%s", buf );
	    printf( "**********************************************************************\n\n" );
		return 0;
	}
    // #error Platform not supported /* TODO */
#endif /* _WIN32 */



/*
---------------------
    MATH WRAPPERS
---------------------
*/

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
