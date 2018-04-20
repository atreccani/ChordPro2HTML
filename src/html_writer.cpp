//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

/* System Library Include
*/
#include "chordpro_parser.h"
#include "html_writer.h"

/* Application Local Include
*/
// No application library includes

//////////////////////////////////////////////////////////////////////////////
//                 C O N S T A N T S   D E F I N I T I O N S                //
//////////////////////////////////////////////////////////////////////////////

// No local constants


//////////////////////////////////////////////////////////////////////////////
//                  M A C R O S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

#define LINE_SPACING		40


//////////////////////////////////////////////////////////////////////////////
//                    T Y P E S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

// No local type definitions


//////////////////////////////////////////////////////////////////////////////
//                    C L A S S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

/*
<html>
<head>
	<title>Nel blu dipinto di blu</title>
	<link href="./cantalagioia.css" rel="stylesheet" type="text/css">
</head>

<body>
	<h1 class="Title">Nel blu dipinto di blu</h1>
	<h2 class="Artist">(Domenico Modugno)</h2>
*/

HTMLWrite::HTMLWrite()
{
}

size_t HTMLWrite::paint(ChordProData &src)
{
	parsed_item_t it;

	string value;

	reinit();

	for (const chordpro_element_t &it : src.elements) {

		switch (it.id) {
		case PARSED_ITEM_NEWLINE:
			startline();
			break;

		case PARSED_ITEM_DIRECTIVE_COMMENT:
		case PARSED_ITEM_DIRECTIVE_COMMENT_ITALIC:
		case PARSED_ITEM_DIRECTIVE_COMMENT_BOX:
			putlyrics(L"(");
			putlyrics(it.value);
			putlyrics(L")");
			break;

		case PARSED_ITEM_TEXT:
			putlyrics(it.value);
			break;

		case PARSED_ITEM_CHORD:
			putchord(it.value);
			break;
		}
	}
	yMax -= LINE_SPACING;

	return 0;
}

void HTMLWrite::reinit()
{
	xLyrics = 0;
	xChords = 0;
	yLyrics = 0;

	xMax = 0;
	yMax = 0;
}

void HTMLWrite::startline()
{
	xLyrics = 0;
	xChords = 0;

	yLyrics += LINE_SPACING;

	if (yLyrics > yMax) {
		yMax = yLyrics;
	}
}

void HTMLWrite::putlyrics(wstring scan)
{
	// Print Lyrics

	if (xLyrics > xMax) {
		xMax = xLyrics;
	}
}

void HTMLWrite::putchord(wstring name)
{
	if (xChords < xLyrics) {
		// align chord coordinate to lyrics 
		xChords = xLyrics;
	}

	if (xChords > xMax) {
		xMax = xChords;
	}
}

