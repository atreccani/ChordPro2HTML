/****************************************************************************
**
** ChordPro parser.
**
****************************************************************************/

#ifndef CHORDPROPARSER_H
#define CHORDPROPARSER_H

//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

/* System Library Include
*/
#include <list>
#include <string>

using namespace std;

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

class ParsedSongItem {
public:
	//ParsedSongItem() {}
	//ParsedSongItem(const ParsedSongItem &other);

public:
	parsed_item_t	id;
	wstring			value;
};

class ChordProParser
{
public:
	ChordProParser(wstring &file_name) {
		m_FileName = file_name;
	}

	bool loadFile();

	bool parseTitle(void);
	wstring &title();
	void parseAll(void);
	void removeMultipleSpaces(void);
	list <ParsedSongItem> &all();
	void reinit(void);
	parsed_item_t get(wstring &arg);



private:
	bool isLineBegin(void);
	parsed_item_t item_starting();
	parsed_item_t parseDirective(string &label);
	void getComment(wstring &arg);
	void getChord(wstring &arg);
	parsed_item_t getDirective(wstring &arg);
	void getText(wstring &arg);

public:
	wstring					m_Input;

private:
	wstring					m_FileName;
	const wchar_t			*m_Pos;
	wstring					m_Title;
	list<wstring>			m_Subtitles;
	list<ParsedSongItem>	m_AllItems;

};

extern const wchar_t *ParserLabel(parsed_item_t it);

#endif // CHORDPROPARSER_H
