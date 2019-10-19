/*
------------------------------------------------------------------------------
          Licensing information can be found at the end of the file.
------------------------------------------------------------------------------

mid.h - v0.1 - Midi playback library using the TinySoundFont library.

Do this:
    #define MID_IMPLEMENTATION
before you include this file in *one* C/C++ file to create the implementation.
*/

#ifndef mid_h
#define mid_h

#define _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS
#include <stddef.h>

typedef struct mid_t mid_t;
typedef struct tsf tsf;
    
mid_t* mid_create( void const* midi_data, size_t midi_size, tsf* sound_font, void* memctx );
mid_t* mid_create_from_raw( void const* raw_data, size_t raw_size, tsf* sound_font, void* memctx );

void mid_destroy( mid_t* mid ); 

size_t mid_save_raw( mid_t* mid, void* data, size_t capacity ); 

int mid_render_short( mid_t* mid, short* sample_pairs, int sample_pairs_count );
int mid_render_float( mid_t* mid, float* sample_pairs, int sample_pairs_count );

void mid_skip_leading_silence( mid_t* mid );

#endif /* mid_h */


/*
----------------------
    IMPLEMENTATION
----------------------
*/

#ifdef MID_IMPLEMENTATION
#undef MID_IMPLEMENTATION

#ifndef MID_U8
    #define MID_U8 unsigned char
#endif

#ifndef MID_U16
    #define MID_U16 unsigned short
#endif

#ifndef MID_U32
    #define MID_U32 unsigned int
#endif

#ifndef MID_U64
    #define MID_U64 unsigned long long
#endif

#ifndef MID_MALLOC
    #define _CRT_NONSTDC_NO_DEPRECATE 
    #define _CRT_SECURE_NO_WARNINGS
    #include <stdlib.h>
    #if defined(_cplusplus)
        #define MID_MALLOC( ctx, size ) ( ::malloc( size ) )
        #define MID_FREE( ctx, ptr ) ( ::free( ptr ) )
    #else
        #define MID_MALLOC( ctx, size ) ( malloc( size ) )
        #define MID_FREE( ctx, ptr ) ( free( ptr ) )
    #endif
#endif
#include <assert.h>
#define MID_LOG(...) (void) __VA_ARGS__

#include <string.h>

#pragma warning( push )
#pragma warning( disable: 4242 )
#pragma warning( disable: 4244 )
#pragma warning( disable: 4365 )
#pragma warning( disable: 4668 )
#pragma warning( disable: 4701 )
#pragma warning( disable: 4703 )
#ifndef MID_NO_TSF_IMPLEMENTATION
    #define TSF_NO_STDIO
    #define TSF_IMPLEMENTATION
#endif
#include "tsf.h"
#pragma warning( pop )


typedef struct mid_file_t
    {
    uintptr_t ptr;
    int size;
    } mid_file_t;


unsigned char mid_file_getc( mid_file_t* f )
    {
    if( f->size <= 0 ) return 0;

    unsigned char c = *(unsigned char*) f->ptr;
    ++f->ptr;
    --f->size;
    return c;
    }


int mid_file_read( void* out, int elem_size, int count, mid_file_t* f )
    {
    char* ptr = (char*) out;
    int read_count = 0;
    for( int i = 0; i < elem_size * count; ++i )
        {
        if( f->size <= 0 ) break;
        *ptr++ = (char) mid_file_getc( f );
        ++read_count;
        }
    return read_count;
    }


int mid_file_readvar( mid_file_t* f ) 
    {
    int d;
    d = mid_file_getc(f);
    if( d & 0x80 )
        {
        d &= 0x7f;
        int v;
        do
            {
            v = mid_file_getc(f);
            d = (d << 7) + (v & 0x7f);
            } while (v & 0x80);
        }
    return d;
    }


int mid_file_readdword(mid_file_t * f)
    {
    int x;
    mid_file_read(&x,4,1,f);
    x = (int)((((x) & 0x000000ff) << 24) | (((x) & 0x0000ff00) << 8 ) | (((x) & 0x00ff0000) >> 8 ) | (((x) & 0xff000000) >> 24));
    return x;
    }


int mid_file_readword(mid_file_t * f)
    {
    short int x;
    mid_file_read(&x,2,1,f);
    x = ((((x) & 0xff00) >> 8) | (((x) & 0xff) << 8)); 
    return x;
    }


int mid_file_loadchunkheader(mid_file_t * f, int* length)
    {
    int id;
    id = mid_file_readdword(f);
    *length = mid_file_readdword(f);
    return id;
    }


