/*
------------------------------------------------------------------------------
          Licensing information can be found at the end of the file.
------------------------------------------------------------------------------

dialog_playback.h - v0.1 - System for playing dialogs loaded with dialog.h lib

Do this:
    #define DIALOG_PLAYBACK_IMPLEMENTATION
before you include this file in *one* C/C++ file to create the implementation.
*/

#ifndef dialog_playback_h
#define dialog_playback_h

#include "dialog.h"

typedef enum dialog_playback_state_t {
    DIALOG_PLAYBACK_STATE_READY,
    DIALOG_PLAYBACK_STATE_RUN_CONVERSATION,
    DIALOG_PLAYBACK_STATE_DISPLAYING_OPTIONS,
    DIALOG_PLAYBACK_STATE_EXECUTING_OPTIONS,
    DIALOG_PLAYBACK_STATE_DISPLAYING_SPEECH,
    DIALOG_PLAYBACK_STATE_SENDING_EVENT,
    DIALOG_PLAYBACK_STATE_UNDEFINED,
} dialog_playback_state_t;


typedef struct dialog_playback_t {
    dialog_playback_state_t current_state;
    
    dialog_t* dialogs[ 256 ];
    int dialogs_count;
    
    struct {
        int dialog;
	    dialog_conversation_t conversation;
	    int conversation_index;			
    } stack[ 1024 ];
    int stack_count;

    struct {
        int dialog;
        dialog_option_t option;
    } current_option_list[ 32 ];
    char const* current_option_list_strings[ 32 ];
    int current_option_list_count;

    struct
	    {
        int dialog;
	    dialog_option_t option;
	    } used_options[ 256 ]; 
    int used_options_count;

    int option_to_execute;
} dialog_playback_t;



void dialog_playback_init( dialog_playback_t* playback );

dialog_playback_state_t dialog_playback_state( dialog_playback_t const* playback );

void dialog_playback_add_dialog( dialog_playback_t* playback, dialog_t* dialog );

int dialog_playback_conversation_count( dialog_playback_t const* playback );
char const* dialog_playback_conversation_id( dialog_playback_t const* playback, int index );

void dialog_playback_start_conversation( dialog_playback_t* playback, char const* conversation_id );

void dialog_playback_execute( dialog_playback_t* playback );

char const* dialog_playback_current_conversation_line_actor( dialog_playback_t const* playback );
char const* dialog_playback_current_conversation_line_text( dialog_playback_t const* playback );

char const* dialog_playback_current_conversation_event( dialog_playback_t const* playback );

char const* const* dialog_playback_current_options( dialog_playback_t const* playback, int* count );
void dialog_playback_select_option( dialog_playback_t* playback, int option_index );

#endif /* dialog_playback_h */

/*
----------------------
    IMPLEMENTATION
----------------------
*/


#ifdef DIALOG_PLAYBACK_IMPLEMENTATION

#define _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>


void dialog_playback_init( dialog_playback_t* playback ) {
    memset( playback, 0, sizeof( *playback ) );
    playback->current_state = DIALOG_PLAYBACK_STATE_READY;
    playback->option_to_execute = -1;
}
		
		
void dialog_playback_destroy( dialog_playback_t* playback ) {
    free( playback );
}


dialog_playback_state_t dialog_playback_state( dialog_playback_t const* playback ) { 
    return playback->current_state;
}
	

void dialog_playback_add_dialog( dialog_playback_t* playback, dialog_t* dialog ) {
    playback->dialogs[ playback->dialogs_count++ ] = dialog;
}


int dialog_playback_conversation_count( dialog_playback_t const* playback ) {
    int count = 0;
    for( int i = 0; i < playback->dialogs_count; ++i ) {	    
	    count += dialog_conversation_count( playback->dialogs[ i ] );
	}
    return count;
}


char const* dialog_playback_conversation_id( dialog_playback_t const* playback, int index ) {
	int count = 0;
	for( int i = 0; i < playback->dialogs_count; ++i ) {
		dialog_t* dlg = playback->dialogs[ i ];
		for( int j = 0; j < dialog_conversation_count( dlg ); ++j ) {
			if( count == index ) {
                return dialog_conversation_id( dlg, dialog_conversation( dlg, j ) );
            }
			count++;
		}
	}

	return "";
}


static int dialog_playback_find_conversation( dialog_playback_t const* playback, char const* conversation_id, int* out_dialog, dialog_conversation_t* out_conversation ) {
	for( int i = playback->dialogs_count - 1; i >= 0; --i ) {
		int conv_index = dialog_find_conversation( playback->dialogs[ i ], conversation_id );
		if( conv_index >= 0 ) {
            *out_dialog = i;
            *out_conversation = dialog_conversation( playback->dialogs[ i ], conv_index );
            return 1;
        }
	}

	return 0;
}


