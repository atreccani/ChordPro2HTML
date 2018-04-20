/****************************************************************************
**
** ChordPro data.
**
****************************************************************************/

//Include only once
#ifndef CHORDPRODATA_H
#define CHORDPRODATA_H

//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

/* System Library Include
*/
#include <list>
#include <string>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//                 C O N S T A N T S   D E F I N I T I O N S                //
//////////////////////////////////////////////////////////////////////////////

// No public constants


//////////////////////////////////////////////////////////////////////////////
//                  M A C R O S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

// No public macros


//////////////////////////////////////////////////////////////////////////////
//                    T Y P E S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

typedef enum {
	PARSED_ITEM_MIN = 0,
	PARSED_ITEM_NONE			= PARSED_ITEM_MIN,
	PARSED_ITEM_NEWLINE,
	PARSED_ITEM_COMMENT,
	PARSED_ITEM_CHORD,
	PARSED_ITEM_TEXT,

	// Directives
	PARSED_ITEM_DIRECTIVE_NONE,

	// Preamble directives
	PARSED_ITEM_DIRECTIVE_NEW_SONG,

	// Metadata directives
	PARSED_ITEM_DIRECTIVE_TITLE,
	PARSED_ITEM_DIRECTIVE_SUBTITLE,
	PARSED_ITEM_DIRECTIVE_ARTIST,
	PARSED_ITEM_DIRECTIVE_COMPOSER,
	PARSED_ITEM_DIRECTIVE_LYRICIST,
	PARSED_ITEM_DIRECTIVE_ARRANGER,
	PARSED_ITEM_DIRECTIVE_COPYRIGHT,
	PARSED_ITEM_DIRECTIVE_ALBUM,
	PARSED_ITEM_DIRECTIVE_YEAR,
	PARSED_ITEM_DIRECTIVE_KEY,
	PARSED_ITEM_DIRECTIVE_TIME,
	PARSED_ITEM_DIRECTIVE_TEMPO,
	PARSED_ITEM_DIRECTIVE_DURATION,
	PARSED_ITEM_DIRECTIVE_CAPO,
	PARSED_ITEM_DIRECTIVE_META,

	// Formatting directives
	PARSED_ITEM_DIRECTIVE_COMMENT,
	PARSED_ITEM_DIRECTIVE_COMMENT_ITALIC,
	PARSED_ITEM_DIRECTIVE_COMMENT_BOX,
	PARSED_ITEM_DIRECTIVE_CHORUS,
	PARSED_ITEM_DIRECTIVE_IMAGE,

	// Environment
	PARSED_ITEM_DIRECTIVE_CHORUS_START,
	PARSED_ITEM_DIRECTIVE_CHORUS_END,
	PARSED_ITEM_DIRECTIVE_VERSE_START,
	PARSED_ITEM_DIRECTIVE_VERSE_END,
	PARSED_ITEM_DIRECTIVE_TAB_START,
	PARSED_ITEM_DIRECTIVE_TAB_END,
	PARSED_ITEM_DIRECTIVE_GRID_START,
	PARSED_ITEM_DIRECTIVE_GRID_END,

	// Chord diagrams directives
	PARSED_ITEM_DIRECTIVE_DEFINE,
	PARSED_ITEM_DIRECTIVE_CHORD,

	PARSED_ITEM_MAX
} parsed_item_t;

typedef struct {
	parsed_item_t	id;
	wstring			value;
} chordpro_element_t;


//////////////////////////////////////////////////////////////////////////////
//                   G L O B A L   V A R I A B L E S                        //
//////////////////////////////////////////////////////////////////////////////

// No global variables


//////////////////////////////////////////////////////////////////////////////
//                 F U N C T I O N S    P R O T O T Y P I N G               //
//////////////////////////////////////////////////////////////////////////////
parsed_item_t getEnDirective(string &label);


//////////////////////////////////////////////////////////////////////////////
//                    C L A S S    D E C L A R A T I O N                    //
//////////////////////////////////////////////////////////////////////////////

class ChordProData
{
public:
	list<chordpro_element_t> elements;
};



#endif // CHORDPRODATA_H