typedef enum mid_cc_t
    {
    MID_CC_BANK_SELECT_COARSE = 0,
    MID_CC_MODULATION_WHEEL_COARSE = 1,
    MID_CC_BREATH_CONTROLLER_COARSE = 2,
    MID_CC_FOOT_PEDAL_COARSE = 4,
    MID_CC_PORTAMENTO_TIME_COARSE = 5,
    MID_CC_DATA_ENTRY_COARSE = 6,
    MID_CC_VOLUME_COARSE = 7,
    MID_CC_BALANCE_COARSE = 8,
    MID_CC_PAN_POSITION_COARSE = 10,
    MID_CC_EXPRESSION_COARSE = 11,
    MID_CC_EFFECT_CONTROL_1_COARSE = 12,
    MID_CC_EFFECT_CONTROL_2_COARSE = 13,
    MID_CC_GENERAL_PURPOSE_SLIDER_1 = 16,
    MID_CC_GENERAL_PURPOSE_SLIDER_2 = 17,
    MID_CC_GENERAL_PURPOSE_SLIDER_3 = 18,
    MID_CC_GENERAL_PURPOSE_SLIDER_4 = 19,
    MID_CC_BANK_SELECT_FINE = 32,
    MID_CC_MODULATION_WHEEL_FINE = 33,
    MID_CC_BREATH_CONTROLLER_FINE = 34,
    MID_CC_FOOT_PEDAL_FINE = 36,
    MID_CC_PORTAMENTO_TIME_FINE = 37,
    MID_CC_DATA_ENTRY_FINE = 38,
    MID_CC_VOLUME_FINE = 39,
    MID_CC_BALANCE_FINE = 40,
    MID_CC_PAN_POSITION_FINE = 42,
    MID_CC_EXPRESSION_FINE = 43,
    MID_CC_EFFECT_CONTROL_1_FINE = 44,
    MID_CC_EFFECT_CONTROL_2_FINE = 45,
    MID_CC_HOLD_PEDAL_ON_OFF = 64,
    MID_CC_PORTAMENTO_ON_OFF = 65,
    MID_CC_SUSTENUTO_PEDAL_ON_OFF = 66,
    MID_CC_SOFT_PEDAL_ON_OFF = 67,
    MID_CC_LEGATO_PEDAL_ON_OFF = 68,
    MID_CC_HOLD_2_PEDAL_ON_OFF = 69,
    MID_CC_SOUND_VARIATION = 70,
    MID_CC_SOUND_TIMBRE = 71,
    MID_CC_SOUND_RELEASE_TIME = 72,
    MID_CC_SOUND_ATTACK_TIME = 73,
    MID_CC_SOUND_BRIGHTNESS = 74,
    MID_CC_SOUND_CONTROL_6 = 75,
    MID_CC_SOUND_CONTROL_7 = 76,
    MID_CC_SOUND_CONTROL_8 = 77,
    MID_CC_SOUND_CONTROL_9 = 78,
    MID_CC_SOUND_CONTROL_10 = 79,
    MID_CC_GENERAL_PURPOSE_BUTTON_1_ON_OFF = 80,
    MID_CC_GENERAL_PURPOSE_BUTTON_2_ON_OFF = 81,
    MID_CC_GENERAL_PURPOSE_BUTTON_3_ON_OFF = 82,
    MID_CC_GENERAL_PURPOSE_BUTTON_4_ON_OFF = 83,
    MID_CC_EFFECTS_LEVEL = 91,
    MID_CC_TREMULO_LEVEL = 92,
    MID_CC_CHORUS_LEVEL = 93,
    MID_CC_CELESTE_LEVEL = 94,
    MID_CC_PHASER_LEVEL = 95,
    MID_CC_DATA_BUTTON_INCREMENT = 96,
    MID_CC_DATA_BUTTON_DECREMENT = 97,
    MID_CC_NON_REGISTERED_PARAMETER_FINE = 98,
    MID_CC_NON_REGISTERED_PARAMETER_COARSE = 99,
    MID_CC_REGISTERED_PARAMETER_FINE = 100,
    MID_CC_REGISTERED_PARAMETER_COARSE = 101,
    MID_CC_ALL_SOUND_OFF = 120,
    MID_CC_ALL_CONTROLLERS_OFF = 121,
    MID_CC_LOCAL_KEYBOARD_ON_OFF = 122,
    MID_CC_ALL_NOTES_OFF = 123,
    MID_CC_OMNI_MODE_OFF = 124,
    MID_CC_OMNI_MODE_ON = 125,
    MID_CC_MONO_OPERATION = 126,
    MID_CC_POLY_OPERATION = 127,
    } mid_msg_id_t;


