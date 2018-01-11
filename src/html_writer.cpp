/****************************************************************************
**
** HTML writer.
**
****************************************************************************/
#include "chordpro_parser.h"
#include "html_writer.h"


#define LINE_SPACING		40

#define DIM(a) (sizeof(a) / sizeof(a[0]))

HTMLWrite::HTMLWrite()
{
}

size_t HTMLWrite::paint(ChordProParser *chproFile)
{
	parsed_item_t it;

	string value;

	reinit();

	for (const ParsedSongItem &it : chproFile->all()) {

		switch (it.id) {
		case PARSED_ITEM_NEWLINE:
			startline();
			break;

		case PARSED_ITEM_DIRECTIVE_COMMENT:
		case PARSED_ITEM_DIRECTIVE_COMMENT_ITALIC:
		case PARSED_ITEM_DIRECTIVE_COMMENT_BOX:
			putlyrics("(");
			putlyrics(it.value);
			putlyrics(")");
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

void HTMLWrite::putlyrics(string scan)
{
	// Print Lyrics

	if (xLyrics > xMax) {
		xMax = xLyrics;
	}
}

void HTMLWrite::putchord(string name)
{
	if (xChords < xLyrics) {
		// align chord coordinate to lyrics 
		xChords = xLyrics;
	}

	if (xChords > xMax) {
		xMax = xChords;
	}
}