static void dialog_playback_stack_push( dialog_playback_t* playback, int dialog, dialog_conversation_t conversation, int conversation_index ) {
    playback->stack[ playback->stack_count ].dialog = dialog;
    playback->stack[ playback->stack_count ].conversation = conversation;
    playback->stack[ playback->stack_count ].conversation_index = conversation_index;
    ++playback->stack_count;
} 


static int dialog_playback_stack_top( dialog_playback_t const* playback, int* out_dialog, dialog_conversation_t* out_conversation, int* out_conversation_index ) {
 	if( playback->stack_count <= 0 ) return 0;

    *out_dialog = playback->stack[ playback->stack_count - 1 ].dialog;
    *out_conversation = playback->stack[ playback->stack_count - 1 ].conversation;
    *out_conversation_index = playback->stack[ playback->stack_count - 1 ].conversation_index;
    return 1;
} 


static int dialog_playback_stack_pop( dialog_playback_t* playback ) {
 	if( playback->stack_count <= 0 ) return 0;

    --playback->stack_count;
    return 1;
} 


void dialog_playback_start_conversation( dialog_playback_t* playback, char const* conversation_id ) {
	ASSERT( playback->current_state == DIALOG_PLAYBACK_STATE_READY, "Already running a conv" );		
		
	// reset system
	playback->current_state = DIALOG_PLAYBACK_STATE_READY;
	playback->current_option_list_count = 0;
	playback->stack_count = 0;

	// Find the right conv
    int dialog = -1;
	dialog_conversation_t conv;
	if( dialog_playback_find_conversation( playback, conversation_id, &dialog, &conv ) ) {
		dialog_playback_stack_push( playback, dialog, conv, 0 );
		playback->current_state = DIALOG_PLAYBACK_STATE_RUN_CONVERSATION;
		return;
	}

    playback->current_state = DIALOG_PLAYBACK_STATE_READY;
	ASSERT( 0, "Conversation not found" );		
}

static void dialog_playback_run_conversation_entry( dialog_playback_t* playback, int dialog, dialog_conversation_t conversation, int current_index );
static void dialog_playback_run_conversation_option( dialog_playback_t* playback, int dialog, dialog_option_t option );
static void dialog_playback_redirect( dialog_playback_t* playback, char const* target );


void dialog_playback_execute( dialog_playback_t* playback ) {
	switch( playback->current_state ) {
		case DIALOG_PLAYBACK_STATE_UNDEFINED: ASSERT( 0, "Undefined state" ); break;

		case DIALOG_PLAYBACK_STATE_READY: break;

		case DIALOG_PLAYBACK_STATE_RUN_CONVERSATION: {
			int dialog = -1;
            dialog_conversation_t conv;
            int conv_index = -1;
            if( dialog_playback_stack_top( playback, &dialog, &conv, &conv_index ) ) {
			    dialog_playback_run_conversation_entry( playback, dialog, conv, conv_index );
            } else {
			    ASSERT( 0, "Conversation stack corrupted" );
            }
		} break;
		
		case DIALOG_PLAYBACK_STATE_DISPLAYING_OPTIONS: break;

		case DIALOG_PLAYBACK_STATE_EXECUTING_OPTIONS: {
			int dialog = -1;
            dialog_conversation_t conv;
            int conv_index = -1;
            if( dialog_playback_stack_top( playback, &dialog, &conv, &conv_index ) ) {
			    if( playback->current_option_list_count > 0 && playback->option_to_execute >= 0 && playback->option_to_execute < playback->current_option_list_count ) {
                    ASSERT( dialog == playback->current_option_list[ playback->option_to_execute ].dialog, "Dialog index mismatch" );
                    dialog_option_t option = playback->current_option_list[ playback->option_to_execute ].option;
				    if( !dialog_option_is_persistent( playback->dialogs[ dialog ], option ) ) {
                        playback->used_options[ playback->used_options_count ].dialog = dialog;
                        playback->used_options[ playback->used_options_count ].option = option;
                        ++playback->used_options_count;
                    }
                    
				    dialog_playback_redirect( playback, dialog_option_target( playback->dialogs[ dialog ], option ) );											
				    playback->current_option_list_count = 0;
			    }
            } else {
    			ASSERT( 0, "Conversation stack corrupted");
            }
		} break;

		case DIALOG_PLAYBACK_STATE_DISPLAYING_SPEECH: {
			int dialog = -1;
            dialog_conversation_t conv;
            int conv_index = -1;
            if( dialog_playback_stack_top( playback, &dialog, &conv, &conv_index ) ) {
                dialog_playback_stack_pop( playback );
                ++conv_index;
                dialog_playback_stack_push( playback, dialog, conv, conv_index );
			    dialog_playback_run_conversation_entry( playback, dialog, conv, conv_index );
            } else {
			    ASSERT( 0, "Conversation stack corrupted" );
            }
		} break;


		case DIALOG_PLAYBACK_STATE_SENDING_EVENT: {
			int dialog = -1;
            dialog_conversation_t conv;
            int conv_index = -1;
            if( dialog_playback_stack_top( playback, &dialog, &conv, &conv_index ) ) {
                dialog_playback_stack_pop( playback );
                ++conv_index;
                dialog_playback_stack_push( playback, dialog, conv, conv_index );
			    playback->current_state = DIALOG_PLAYBACK_STATE_RUN_CONVERSATION;
            } else {
			    ASSERT( 0, "Conversation stack corrupted" );
            }
		} break;
	}
}


