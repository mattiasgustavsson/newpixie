#include "pixie.h"
#include "dialog.h"
#include "dialog_playback.h"

#if defined( __cplusplus ) && !defined( PIXIE_NO_NAMESPACE )
using namespace pixie;
#endif

#ifndef PIXIE_NO_BUILD
    void* adjust_sprite( const char* filenames[], int count, int* out_size );
    void* build_dialog( char const* filenames[], int count, int* out_size );
#endif /* PIXIE_NO_BUILD */

#define ASSET_DIALOG( id, filename ) id,


ASSETS_BEGIN( "stranded.dat" )
ASSET_BINARY( SOUNDFONT, "stranded/AweROMGM.sf2" )
ASSET_SONG( SONG, "stranded/stranded.mid" )
ASSET_FONT( FONT, "stranded/Volter__28Goldfish_29.ttf" )
ASSET_PALETTE( PALETTE, "stranded/palette.png" )
ASSET_SPRITE( BACKGROUND, "stranded/background.png" )
ASSET_SPRITE( TREELINE, "stranded/sprites/treeline.png" )
ASSET_SPRITE( FACE, "stranded/sprites/face.png" )
ASSET_SPRITE( LOGO, "stranded/logo.png" )
ASSET_DIALOG( DIALOG, "stranded/dialog.dlg" )
ASSET_SPRITE( BOAT, "stranded/sprites/boat.png" )
ASSET_SPRITE( CHAIR, "stranded/sprites/chair.png" )
ASSET_SPRITE( GUY_CHAIR, "stranded/sprites/guychair.png" )
ASSET_SPRITE( GUY_IDLE, "stranded/sprites/guy_idle.png" )
ASSET_SPRITE( GUY_JUMP, "stranded/sprites/guy_jump_*.png" )
ASSET_SPRITE( GUY_LEFT, "stranded/sprites/guy_walk_left_*.png" )
ASSET_SPRITE( GUY_RIGHT, "stranded/sprites/guy_walk_right_*.png" )
ASSET_SPRITE( HUT1, "stranded/sprites/hut1.png" )
ASSET_SPRITE( HUT2, "stranded/sprites/hut2.png" )
ASSET_SPRITE( HUT_BOOK, "stranded/sprites/hut_book.png" )
ASSET_SPRITE( HUT_BOOK_NOBULB, "stranded/sprites/hut_book_nobulb.png" )
ASSET_SPRITE( HUT_TV, "stranded/sprites/hut_tv.png" )
ASSET_SPRITE( HUT_TV_NODISH, "stranded/sprites/hut_tv_nodish.png" )
ASSET_SPRITE( PALMTREE, "stranded/sprites/palmtree.png" )
ASSET_SPRITE( PENGUIN, "stranded/sprites/penguin.png" )
ASSET_SPRITE( PENGUIN_BATTERY, "stranded/sprites/penguin_battery.png" )
ASSET_SPRITE( PENGUIN_WALK, "stranded/sprites/penguin_walk_*.png" )
ASSET_SPRITE( RADIO_BATTERY, "stranded/sprites/radio_battery.png" )
ASSET_SPRITE( RADIO_BATTERY_DISH, "stranded/sprites/radio_battery_dish.png" )
ASSET_SPRITE( RADIO_BULB, "stranded/sprites/radio_bulb.png" )
ASSET_SPRITE( RADIO_BULB_BATTERY, "stranded/sprites/radio_bulb_battery.png" )
ASSET_SPRITE( RADIO_BULB_DISH, "stranded/sprites/radio_bulb_dish.png" )
ASSET_SPRITE( RADIO_DISH, "stranded/sprites/radio_dish.png" )
ASSET_SPRITE( RADIO_FULL, "stranded/sprites/radio_full.png" )
ASSET_SPRITE( RADIO_NONE, "stranded/sprites/radio_none.png" )
ASSET_SPRITE( RADIO_SPR, "stranded/sprites/radio_spr.png" )
ASSET_SPRITE( TREE2, "stranded/sprites/tree2.png" )
ASSETS_END()
 


int is_near( int spr, int xpos ) {
	return ( xpos > sprite_x( spr ) - 25 ) && ( xpos < sprite_x( spr ) + 25 );
}


