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

typedef struct {
	parsed_item_t	id;
	const char		*label_long;
	const char		*label_short;
} directive_define_t;


//////////////////////////////////////////////////////////////////////////////
//                   L O C A L S    D E F I N I T I O N S                   //
//////////////////////////////////////////////////////////////////////////////

static const directive_define_t directive_list[] = {

	// Preamble directives
	{ PARSED_ITEM_DIRECTIVE_NEW_SONG		, "new_song"			, "ns"	},

	// Metadata directives
	{ PARSED_ITEM_DIRECTIVE_TITLE			, "title"				, "t"	},
	{ PARSED_ITEM_DIRECTIVE_SUBTITLE		, "subtitle"			, "st"	},
	{ PARSED_ITEM_DIRECTIVE_ARTIST			, "artist"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_COMPOSER		, "composer"			, NULL	},
	{ PARSED_ITEM_DIRECTIVE_LYRICIST		, "lyricist"			, NULL	},
	{ PARSED_ITEM_DIRECTIVE_ARRANGER		, "arranger"			, NULL	},
	{ PARSED_ITEM_DIRECTIVE_COPYRIGHT		, "copyright"			, NULL	},
	{ PARSED_ITEM_DIRECTIVE_ALBUM			, "album"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_YEAR			, "year"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_KEY				, "key"					, NULL	},
	{ PARSED_ITEM_DIRECTIVE_TIME			, "time"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_TEMPO			, "tempo"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_DURATION		, "duration"			, NULL	},
	{ PARSED_ITEM_DIRECTIVE_CAPO			, "capo"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_META			, "meta"				, NULL	},

	// Formatting directives
	{ PARSED_ITEM_DIRECTIVE_COMMENT			, "comment"				, "c"	},
	{ PARSED_ITEM_DIRECTIVE_COMMENT_ITALIC	, "comment_italic"		, "ci"	},
	{ PARSED_ITEM_DIRECTIVE_COMMENT_BOX		, "comment_box"			, "cb"	},
	{ PARSED_ITEM_DIRECTIVE_CHORUS			, "chorus"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_IMAGE			, "image"				, NULL	},

	// Environment
	{ PARSED_ITEM_DIRECTIVE_CHORUS_START	, "start_of_chorus"		, "soc"	},
	{ PARSED_ITEM_DIRECTIVE_CHORUS_END		, "end_of_chorus"		, "eoc"	},
	{ PARSED_ITEM_DIRECTIVE_VERSE_START		, "start_of_verse"		, NULL	},
	{ PARSED_ITEM_DIRECTIVE_VERSE_END		, "end_of_verse"		, NULL	},
	{ PARSED_ITEM_DIRECTIVE_TAB_START		, "start_of_tab"		, "sot"	},
	{ PARSED_ITEM_DIRECTIVE_TAB_END			, "end_of_tab"			, "eot"	},
	{ PARSED_ITEM_DIRECTIVE_GRID_START		, "start_of_grid"		, NULL	},
	{ PARSED_ITEM_DIRECTIVE_GRID_END		, "end_of_grid"			, NULL	},

	// Chord diagrams directives
	{ PARSED_ITEM_DIRECTIVE_DEFINE			, "define"				, NULL	},
	{ PARSED_ITEM_DIRECTIVE_CHORD			, "chord"				, NULL	}
};


//////////////////////////////////////////////////////////////////////////////
//                    C L A S S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//                     P U B L I C   F U N C T I O N S                      //
//////////////////////////////////////////////////////////////////////////////
parsed_item_t getEnDirective(string &label)
{
	// Look for metadata directives entry
	for (int ii = 0; ii < ARRAY_SIZE(directive_list); ii++) {
		const directive_define_t *pp = &directive_list[ii];

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