char const* dialog_playback_current_conversation_line_actor( dialog_playback_t const* playback ) {
	if( playback->current_state != DIALOG_PLAYBACK_STATE_DISPLAYING_SPEECH ) return "";

	int dialog = -1;
    dialog_conversation_t conv;
    int conv_index = -1;
    if( dialog_playback_stack_top( playback, &dialog, &conv, &conv_index ) ) {
        dialog_entry_t entry = dialog_conversation_entry( playback->dialogs[ dialog ], conv, conv_index );
        if( dialog_entry_type( playback->dialogs[ dialog], entry ) == DIALOG_ENTRY_TYPE_LINE ) {
            dialog_line_t line = dialog_entry_line( playback->dialogs[ dialog ], entry );
	        return dialog_line_actor( playback->dialogs[ dialog ], line );
        }
    }

	return "";		
}


char const* dialog_playback_current_conversation_line_text( dialog_playback_t const* playback ) {
	if( playback->current_state != DIALOG_PLAYBACK_STATE_DISPLAYING_SPEECH ) return "";

	int dialog = -1;
    dialog_conversation_t conv;
    int conv_index = -1;
    if( dialog_playback_stack_top( playback, &dialog, &conv, &conv_index ) ) {
        dialog_entry_t entry = dialog_conversation_entry( playback->dialogs[ dialog ], conv, conv_index );
        if( dialog_entry_type( playback->dialogs[ dialog], entry ) == DIALOG_ENTRY_TYPE_LINE ) {
            dialog_line_t line = dialog_entry_line( playback->dialogs[ dialog ], entry );
	        return dialog_line_text( playback->dialogs[ dialog ], line );
        }
    }

	return "";		
}

char const* dialog_playback_current_conversation_event( dialog_playback_t const* playback ) {
	if( playback->current_state != DIALOG_PLAYBACK_STATE_SENDING_EVENT ) return "";

	int dialog = -1;
    dialog_conversation_t conv;
    int conv_index = -1;
    if( dialog_playback_stack_top( playback, &dialog, &conv, &conv_index ) ) {
        dialog_entry_t entry = dialog_conversation_entry( playback->dialogs[ dialog ], conv, conv_index );
        if( dialog_entry_type( playback->dialogs[ dialog], entry ) == DIALOG_ENTRY_TYPE_EVENT ) {
            dialog_event_t event = dialog_entry_event( playback->dialogs[ dialog ], entry );
	        return dialog_event_event( playback->dialogs[ dialog ], event );
        }
    }

	return "";		
}


char const* const* dialog_playback_current_options( dialog_playback_t const* playback, int* count ) {
	if( playback->current_state != DIALOG_PLAYBACK_STATE_DISPLAYING_OPTIONS ) return 0;
	if( playback->stack_count <= 0 ) return 0;
		
    if( count ) *count = playback->current_option_list_count;
	return playback->current_option_list_strings;
}


void dialog_playback_select_option( dialog_playback_t* playback, int option_index ) {
	if( option_index >= 0 && option_index < playback->current_option_list_count ) {
		playback->option_to_execute = option_index;
		playback->current_state = DIALOG_PLAYBACK_STATE_EXECUTING_OPTIONS;
	}
}