typedef enum mid_event_type_t
    {
    MID_EVENT_TYPE_UNKNOWN,
    MID_EVENT_TYPE_BANK,
    MID_EVENT_TYPE_PROGRAM,
    MID_EVENT_TYPE_NOTE_ON,
    MID_EVENT_TYPE_NOTE_OFF,
    MID_EVENT_TYPE_VOLUME,
    MID_EVENT_TYPE_PAN,
    MID_EVENT_TYPE_PITCH_BEND,
    MID_EVENT_TYPE_CC,
    } mid_event_type_t;


typedef struct mid_event_t
    {
    MID_U32 delay_us;
    MID_U8 channel;
    MID_U8 type;
    union 
        {
        struct { MID_U16 index; } bank;
        struct { MID_U8 preset; } program;
        struct { MID_U8 note; MID_U8 velocity; } note_on;
        struct { MID_U8 note; } note_off;
        struct { MID_U16 level; } volume;
        struct { MID_U16 value; } pan;
        struct { MID_U16 value; } pitch_bend;
        struct { MID_U8 data1, data2; } cc;
        } data;
    } mid_event_t;


typedef struct mid_song_t
    {
    int event_count;
    mid_event_t* events;
    } mid_song_t;


struct mid_t
    {
    void* memctx;
    mid_song_t song;
    int percussion_preset;
    MID_U64 playback_accumulated_time_us;
    int playback_sample_pos;
    int playback_event_pos;
    tsf* sound_font;
    };


typedef struct mid_track_data_t
    {
    MID_U64 time_us;
    mid_event_t event;
    } mid_track_data_t;


