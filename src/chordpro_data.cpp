//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

/* System Library Include
*/
#include <fstream>
#include <iostream>
#include <sstream>

/* Application Local Include
*/
#include "chordpro_data.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//                 C O N S T A N T S   D E F I N I T I O N S                //
//////////////////////////////////////////////////////////////////////////////

// No local constants


//////////////////////////////////////////////////////////////////////////////
//                  M A C R O S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


//////////////////////////////////////////////////////////////////////////////
//                    T Y P E S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

// No local type definitions


//////////////////////////////////////////////////////////////////////////////
//                   L O C A L S    D E F I N I T I O N S                   //
//////////////////////////////////////////////////////////////////////////////
const song_element_descript_t ChordProData::song_element_desc_list[] = {

	{ PARSED_ITEM_NONE						, u8"None"							, NULL	, NULL	},
	{ PARSED_ITEM_NEWLINE					, u8"Newline"						, NULL	, NULL	},
	{ PARSED_ITEM_COMMENT					, u8"Comment"						, NULL	, NULL	},
	{ PARSED_ITEM_CHORD						, u8"Chord"							, NULL	, NULL	},
	{ PARSED_ITEM_TEXT						, u8"Text"							, NULL	, NULL	},


	// Directives
	{ PARSED_ITEM_DIRECTIVE_NONE			, u8"Directive - None"				, NULL	, NULL	},

	// Preamble directives
	{ PARSED_ITEM_DIRECTIVE_NEW_SONG		, u8"Directive - New song"			, "new_song"			, "ns"	},

	// Metadata directives
	{ PARSED_ITEM_DIRECTIVE_TITLE			, u8"Directive - Title"				, "title"				, "t"	},
	{ PARSED_ITEM_DIRECTIVE_SUBTITLE		, u8"Directive - Subtitle"			, "subtitle"			, "st"	},
	{ PARSED_ITEM_DIRECTIVE_ARTIST			, u8"Directive - Artist"			, "artist"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_COMPOSER		, u8"Directive - Composer"			, "composer"			, NULL	},
	{ PARSED_ITEM_DIRECTIVE_LYRICIST		, u8"Directive - Lyricist"			, "lyricist"			, NULL	},
	{ PARSED_ITEM_DIRECTIVE_ARRANGER		, u8"Directive - Arranger"			, "arranger"			, NULL	},
	{ PARSED_ITEM_DIRECTIVE_COPYRIGHT		, u8"Directive - Copyright"			, "copyright"			, NULL	},
	{ PARSED_ITEM_DIRECTIVE_ALBUM			, u8"Directive - Album"				, "album"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_YEAR			, u8"Directive - Year"				, "year"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_KEY				, u8"Directive - Key"				, "key"					, NULL	},
	{ PARSED_ITEM_DIRECTIVE_TIME			, u8"Directive - Time"				, "time"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_TEMPO			, u8"Directive - Tempo"				, "tempo"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_DURATION		, u8"Directive - Duration"			, "duration"			, NULL	},
	{ PARSED_ITEM_DIRECTIVE_CAPO			, u8"Directive - Capo"				, "capo"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_META			, u8"Directive - Meta"				, "meta"				, NULL	},


	// Formatting directives
	{ PARSED_ITEM_DIRECTIVE_COMMENT			, u8"Directive - Comment"			, "comment"				, "c"	},
	{ PARSED_ITEM_DIRECTIVE_COMMENT_ITALIC	, u8"Directive - Comment italic"	, "comment_italic"		, "ci"	},
	{ PARSED_ITEM_DIRECTIVE_COMMENT_BOX		, u8"Directive - Comment box"		, "comment_box"			, "cb"	},
	{ PARSED_ITEM_DIRECTIVE_CHORUS			, u8"Directive - Chorus"			, "chorus"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_IMAGE			, u8"Directive - Image"				, "image"				, NULL	},


	// Environment
	{ PARSED_ITEM_DIRECTIVE_CHORUS_START	, u8"Directive - Chorus start"		, "start_of_chorus"		, "soc"	},
	{ PARSED_ITEM_DIRECTIVE_CHORUS_END		, u8"Directive - Chorus end"		, "end_of_chorus"		, "eoc"	},
	{ PARSED_ITEM_DIRECTIVE_VERSE_START		, u8"Directive - Verse start"		, "start_of_verse"		, NULL	},
	{ PARSED_ITEM_DIRECTIVE_VERSE_END		, u8"Directive - Verse end"			, "end_of_verse"		, NULL	},
	{ PARSED_ITEM_DIRECTIVE_TAB_START		, u8"Directive - Tab start"			, "start_of_tab"		, "sot"	},
	{ PARSED_ITEM_DIRECTIVE_TAB_END			, u8"Directive - Tab end"			, "end_of_tab"			, "eot"	},
	{ PARSED_ITEM_DIRECTIVE_GRID_START		, u8"Directive - Grid start"		, "start_of_grid"		, NULL	},
	{ PARSED_ITEM_DIRECTIVE_GRID_END		, u8"Directive - Grid end"			, "end_of_grid"			, NULL	},

	// Chord diagrams directives
	{ PARSED_ITEM_DIRECTIVE_DEFINE			, u8"Directive - Define"			, "define"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_CHORD			, u8"Directive - Chord"				, "chord"				, NULL	}
};


//////////////////////////////////////////////////////////////////////////////
//                    C L A S S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//                     P U B L I C   F U N C T I O N S                      //
//////////////////////////////////////////////////////////////////////////////
song_element_t ChordProData::getEnDirective(string &label)
{
	// Look for metadata directives entry
	for (int ii = 0; ii < ARRAY_SIZE(ChordProData::song_element_desc_list); ii++) {
		const song_element_descript_t *pp = &ChordProData::song_element_desc_list[ii];

		// short form
		if (pp->label_short) {
			if (label == pp->label_short) {
				return pp->id;
			}
		}
		// long form
		if (pp->label_long) {
			if (label == pp->label_long) {
				return pp->id;
			}
		}
	}
	return PARSED_ITEM_DIRECTIVE_NONE;
}

const char *ChordProData::getDescription(song_element_t it)
{
	for (int ii = 0; ii < ARRAY_SIZE(ChordProData::song_element_desc_list); ii++) {
		const song_element_descript_t *pp = &ChordProData::song_element_desc_list[ii];
		if(pp->id == it) {
			return pp->description;
		}
	}
	return "?";
}

void ChordProData::removeMultipleSpaces()
{
	bool newline_found = false;

	list<chordpro_element_t>::iterator it = elements.begin();
	while (it != elements.end()) {
		if (newline_found) {
			if (
				(it->id == PARSED_ITEM_NEWLINE) ||
				((it->id == PARSED_ITEM_TEXT) && (it->value.empty()))	/* nothing but whitespace */
				) {
				string Label(ChordProData::getDescription(it->id));
				string Log = "Removing Id: " + Label + ", Val: " + it->value;

				it = elements.erase(it);
			}
			else {
				newline_found = false;
				it++;
			}
		}
		else {
			if (it->id == PARSED_ITEM_NEWLINE) {
				newline_found = true;
			}
			it++;
		}
	}
	if (newline_found) {
		elements.pop_back();
	}
}