void set_interact( int spr, char const* str, int interact, int offset ) {
    label_text( interact, str );
	sprite_pos( interact, sprite_x( spr ) + 160 + offset, sprite_y( interact ) );
}

// Color indexes of the loaded palette
int color_green = 17;
int color_yellow = 30;
int color_purple = 51;
int color_brown = 35;

void setup_speech( int speech, char const* character )
	{
    struct {
        char const* name;
        int x;
        int y;
        int color;
        int wrap;
    } speech_settings[] = {
		{ "player_sitting", 50, 85, color_green, 220 },
		{ "player", 20, 60, color_green, 220 }, 
		{ "player_hut", 100, 60, color_green, 220 }, 
		{ "player_radio", 50, 20, color_green, 220 }, 
		{ "tv_guy", 20, 20, color_yellow, 220 }, 
		{ "book_guy", 20, 20, color_purple, 220 }, 
		{ "penguin", 185, 75, color_brown, 120 }, 
	};

    for( int i = 0; i < ARRAY_COUNT( speech_settings ) ; ++i ) {
        if( compare( str( speech_settings[ i ].name ), str( character ) ) == 0 ) {
            sprite_pos( speech, speech_settings[ i ].x, speech_settings[ i ].y );
            label_color( speech, speech_settings[ i ].color );
            label_wrap( speech, speech_settings[ i ].wrap );
            return;
        }
    }
}


void title_screen( void ) {
    int spr_index = 1;
    sprite( spr_index++, -1050, 0, BACKGROUND );
    sprite( spr_index++, -1050, 0, TREELINE ) ;
    int face = sprite( spr_index++, 0, 200, FACE ) ;
    int logo = sprite( spr_index++, 0, -200, LOGO ) ;

    move_t face_move[] = { { 0, 6, MOVE_DELAY }, { 50, 120, MOVE_EASE_OUT_ELASTIC } };
    move_sprite_y( face, face_move, ARRAY_COUNT( face_move ) );

    move_t logo_move[] = { { 0, 30, MOVE_DELAY }, { 0, 60, MOVE_EASE_OUT_BOUNCE } };
    move_sprite_y( logo, logo_move, ARRAY_COUNT( logo_move ) );

    int credits = label( spr_index++, -160, 185, "Code, art, music and design by Mattias Gustavsson", 2, FONT );
    label_align( credits, TEXT_ALIGN_CENTER );
    label_outline( credits, 0 );
    label_shadow( credits, 0 );

    move_t credits_move[] = { { 0, 30, MOVE_DELAY }, { 160, 60, MOVE_EASE_OUT_QUAD } };
    move_sprite_x( credits, credits_move, ARRAY_COUNT( credits_move ) );

	LOOP {
        wait_vbl();
		if( key_was_pressed( KEY_ESCAPE ) ) end( 0 ); // exit with no error
        if( key_was_pressed( KEY_RETURN ) && key_is_down( KEY_MENU ) ) fullscreen() ? fullscreen_off() : fullscreen_on();
        if( key_was_pressed( KEY_TAB ) ) crt_mode() ? crt_mode_off() : crt_mode_on();
		if( key_was_pressed( KEY_SPACE ) ) { 
            sprites_off();
            return;
        }
	}	
}


ARRAY_TYPE( objects, int, 32 );