mid_t* mid_create( void const* midi_data, size_t midi_size, tsf* sound_font, void* memctx )
    {
    mid_file_t mid_file;
    mid_file.ptr = (uintptr_t)midi_data;
    mid_file.size = (int)midi_size;
    mid_file_t* f = &mid_file;

    int len;
    int id = mid_file_loadchunkheader(f,&len);
    if (id != 'MThd')
        {
        // Bad header id
        return NULL;
        }
    if (len < 6)
        {
        // Bad header block length
        return NULL;
        }
    int format = mid_file_readword(f);
    if (format != 1 && format != 0)
        {
        // Unsupported format
        return NULL;
        }
    int number_of_tracks = mid_file_readword(f);

    if( number_of_tracks <= 0 ) return NULL;

    // pulses (clocks) per quarter note
    int ppqn = mid_file_readword(f);
    if (ppqn < 0)
        {
        //("negative ppqn formats not supported\n");
        return NULL;
        }
    if (len > 6)
        {
        while (len > 6)
            {
            mid_file_getc(f);
            len--;
            }
        }

    int track_data_count = 0;
    int track_data_capacity = 1024;
    mid_track_data_t* track_data = (mid_track_data_t*) MID_MALLOC( memctx, sizeof( *track_data ) * track_data_capacity );

    int* track_entry_counts = (int*) MID_MALLOC( memctx, sizeof( int ) * number_of_tracks * 2 );
    int* track_current_merge_pos = track_entry_counts + number_of_tracks;
    memset( track_entry_counts, 0, sizeof( int ) * number_of_tracks * 2 );

    int uspertick = (500000 / ppqn);
    for( int i = 0; i < number_of_tracks; ++i )
        {
        id = mid_file_loadchunkheader(f,&len);
        if (id != 'MTrk')
            {
            // Unknown chunk
            MID_FREE( memctx, track_data );
            MID_FREE( memctx, track_entry_counts );
            return NULL;
            }
        MID_LOG("\nNew track, length %d\n",len);
        int trackend = 0;
        int command = 0;
        MID_U64 time_us = 0;
        MID_U64 prev_time_us = 0;
        int channel = 0;
        while (!trackend)
            {
            int bank_lsb = -1;
            int bank_msb = -1;
            //int volume = ( 127 << 7 ) | 127;
            //int pan = ( ( 127 << 7 ) | 127 ) / 2 + 1;
            MID_U64 dtime = (MID_U64) mid_file_readvar(f);
            MID_U64 dt_us = ( dtime * uspertick );
            time_us += dt_us;
            assert( time_us >= prev_time_us );
            prev_time_us = time_us;
            if( track_data_count >= track_data_capacity )
                {
                track_data_capacity *= 2;
                mid_track_data_t* new_data = (mid_track_data_t*) MID_MALLOC( memctx, sizeof( *new_data ) * track_data_capacity );
                memcpy( new_data, track_data, sizeof( *track_data ) * track_data_count );
                MID_FREE( memctx, track_data );
                track_data = new_data;
                }
            mid_track_data_t* trdat = &track_data[ track_data_count++ ];
            trdat->time_us = time_us;
            mid_event_t* event = &trdat->event;
            event->channel = (MID_U8) channel;
            event->type = MID_EVENT_TYPE_UNKNOWN;
            int data = mid_file_getc(f);
            if (data == 0xff)
                {
                int data1 = mid_file_getc(f); // sub-command
                len = mid_file_readvar(f);
                char str[ 65536 ]; memset( str, 0, sizeof( str ) );
                if( data1 >= 0 && data1 <= 9 )  for( int j = 0; j < len; ++j ) str[ j ] = (char) mid_file_getc(f);
                switch( data1 )
                    {
                    case 1: MID_LOG( "Text: %s\n", str ); break;
                    case 2: MID_LOG( "Copyright: %s\n", str ); break;
                    case 3: MID_LOG( "Track name: %s\n", str ); break;
                    case 4: MID_LOG( "Instrument: %s\n", str ); break;
                    case 5: MID_LOG( "Lyric: %s\n", str ); break;
                    case 6: MID_LOG( "Marker: %s\n", str ); break;
                    case 7: MID_LOG( "Cue point: %s\n", str ); break;
                    case 8: MID_LOG( "Patch name: %s\n", str ); break;
                    case 9: MID_LOG( "Port name: %s\n", str ); break;
                    case 0x2f: trackend = 1; break;
                    case 0x58: // time signature
                        {
                        int nn = mid_file_getc(f);
                        int dd = mid_file_getc(f);
                        int cc = mid_file_getc(f);
                        int bb = mid_file_getc(f);
                        MID_LOG("Time sig: %d:%d, metronome:%d, quarter:%d\n",nn,dd,cc,bb);
                        } break;
                    case 0x59: // key signature
                        {
                        int sf = mid_file_getc(f);
                        int mi = mid_file_getc(f);
                        MID_LOG("Key sig: %d %s, %s\n",abs(sf),sf == 0?"c":(sf < 0 ? "flat":"sharp"), mi?"minor":"major");
                        } break;
                    case 0x51: // tempo
                        {
                        int t = 0;
                        t = mid_file_getc(f) << 16;
                        t |= mid_file_getc(f) << 8;
                        t |= mid_file_getc(f);
                        MID_LOG("Tempo: quarter is %dus (%3.3fs) long - BPM = %3.3f\n",t,t/1000000.0f, 60000000.0f/t);                      
                        uspertick = t / ppqn;
                        } break;
                    case 0x21: { int pp = mid_file_getc(f); MID_LOG("[obsolete] midi port: %d\n",pp); } break;
                    case 0x20: { int cc = mid_file_getc(f); MID_LOG("[obsolete] midi channel: %d\n",cc); 
                        event->type = MID_EVENT_TYPE_BANK;
                        event->data.bank.index = 128;
                        if( track_data_count >= track_data_capacity )
                            {
                            track_data_capacity *= 2;
                            mid_track_data_t* new_data = (mid_track_data_t*) MID_MALLOC( memctx, sizeof( *new_data ) * track_data_capacity );
                            memcpy( new_data, track_data, sizeof( *track_data ) * track_data_count );
                            MID_FREE( memctx, track_data );
                            track_data = new_data;
                            }

                        trdat = &track_data[ track_data_count++ ];
                        trdat->time_us = time_us;
                        event = &trdat->event;
                        event->channel = (MID_U8) i;
                        event->type = MID_EVENT_TYPE_PROGRAM;
                        event->data.program.preset = 0;
                    
                    } break;
                    case 0x54: /* SMPTE offset */ { int hr = mid_file_getc(f); int mn = mid_file_getc(f); int se = mid_file_getc(f); int fr = mid_file_getc(f); int ff = mid_file_getc(f); MID_LOG("SMPTE Offset: %dh %dm %ds %dfr %dff\n",hr,mn,se,fr,ff); } break;
                    case 0x7f: // Proprietary event
                        {
                        MID_LOG("Proprietary event ");
                        while (len) { int d = mid_file_getc(f); MID_LOG("%02X ",d); len--; } MID_LOG("\n");
                        }   
                        break;
                    default:
                        MID_LOG("meta command %02x %d\n", data1, len);
                        while (len) { mid_file_getc(f); len--; }
                    }
                }
            else
                {
                if (data & 0x80) // new command?
                    {
                    command = data;
                    data = mid_file_getc(f);
                    }
                if( ( command & 0xf ) != channel ) channel = command & 0xf;
                switch (command & 0xf0)
                    {
                    case 0x80: // note off
                        {
                        int data2 = mid_file_getc(f); (void) data2;
                        //MID_LOG("Note off: channel %d, Oct %d Note %s Velocity %d\n",command & 0xf, (data/12)-1,note[data%12], data2);
                        event->type = MID_EVENT_TYPE_NOTE_OFF;
                        event->data.note_off.note = (MID_U8) data;
                        } break;
                    case 0x90: // note on
                        {
                        int data2 = mid_file_getc(f); (void) data2;
                        //MID_LOG("Note on: channel %d, Oct %d Note %s Velocity %d\n",command & 0xf, (data/12)-1,note[data%12], data2);

                        event->type = MID_EVENT_TYPE_NOTE_ON;
                        event->data.note_on.note = (MID_U8) data;
                        event->data.note_on.velocity = (MID_U8) data2;
                        } break;
                    case 0xa0: // Note aftertouch
                        {
                        int data2 = mid_file_getc(f); (void) data2;
                        //MID_LOG("Aftertouch: channel %d, Oct %d, Note %s Aftertouch %d\n",command & 0xf, (data/12)-1,note[data%12], data2);
                        } break;
                    case 0xb0: // Controller
                        {
                        int data2 = mid_file_getc(f); (void) data2;
                        event->type = MID_EVENT_TYPE_CC;
                        event->data.cc.data1 = (MID_U8) data;
                        event->data.cc.data2 = (MID_U8) data2;
/*
                        switch( data )
                            {
                            case MID_CC_BANK_SELECT_COARSE:
                                bank_msb = data2;
                                if( bank_lsb >= 0 )
                                    {                                    
                                    event->type = MID_EVENT_TYPE_BANK;
                                    event->data.bank.index = (MID_U16) ( bank_msb << 7 | bank_lsb );
                                    bank_msb = -1;
                                    bank_lsb = -1;
                                    }   
                                break;
                            case MID_CC_BANK_SELECT_FINE:
                                bank_lsb = data2;
                                if( bank_lsb >= 0 )
                                    {                                    
                                    event->type = MID_EVENT_TYPE_BANK;
                                    event->data.bank.index = (MID_U16) ( bank_msb << 7 | bank_lsb );
                                    bank_msb = -1;
                                    bank_lsb = -1;
                                    }   
                                break;

                            case MID_CC_VOLUME_COARSE:
                                {
                                int new_volume = ( volume & 127 ) | ( data2 << 7 );
                                if( new_volume != volume )
                                    {                                    
                                    volume = new_volume;
                                    event->type = MID_EVENT_TYPE_VOLUME;
                                    event->data.volume.level = (MID_U16) ( volume );
                                    }   
                                } break;
                            case MID_CC_VOLUME_FINE:
                                {
                                int new_volume = ( volume & ( 127 << 7 ) ) | ( data2 );
                                if( new_volume != volume )
                                    {                                    
                                    volume = new_volume;
                                    event->type = MID_EVENT_TYPE_VOLUME;
                                    event->data.volume.level = (MID_U16) ( volume );
                                    }   
                                } break;
                            case MID_CC_PAN_POSITION_COARSE:
                                {
                                int new_pan = ( pan & 127 ) | ( data2 << 7 );
                                if( new_pan != pan )
                                    {                                    
                                    pan = new_pan;
                                    event->type = MID_EVENT_TYPE_PAN;
                                    event->data.pan.value = (MID_U16) ( pan );
                                    MID_LOG( "PAN: %d\n", pan );
                                    }   
                                } break;
                            case MID_CC_PAN_POSITION_FINE:
                                {
                                int new_pan = ( pan & ( 127 << 7 ) ) | ( data2 );
                                if( new_pan != pan )
                                    {                                    
                                    pan = new_pan;
                                    event->type = MID_EVENT_TYPE_PAN;
                                    event->data.pan.value = (MID_U16) ( pan );
                                    MID_LOG( "PAN: %d\n", pan );
                                    }   
                                } break;
                            case MID_CC_REGISTERED_PARAMETER_FINE:
                                {
                                if( data2 )
                                    {
                                    int a = data2;
                                    (void) a;
                                    }
                                } break;
                            case MID_CC_REGISTERED_PARAMETER_COARSE:
                                {
                              //  MID_LOG( "REG: %d\n", data2 );
                                if( data2 )
                                    {
                                    int a = data2;
                                    (void) a;
                                    }
                                } break;
                            case MID_CC_DATA_ENTRY_COARSE:
                                {
                               // MID_LOG( "DAT: %d\n", data2 );
                                if( data2 )
                                    {
                                    int a = data2;
                                    (void) a;
                                    }
                                } break;
                            case MID_CC_DATA_ENTRY_FINE:
                                {
                                if( data2 )
                                    {
                                    int a = data2;
                                    (void) a;
                                    }
                                } break;
                            }
                            */
                        } break;
                    case 0xc0: /* program change */ 
                        { 
                        MID_LOG("Program change: channel %d, program %d\n",command & 0xf, data); 
                        if( bank_lsb >= 0 || bank_msb >= 0 )
                            {
                            if( bank_msb <= 0 ) bank_msb = 0;
                            if( bank_lsb <= 0 ) bank_lsb = 0;
                            event->type = MID_EVENT_TYPE_BANK;
                            event->data.bank.index = (MID_U16) ( bank_msb << 7 | bank_lsb );
                            bank_msb = -1;
                            bank_lsb = -1;

                            if( track_data_count >= track_data_capacity )
                                {
                                track_data_capacity *= 2;
                                mid_track_data_t* new_data = (mid_track_data_t*) MID_MALLOC( memctx, sizeof( *new_data ) * track_data_capacity );
                                memcpy( new_data, track_data, sizeof( *track_data ) * track_data_count );
                                MID_FREE( memctx, track_data );
                                track_data = new_data;
                                }

                            trdat = &track_data[ track_data_count++ ];
                            trdat->time_us = time_us;
                            event = &trdat->event;
                            event->channel = (MID_U8) i;
                            event->type = MID_EVENT_TYPE_UNKNOWN;
                            }
                      event->type = MID_EVENT_TYPE_PROGRAM;
                        event->data.program.preset = (MID_U8) data;
                        } break;
                    case 0xd0: /* Channel aftertouch */ { MID_LOG("Channel aftertouch: channel %d, Aftertouch %d\n",command & 0xf, data); } break;
                    case 0xe0: // Pitch bend
                        {
                        int data2 = mid_file_getc(f); (void) data2;
                        event->type = MID_EVENT_TYPE_PITCH_BEND;
                        event->data.pitch_bend.value = (MID_U16)( data + ( data2 << 7 ) );
                        // MID_LOG("Pitchbend: channel %d, Pitch %d\n",command & 0xf, data + (data2 << 7));
                        } break;
                    case 0xf0: // general / immediate
                        {
                        switch (command)
                            {
                            case 0xf0: // SysEx
                                {
                                MID_LOG("SysEx "); while (data != 0xf7) { MID_LOG("%02X ", data); data = mid_file_getc(f); } MID_LOG("\n");
                                // universal sysexes of note:
                                // f0 (05) 7e 7f 09 01 f7 = "general midi enable"
                                // f0 (05) 7e 7f 09 00 f7 = "general midi disable"
                                // f0 (07) 7f 7f 04 01 ll mm f7 = "master volume", ll mm = 14bit value
                                // spec doesn't say that the length byte should be there,
                                // but it appears to be (the ones in brackets)
                                } break;
                            case 0xf1: /* MTC quarter frame */ { int dd = mid_file_getc(f); MID_LOG("MTC quarter frame %d\n",dd); } break;
                            case 0xf2: // Song position pointer
                                {
                                data = mid_file_getc(f);
                                int data2 = mid_file_getc(f);
                                MID_LOG("Song position pointer %d\n", data + (data2 << 7));
                                } break;
                            case 0xf3: /* Song select */ { int song = mid_file_getc(f); MID_LOG("Song select %d\n", song); } break;
                            case 0xf6: /* Tuning request */ MID_LOG("Tuning request\n"); break;
                            case 0xf8: /* MIDI clock */ MID_LOG("MIDI clock\n"); break;
                            case 0xf9: /* MIDI Tick */ MID_LOG("MIDI Tick\n"); break;
                            case 0xfa: /* MIDI start */ MID_LOG("MIDI start\n"); break;
                            case 0xfc: MID_LOG("MIDI stop\n"); break;
                            case 0xfb: MID_LOG("MIDI continue\n"); break;
                            case 0xfe: MID_LOG("Active sense\n"); break;
                            case 0xff: MID_LOG("Reset\n"); break;
                            default: MID_LOG("Unknown: command 0x%02x, data 0x%02x\n", command, data); break;
                            }
                        }
                        break;
                    default:
                        {
                            MID_LOG("Unknown: command 0x%02x, data 0x%02x\n", command, data);
                        }
                        break;
                    }
                }
            if( event->type == MID_EVENT_TYPE_UNKNOWN ) --track_data_count;
            }
        track_entry_counts[ i ] = track_data_count; 
        }

    for( int i = 1; i < number_of_tracks; ++i ) track_current_merge_pos[ i ] = track_entry_counts[ i - 1 ];

    int p = 0;
    int pc = 0;
    for( int i = 0; i < number_of_tracks; ++i )
        {
        int c = track_entry_counts[ i ];
        MID_U64 prev_t = 0;
        for( int j = 0; j < c - pc; ++j )
            {
            MID_U64 t = track_data[ p++ ].time_us;
            assert( t >= prev_t );
            prev_t = t;
            }
        pc = c;
        }

    int events_count = 0;
    int events_capacity = 1024;
    mid_event_t* events = (mid_event_t*) MID_MALLOC( memctx, sizeof( *events ) * events_capacity );

    // merge tracks into one stream
    MID_U64 current_time = 0;
    for( ; ; )
        {
        int lowest_time_track = -1;
        MID_U64 lowest_time_us = 0xffffffffffffffffULL;
        for( int i = 0; i < number_of_tracks; ++i )
            {
            int pos = track_current_merge_pos[ i ];
            if( pos < track_entry_counts[ i ] )
                {
                if( track_data[ pos ].time_us < lowest_time_us )
                    {
                    lowest_time_track = i;
                    lowest_time_us = track_data[ pos ].time_us;
                    }
                }
            }
        if( lowest_time_track < 0 ) break;
        assert( lowest_time_us >= current_time );

        if( events_count >= events_capacity )
            {
            events_capacity *= 2;
            mid_event_t* new_events= (mid_event_t*) MID_MALLOC( memctx, sizeof( *new_events ) * events_capacity );
            memcpy( new_events, events, sizeof( *events ) * events_count );
            MID_FREE( memctx, events );
            events = new_events;
            }
        events[ events_count ] = track_data[ track_current_merge_pos[ lowest_time_track ] ].event;
        events[ events_count ].delay_us = (MID_U32)( lowest_time_us - current_time );
        ++events_count;
        current_time = lowest_time_us;
        ++track_current_merge_pos[ lowest_time_track ];
        }
    
    MID_FREE( memctx, track_entry_counts );
    MID_FREE( memctx, track_data );

    mid_t* mid = (mid_t*) MID_MALLOC( memctx, sizeof( mid_t ) );
    mid->memctx = memctx;
    mid->song.event_count = events_count;
    mid->song.events = events;

    mid->playback_accumulated_time_us = 0ull;
    mid->playback_sample_pos = 0;
    mid->playback_event_pos = 0;

    if( sound_font ) tsf_channel_set_presetnumber( sound_font, 9, 0, 1 ); // drums
    mid->sound_font = sound_font;

    return mid; 
    }