static void dialog_playback_run_conversation_entry( dialog_playback_t* playback, int dialog, dialog_conversation_t conversation, int current_index ) {
	// Is conv finished?
	if( current_index >= dialog_conversation_entry_count( playback->dialogs[ dialog ], conversation ) ) {
		// Yes, so bail out
		playback->current_state = DIALOG_PLAYBACK_STATE_READY;
		playback->current_option_list_count = 0;
		playback->stack_count = 0;
		return;
	}

	dialog_entry_t entry = dialog_conversation_entry( playback->dialogs[ dialog ], conversation, current_index );
    dialog_entry_type_t type = dialog_entry_type( playback->dialogs[ dialog ], entry );
	if( type == DIALOG_ENTRY_TYPE_LINE ) {
		playback->current_state = DIALOG_PLAYBACK_STATE_DISPLAYING_SPEECH;
	} else if( type == DIALOG_ENTRY_TYPE_OPTION ) {
		while( type == DIALOG_ENTRY_TYPE_OPTION ) {
			dialog_playback_run_conversation_option( playback, dialog, dialog_entry_option( playback->dialogs[ dialog ], entry ) );
			++current_index;
			if( dialog_conversation_entry_count( playback->dialogs[ dialog ], conversation ) == current_index ) break;
			entry = dialog_conversation_entry( playback->dialogs[ dialog ], conversation, current_index );
            type = dialog_entry_type( playback->dialogs[ dialog ], entry );
		}
					
		if( playback->current_option_list_count > 0 ) {
			playback->current_state = DIALOG_PLAYBACK_STATE_DISPLAYING_OPTIONS;
		} else {
			int dlg = -1;
            dialog_conversation_t conv;
            int conv_index = -1;
            if( dialog_playback_stack_top( playback, &dlg, &conv, &conv_index ) ) {
                dialog_playback_stack_pop( playback );
                dialog_playback_stack_push( playback, dlg, conv, current_index );
			    playback->current_state = DIALOG_PLAYBACK_STATE_RUN_CONVERSATION;
            } else {
			    ASSERT( 0, "Conversation stack corrupted");
            }
		}
	} else if( type == DIALOG_ENTRY_TYPE_REDIRECT ) {
        dialog_redirect_t redirect = dialog_entry_redirect( playback->dialogs[ dialog ], entry );
		dialog_playback_redirect( playback, dialog_redirect_target( playback->dialogs[ dialog ], redirect ) );
	} else if( type == DIALOG_ENTRY_TYPE_EVENT ) {
		playback->current_state = DIALOG_PLAYBACK_STATE_SENDING_EVENT;
	}
}

		
static void dialog_playback_run_conversation_option( dialog_playback_t* playback, int dialog, dialog_option_t option )	{
	int dlg = -1;
    dialog_conversation_t conv;
    int conv_index = -1;
    if( dialog_playback_stack_top( playback, &dlg, &conv, &conv_index ) ) {
	    for( int i = 0; i < playback->used_options_count; ++i ) {
		    if( playback->used_options[ i ].dialog == dlg && playback->used_options[ i ].option.handle == option.handle ) return;
		}

        playback->current_option_list[ playback->current_option_list_count ].dialog = dialog;
        playback->current_option_list[ playback->current_option_list_count ].option = option;
        char const* text = dialog_option_text( playback->dialogs[ dialog ], option );
        playback->current_option_list_strings[ playback->current_option_list_count ] = text;
	    ++playback->current_option_list_count;
    } else {
		ASSERT( 0, "Conversation stack corrupted");
    }

}
			

static void dialog_playback_redirect( dialog_playback_t* playback, char const* target ) {
	#ifdef _WIN32
	if( stricmp( target, "RETURN" ) == 0 && playback->stack_count > 1 ) {
	#else
	if( strcasecmp( target, "RETURN" ) == 0 && playback->stack_count > 1 ) {
	#endif
		dialog_playback_stack_pop( playback );
		playback->current_state = DIALOG_PLAYBACK_STATE_RUN_CONVERSATION;
		int dlg = -1;
        dialog_conversation_t conv;
        int conv_index = -1;
        if( dialog_playback_stack_top( playback, &dlg, &conv, &conv_index ) ) {
            dialog_playback_stack_pop( playback );
            dialog_playback_stack_push( playback, dlg, conv, 0 );
        }
	#ifdef _WIN32
    } else if( stricmp( target, "EXIT" ) == 0 || ( stricmp( target, "RETURN" ) == 0 && playback->stack_count <= 1 ) ) {
	#else
    } else if( strcasecmp( target, "EXIT" ) == 0 || ( strcasecmp( target, "RETURN" ) == 0 && playback->stack_count <= 1 ) ) {
	#endif
		playback->current_state = DIALOG_PLAYBACK_STATE_READY;
		playback->current_option_list_count = 0;
		playback->stack_count = 0;
	} else {
		// Find the right conv
        int dialog = -1;
        dialog_conversation_t conv;
		if( dialog_playback_find_conversation( playback, target, &dialog, &conv ) ) {
			dialog_playback_stack_push( playback, dialog, conv, 0 );
			playback->current_state = DIALOG_PLAYBACK_STATE_RUN_CONVERSATION;
		} else {
		    playback->current_state = DIALOG_PLAYBACK_STATE_READY;
		}
	}
}

#endif /* DIALOG_PLAYBACK_IMPLEMENTATION */

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