int pixmain( int argc, char** argv ) {
    (void) argc, argv;
    #ifndef PIXIE_NO_BUILD
        register_asset_type( "DIALOG", build_dialog );
        register_asset_type( "SPRITE", adjust_sprite );
    #endif
    if( load_assets() != 0 ) return 1;

//  title( "Stranded" );
//	screen_size( 320, 200 );
//	border_size( 32, 44 );
	fullscreen_on();
	crt_mode_on();
//	mouse_hide();

	load_palette( PALETTE );
    set_soundfont( SOUNDFONT );
    play_song( SONG );

	title_screen();
  
    objects_type objects = { 0 };

    int spr_index = 1;

    int background = sprite( spr_index++, -65, 0, BACKGROUND );
    int treeline = sprite( spr_index++, -65, 0, TREELINE );
    int radio = sprite( spr_index++, 240, 0, RADIO_SPR ); objects_add( &objects, radio );
    int hut1 = sprite( spr_index++, 540, 0, HUT1 ); objects_add( &objects, hut1 );
	int hut2 = sprite( spr_index++, 850, 20, HUT2 ); objects_add( &objects, hut2 );
	objects_add( &objects, sprite( spr_index++, 180, -30, TREE2 ) );
	objects_add( &objects, sprite( spr_index++, 120, -10, PALMTREE ) );
	int chair = sprite( spr_index++, 65, 25, CHAIR ); objects_add( &objects, chair );
	int penguin_battery = sprite( spr_index++, 970 + 62, -10, PENGUIN_BATTERY ); objects_add( &objects, penguin_battery );
	int penguin = sprite( spr_index++, 970, -10, PENGUIN ); objects_add( &objects, penguin );

	int xpos = 65;
	int guy = sprite( spr_index++, xpos, 25, GUY_CHAIR );
    
	int interact = label( spr_index++, 160, 70, "", 2 /* brightest_color() */, FONT );
    label_align( interact, TEXT_ALIGN_CENTER );
    label_outline( interact, 0 );
    label_shadow( interact, 0 );
    
    int radio_back = sprite( spr_index++, -250, 0, BACKGROUND );
    sprite_hide( radio_back );

    int radio_pic = sprite( spr_index++, 0, 0, RADIO_FULL );
    sprite_hide( radio_pic );

	int hut_tv_pic = sprite( spr_index++, 0, 0, HUT_TV );
    sprite_hide( hut_tv_pic );

	int hut_bulb = sprite( spr_index++, 0, 0, HUT_BOOK );
    sprite_hide( hut_bulb );
        	
    int guy_inside = sprite( spr_index++, 0, 0, GUY_IDLE );
    sprite_hide( guy_inside );

    int boat = sprite( spr_index++, -320, 30, BOAT );

	int speech = label( spr_index++, 50, 70, "", 18 /* green */, FONT );
    label_align( speech, TEXT_ALIGN_CENTER );
    label_outline( speech, 0 );
    label_shadow( speech, 0 );
    label_wrap( speech, 220 );

	dialog_playback_t dlg;
    dialog_playback_init( &dlg );
	dialog_playback_add_dialog( &dlg, (dialog_t*) asset_data( DIALOG ) );

	int fadeout = 0;
    int fadeout_delay = 120;
    int speech_visible = 1;
	int rescued = 0;
	int may_stand = 0;
	int looked_at_radio = 0;
	int sitting = 1;
	int penguin_gone = 0;
	int at_radio = 0;
	int at_hut_tv = 0;
	int at_hut_bulb = 0;
	int has_dish = 0;
	int has_bulb = 0;
	int has_battery = 0;	  
    int shown_intro = 0;

    float anim = 0.0f;

	LOOP {
		for( int i = 0; i < objects_count( &objects ); ++i )
            sprite_origin( objects_get( &objects, i ), xpos, 0 );

		sprite_origin( interact, xpos, 0 );

		sprite_origin( guy, xpos, 0 );
		sprite_pos( guy, xpos, sprite_y( guy ) );
		sprite_pos( background, -xpos, sprite_y( background ) );
		sprite_pos( treeline, -xpos, sprite_y( treeline ) );			

        if( !shown_intro ) {
            wait( 60 );
            dialog_playback_start_conversation( &dlg, "lets_go_home" );
            shown_intro = 1;
        }

        anim += 25.0f / 60.0f;
        sprite_cel( guy, (int) anim );

        wait_vbl();

    
		if( fadeout ) {
            if( --fadeout_delay <= 0 ) {
			int all_black = 1;
			for( int i = 0; i < 64; ++i ) {
				rgb_t c = getcol( i );
				if( c.r > 0 || c.g > 0 || c.b > 0 ) all_black = 0;
				if( c.r > 0 ) c.r = (u8)( c.r - min( c.r, 3 ) ); 
				if( c.r < 80 && c.g > 0 ) c.g = (u8)( c.g - min( c.g, 3 ) ); 
				if( c.g < 80 && c.b > 0 ) c.b = (u8)( c.b - min( c.b, 3 ) ); 
				setcol( i, c );
			}
			if( all_black ) end( 0 );
            }
		}
		
		if( key_was_pressed( KEY_ESCAPE ) ) end( 0 ); // exit with no error
        if( key_was_pressed( KEY_RETURN ) && key_is_down( KEY_MENU ) ) fullscreen() ? fullscreen_off() : fullscreen_on();
        if( key_was_pressed( KEY_TAB ) ) crt_mode() ? crt_mode_off() : crt_mode_on();
		if( dialog_playback_state( &dlg ) != DIALOG_PLAYBACK_STATE_READY ) {
			if( dialog_playback_state( &dlg ) == DIALOG_PLAYBACK_STATE_SENDING_EVENT ) {
   				string event = str( dialog_playback_current_conversation_event( &dlg ) );
				if( starts_with( event, str( "SetFlag:" ) ) ) {
					string flag = trim( mid( event, 8, -1 ) );
					if( compare( flag, str( "may_stand" ) ) == 0 ) {
						may_stand = 1;
					}
					if( compare( flag, str( "looked_at_radio" ) ) == 0 ) {
						looked_at_radio = 1;
					}
					if( compare( flag, str( "penguin_running" ) ) == 0 ) {
                        sprite_pos( penguin, 970 + 62, sprite_y( penguin ) );
                        sprite_bitmap( penguin, PENGUIN_WALK );
                        move_t penguin_move[] = { { 0, 30, MOVE_DELAY }, { 970 + 62 + 160, 60, MOVE_LINEAR } };
                        move_sprite_x( penguin, penguin_move, ARRAY_COUNT( penguin_move ) );
						wait( 30 );
                        float penganim = 0.0f;
                        for( int i = 0; i < 90; ++i ) {
                            wait_vbl();
                            penganim += 25.0f / 60.0f;
                            sprite_cel( penguin, (int) penganim );
                        }
                        sprite_hide( penguin );
					}
					if( compare( flag, str( "penguin_gone" ) ) == 0 ) {
						penguin_gone = 1;
						dialog_playback_execute( &dlg );
					}
					if( compare( flag, str( "rescued" ) ) == 0 ) {
						rescued = 1;
					}
				}
				if( starts_with( event, str( "ClearFlag:" ) ) ) {
					string flag = trim( mid( event, 10, -1 ) );
					if( compare( flag, str( "at_hut_tv" ) ) == 0 ) {
						sprite_hide( hut_tv_pic );
						sprite_hide( guy_inside );
						sprite_bitmap( hut_tv_pic, HUT_TV_NODISH );
						has_dish = 1;
						at_hut_tv = 0;
					}
					if( compare( flag, str( "at_hut_bulb" ) ) == 0 ) {
						sprite_hide( hut_bulb );
						sprite_hide( guy_inside );
						sprite_bitmap( hut_bulb, HUT_BOOK_NOBULB );
						has_bulb = 1;
						at_hut_bulb = 0;
					}
				}
 				dialog_playback_execute( &dlg );
			} else if( dialog_playback_state( &dlg ) == DIALOG_PLAYBACK_STATE_DISPLAYING_SPEECH ) {
				setup_speech( speech, dialog_playback_current_conversation_line_actor( &dlg ) );
				label_text( speech, dialog_playback_current_conversation_line_text( &dlg ) );
				if( speech_visible && key_was_pressed( KEY_SPACE ) )  {					
					speech_visible = 0;
                    label_text( speech, "" ); 
                    wait( 30 );
					dialog_playback_execute( &dlg ); 
                    speech_visible = 1;
				}
			} else {
                dialog_playback_execute( &dlg );
			}
			continue;
		} else {
		    label_text( speech, "" ); 
		}
		   
        label_text( interact, "" ); 
		if( !sitting && !at_radio && !at_hut_tv && !at_hut_bulb && !rescued ) {
			if( is_near( radio, xpos ) ) set_interact( radio, "Radio", interact, 0 );
			if( is_near( hut1, xpos ) ) set_interact( hut1, "Hut", interact, 0 );
			if( is_near( hut2, xpos ) ) set_interact( hut2, "Hut", interact, 0 );
			if( !penguin_gone && is_near( penguin, xpos ) ) set_interact( penguin, "Penguin", interact, 60 );
			if( !has_battery && penguin_gone && is_near( penguin_battery, xpos ) ) set_interact( penguin_battery, "Battery", interact, 0 );
				
			if( key_is_down( KEY_LEFT ) )  {
				xpos -= 2;
				if( xpos < 10 ) xpos = 10;
                sprite( guy, sprite_x( guy ), sprite_y( guy ), GUY_LEFT );
			} else if( key_is_down( KEY_RIGHT ) ) {
				xpos += 2;
				if( xpos > 1120 ) xpos = 1120;
                sprite( guy, sprite_x( guy ), sprite_y( guy ), GUY_RIGHT );
			} else {
                sprite( guy, sprite_x( guy ), sprite_y( guy ), GUY_IDLE );
			}

			if( key_was_pressed( KEY_SPACE ) ) {
				if( is_near( radio, xpos ) ) {
					sprite_show( radio_pic );
					if( has_dish && has_bulb && has_battery ) {
                        sprite_bitmap( radio_pic, RADIO_FULL );
						dialog_playback_start_conversation( &dlg, "radio_all" );
					} else if( has_dish && has_bulb ) {
                        sprite_bitmap( radio_pic, RADIO_BULB_DISH );
						dialog_playback_start_conversation( &dlg, "radio_bulb_dish" );
					} else if( has_battery && has_bulb ) {
                        sprite_bitmap( radio_pic, RADIO_BULB_BATTERY );
						dialog_playback_start_conversation( &dlg, "radio_bulb_battery" );
					} else if( has_dish && has_battery ) {
                        sprite_bitmap( radio_pic, RADIO_BATTERY_DISH );
						dialog_playback_start_conversation( &dlg, "radio_battery_dish" );
					} else if( has_dish ) {
                        sprite_bitmap( radio_pic, RADIO_DISH );
						dialog_playback_start_conversation( &dlg, "radio_dish" );
					} else if( has_battery ) {
                        sprite_bitmap( radio_pic, RADIO_BATTERY );
						dialog_playback_start_conversation( &dlg, "radio_battery" );
					} else if( has_bulb ) {
                        sprite_bitmap( radio_pic, RADIO_BULB );
						dialog_playback_start_conversation( &dlg, "radio_bulb" );
					} else {
                        sprite_bitmap( radio_pic, RADIO_NONE );
						if( !looked_at_radio )
							dialog_playback_start_conversation( &dlg, "radio_first" );
						else
							dialog_playback_start_conversation( &dlg, "radio_none" );
					}

					sprite_show( radio_back );
					at_radio = 1;
				}
				
                if( is_near( hut1, xpos ) ) {
					sprite_show( hut_tv_pic );
					sprite_show( guy_inside );
                    sprite_pos( guy_inside, 100, 40 );
					if( !has_dish )
						dialog_playback_start_conversation( &dlg, "hut_tv" );
					else
						dialog_playback_start_conversation( &dlg, "hut_tv_sleep" );
					at_hut_tv = 1;
				}

				if( is_near( hut2, xpos ) ) {
					sprite_show( hut_bulb );
					sprite_show( guy_inside );
                    sprite_pos( guy_inside, 100, 40 );
					if( !has_bulb )
						dialog_playback_start_conversation( &dlg, "hut_book" );
					else
						dialog_playback_start_conversation( &dlg, "hut_book_sleep" );
					at_hut_bulb = 1;
				}

				if( !penguin_gone && is_near( penguin, xpos ) ) {
                    sprite_bitmap( penguin, PENGUIN );
					dialog_playback_start_conversation( &dlg, "penguin" );
					label_text( interact, "" );
                    sprite_bitmap( guy, GUY_IDLE );
				}

				if( !has_battery && penguin_gone && is_near( penguin_battery, xpos ) ) {
					label_text( interact, "" );
					sprite_hide( penguin );
					dialog_playback_start_conversation( &dlg, "grab_battery" );
					sprite_hide( penguin_battery );
					has_battery = 1;
				}

                if( is_near( chair, xpos ) ) {
					sitting = 1;
					xpos = 65;
					sprite_bitmap( guy, GUY_CHAIR );
                    sprite_pos( guy, 0, 25 );
				}
			}

		} else if( sitting && may_stand ) {
			if( key_was_pressed( KEY_SPACE ) ) {
				sitting = 0;
                sprite( guy, 0, 30, GUY_IDLE ); 
			}
		} else if( at_radio ) {
			if( key_was_pressed( KEY_SPACE ) ) {
				sprite_hide( radio_pic );
				sprite_hide( radio_back );
				at_radio = 0;

                if( rescued ) {
                    sprite_bitmap( guy, GUY_JUMP );
					xpos = 20;
                    move_t boat_move[] = { { 0, 90, MOVE_DELAY }, { -190, 120, MOVE_EASE_OUT_QUAD } };
                    move_sprite_x( boat, boat_move, ARRAY_COUNT( boat_move ) );
					fadeout = 1; 
				}
			}
		} else if( at_hut_tv ) {
			if( key_was_pressed( KEY_SPACE ) ) {
			    sprite_hide( hut_tv_pic );
			    sprite_hide( guy_inside );
				sprite_bitmap( hut_tv_pic, HUT_TV_NODISH );
				has_dish = 1;
				at_hut_tv = 0;
			}
		} else if( at_hut_bulb ) {
			if( key_was_pressed( KEY_SPACE ) ) {
				sprite_hide( hut_bulb );
				sprite_hide( guy_inside );
				sprite_bitmap( hut_bulb, HUT_BOOK_NOBULB );
				has_bulb = 1;
				at_hut_bulb = 0;
			}
		}

        dialog_playback_execute( &dlg );
	}
}