void mid_destroy( mid_t* mid )
    {
    if( mid->song.events ) MID_FREE( mid->memctx, mid->song.events );
    MID_FREE( mid->memctx, mid );
    }


mid_t* mid_create_from_raw( void const* raw_data, size_t raw_size, tsf* sound_font, void* memctx )
    {
    int events_count = *(int*)raw_data;
    if( sizeof( mid_event_t ) * events_count != raw_size - sizeof( int ) ) return NULL;

    mid_event_t* events = (mid_event_t*) MID_MALLOC( memctx, sizeof( *events ) * events_count );
    memcpy( events, ( (int*)raw_data ) + 1, sizeof( *events ) * events_count );

    mid_t* mid = (mid_t*) MID_MALLOC( memctx, sizeof( mid_t ) );
    mid->memctx = memctx;
    mid->song.event_count = events_count;
    mid->song.events = events;

    mid->playback_accumulated_time_us = 0ull;
    mid->playback_sample_pos = 0;
    mid->playback_event_pos = 0;

    if( sound_font ) tsf_channel_set_presetnumber( sound_font, 9, 0, 1 ); // drums
    mid->sound_font = sound_font;

    return mid; 
    }


size_t mid_save_raw( mid_t* mid, void* data, size_t capacity ) 
    {
    size_t size = sizeof( mid_event_t ) * mid->song.event_count + sizeof( int );
    if( data && capacity >= size ) 
        {
        *(int*)data = mid->song.event_count;
        memcpy( ( (int*)data ) + 1, mid->song.events, sizeof( mid_event_t ) * mid->song.event_count );
        }
    return size;
    }


void mid_skip_leading_silence( mid_t* mid )
    {
    for( ; ; )
        {
        MID_U64 next_event_delay_us = mid->song.events[ mid->playback_event_pos ].delay_us;
        MID_U64 playback_time_us = ( mid->playback_sample_pos * 1000000ull ) / 44100ull;
        MID_U64 next_event_time_us = mid->playback_accumulated_time_us + next_event_delay_us;
        assert( next_event_time_us >= playback_time_us );
        MID_U64 time_until_next_event = next_event_time_us - playback_time_us;
        int samples_until_next_event = (int)( ( time_until_next_event * 44100ull ) / 1000000ull );       mid_event_t* event = &mid->song.events[ mid->playback_event_pos ];
        switch( event->type )
            {
            case MID_EVENT_TYPE_PROGRAM:
                if( event->channel == 9 ) 
                    {
                    tsf_channel_set_presetnumber( mid->sound_font, event->channel, event->data.program.preset, 1 );
                    //tsf_channel_set_bank_preset( mid->sound_font, event->channel, 128, 0 );
                    }
                else
                    {
                    tsf_channel_set_bank_preset( mid->sound_font, event->channel, 0, event->data.program.preset );
                    }
                break;
            case MID_EVENT_TYPE_NOTE_ON:
                return;
                break;
            case MID_EVENT_TYPE_NOTE_OFF:
                tsf_channel_note_off( mid->sound_font, event->channel, event->data.note_off.note );
                break;
            case MID_EVENT_TYPE_VOLUME:
                {
                float v = event->data.volume.level / 16384.0f;
                tsf_channel_set_volume( mid->sound_font, event->channel, v * v * v );
                } break;
            case MID_EVENT_TYPE_PAN:
                {
                float p = event->data.pan.value / 16384.0f;
                tsf_channel_set_pan( mid->sound_font, event->channel, p );
                } break;
            case MID_EVENT_TYPE_PITCH_BEND:
                tsf_channel_set_pitchwheel( mid->sound_font, event->channel, event->data.pitch_bend.value );
                break;
            case MID_EVENT_TYPE_CC:
                tsf_channel_midi_control( mid->sound_font, event->channel, event->data.cc.data1, event->data.cc.data2 );
                break;
            }
        mid->playback_sample_pos += samples_until_next_event;
        mid->playback_accumulated_time_us += next_event_delay_us; 
        mid->playback_event_pos++;
        }
    }