#define DIALOG_IMPLEMENTATION
#include "dialog.h"

#define DIALOG_PLAYBACK_IMPLEMENTATION
#include "dialog_playback.h"

#define PIXIE_IMPLEMENTATION
#include "pixie.h"


#ifndef PIXIE_NO_BUILD

    #define _CRT_NONSTDC_NO_DEPRECATE 
    #define _CRT_SECURE_NO_WARNINGS
    #include <stdlib.h>
    #include <string.h>
    #include "img.h"
    #include "pixie/stb_image.h"
    #include "pixie/stb_image_write.h"

    void* build_dialog( char const* filenames[], int count, int* out_size ) {
        if( count != 1 ) return 0;

        int in_size = 0;
        char* in_data = load_text_file( filenames[ 0 ], &in_size );
        if( !in_data ) return 0;
    
        dialog_t* dialog = dialog_create( in_data, in_size );
        free_text_file( in_data );
        if( !dialog ) return 0;
        int size = dialog_save( dialog, 0, 0 );
        void* out_data = malloc( (size_t) size );
        *out_size = dialog_save( dialog, out_data, size );
        dialog_destroy( dialog );
        return out_data;
    }


    void process_image( IMG_U32* abgr, int w, int h ) {
	    img_t img = img_from_abgr32( abgr, w, h );
	    img_adjust_brightness( &img, 0.065f );
	    img_sharpen( &img, 0.5f, 1.0f );
	    img_adjust_contrast( &img, 1.15f );
	    img_adjust_saturation( &img, 0.05f );
	    img_to_argb32( &img, abgr );
        img_free( &img );
    }


    void* adjust_sprite( const char* filenames[], int count, int* out_size ) {
        char** tempnames = (char**) malloc( sizeof( *tempnames ) * count );
        for( int i = 0; i < count; ++i ) {
            int w, h, c;
            stbi_uc* img = stbi_load( filenames[ i ], &w, &h, &c, 4 );
            if( !img ) return NULL;
            if( strstr( filenames[ i ], "/sprites/" ) != 0 ) {
                process_image( (IMG_U32*) img, w, h );
            }
            char tempname[ 64 ];
            sprintf( tempname, "temp_%d.tga", i );
            tempnames[ i ] = strdup( tempname );
            stbi_write_tga_with_rle = 0;
            stbi_write_tga( tempname, w, h, 4, img );
            stbi_image_free( img );
        }
        void* data = build_sprite( (char const**) tempnames, count, out_size );
        for( int i = 0; i < count; ++i ) {
            free( tempnames[ i ] );
        }
        free( tempnames );
        return data;
    }

    #pragma warning( push )
    #pragma warning( disable: 4365 )
    #define STB_IMAGE_WRITE_IMPLEMENTATION
    #include "pixie/stb_image_write.h"
    #undef STB_IMAGE_WRITE_IMPLEMENTATION
    #pragma warning( pop )

    #define IMG_IMPLEMENTATION
    #include "img.h"

#endif /* PIXIE_NO_BUILD */