int mid_render_short( mid_t* mid, short* sample_pairs, int sample_pairs_count )
    {
    int samples_rendered = 0;
    memset( sample_pairs, 0, sample_pairs_count * sizeof( short ) * 2 );
    while( samples_rendered < sample_pairs_count )
        {
        MID_U64 next_event_delay_us = mid->song.events[ mid->playback_event_pos ].delay_us;
        MID_U64 playback_time_us = ( mid->playback_sample_pos * 1000000ull ) / 44100ull;
        MID_U64 next_event_time_us = mid->playback_accumulated_time_us + next_event_delay_us;
        assert( next_event_time_us >= playback_time_us );
        MID_U64 time_until_next_event = next_event_time_us - playback_time_us;
        int samples_until_next_event = (int)( ( time_until_next_event * 44100ull ) / 1000000ull );
        int samples_to_render = samples_until_next_event;
        if( samples_to_render > sample_pairs_count - samples_rendered )
            {
            samples_to_render = sample_pairs_count - samples_rendered;
            tsf_render_short( mid->sound_font, sample_pairs + samples_rendered * 2, 
                samples_to_render, 1 );
            samples_rendered += samples_to_render;
            mid->playback_sample_pos += samples_to_render;
            return samples_rendered;
            }
        else
            {
            tsf_render_short( mid->sound_font, sample_pairs + samples_rendered * 2, 
                samples_to_render, 1 );
            samples_rendered += samples_to_render;
            mid->playback_sample_pos += samples_to_render;
            }


        mid->playback_accumulated_time_us += next_event_delay_us; 
        mid_event_t* event = &mid->song.events[ mid->playback_event_pos++ ];
        switch( event->type )
            {
            case MID_EVENT_TYPE_PROGRAM:
                if( event->channel == 9 ) 
                    {
                    tsf_channel_set_presetnumber( mid->sound_font, event->channel, event->data.program.preset, 1 );
                    //tsf_channel_set_bank_preset( mid->sound_font, event->channel, 128, 0 );
                    }
                else
                    {
                    tsf_channel_set_bank_preset( mid->sound_font, event->channel, 0, event->data.program.preset );
                    }
                break;
            case MID_EVENT_TYPE_NOTE_ON:
                tsf_channel_note_on( mid->sound_font, event->channel, event->data.note_on.note, event->data.note_on.velocity / 127.0f );
                break;
            case MID_EVENT_TYPE_NOTE_OFF:
                tsf_channel_note_off( mid->sound_font, event->channel, event->data.note_off.note );
                break;
            case MID_EVENT_TYPE_VOLUME:
                {
                float v = event->data.volume.level / 16384.0f;
                tsf_channel_set_volume( mid->sound_font, event->channel, v * v * v );
                } break;
            case MID_EVENT_TYPE_PAN:
                {
                float p = event->data.pan.value / 16384.0f;
                tsf_channel_set_pan( mid->sound_font, event->channel, p );
                } break;
            case MID_EVENT_TYPE_PITCH_BEND:
                tsf_channel_set_pitchwheel( mid->sound_font, event->channel, event->data.pitch_bend.value );
                break;
            case MID_EVENT_TYPE_CC:
                tsf_channel_midi_control( mid->sound_font, event->channel, event->data.cc.data1, event->data.cc.data2 );
                break;
                }
        }
    
    return samples_rendered;
    }


#endif /* MID_IMPLEMENTATION */

/*
------------------------------------------------------------------------------

This software is available under 2 licenses - you may choose the one you like.

------------------------------------------------------------------------------

ALTERNATIVE A - MIT License

Copyright (c) 2016 Mattias Gustavsson

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
