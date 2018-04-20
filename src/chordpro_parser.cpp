//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

/* System Library Include
*/
#include <codecvt>
#include <fstream>
#include <iostream>
#include <sstream>

/* Application Local Include
*/
#include "chordpro_data.h"
#include "chordpro_parser.h"

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

//////////////////////////////////////////////////////////////////////////////
//                   L O C A L S    D E F I N I T I O N S                   //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//                    C L A S S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

bool ChordProParser::loadFile()
{
	// Read file in UTF-8
	wifstream wif(m_FileName);
	wif.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	// Open the file for reading
	if (!wif.is_open()) {
		wcout << L"Unable to open file" << endl;
		return false;
	}

	wstringstream wss;
	wss << wif.rdbuf();

	m_Input = wss.str();

	return true;

}

bool ChordProParser::parseTitle(void)
{
	wstring dir_value;
	parsed_item_t it;

	reinit();

	while ((it = get(dir_value)) != PARSED_ITEM_NONE) {
		switch (it) {
		case PARSED_ITEM_DIRECTIVE_TITLE:
			m_Title = dir_value;
			break;
		case PARSED_ITEM_DIRECTIVE_SUBTITLE:
			m_Subtitles.push_back(dir_value);
			break;
		default:
			break;
		}
	}

	if (m_Title.empty()) {
		return false;
	}
	return true;
}

wstring &ChordProParser::title()
{
	return m_Title;
}

void ChordProParser::parseAll(ChordProData	&dst)
{
	// List of elements parsed from file
	chordpro_element_t song_elem;

	reinit();
	while ((song_elem.id = get(song_elem.value)) != PARSED_ITEM_NONE) {
		dst.elements.push_back(song_elem);
	}

}

void ChordProParser::removeMultipleSpaces(ChordProData &lst)
{
	bool newline_found = false;

	list<chordpro_element_t>::iterator it = lst.elements.begin();
	while (it != lst.elements.end()) {
		if (newline_found) {
			if (
				(it->id == PARSED_ITEM_NEWLINE) || 
				((it->id == PARSED_ITEM_TEXT) && (it->value.empty()))	/* nothing but whitespace */
			) {
				wstring Label(ParserLabel(it->id));
				wstring Log = L"Removing Id: " + Label + L", Val: " + it->value;

				it = lst.elements.erase(it);
			} else {
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
		lst.elements.pop_back();
	}
}

void ChordProParser::reinit(void)
{
	m_Pos = m_Input.c_str();
}

parsed_item_t ChordProParser::get(wstring &arg)
{
	if (m_Pos >= m_Input.c_str() + m_Input.size()) {
		return PARSED_ITEM_NONE;
	}

	arg.clear();

	// detect string type depending on first character
	switch (item_starting()) {
	case PARSED_ITEM_NEWLINE:
		m_Pos++;
		return PARSED_ITEM_NEWLINE;

	case PARSED_ITEM_COMMENT:
		// Comment started
		getComment(arg);
		return PARSED_ITEM_COMMENT;

	case PARSED_ITEM_CHORD:
		// Chord started
		getChord(arg);
		return PARSED_ITEM_CHORD;
	
	case PARSED_ITEM_DIRECTIVE_NONE:
		// Directive started
		return getDirective(arg);
	
	default:
		// Normal text
		getText(arg);
		return PARSED_ITEM_TEXT;
	}

}

bool ChordProParser::isLineBegin(void)
{
	if (m_Pos == m_Input.c_str()) {
		// beginning of first  line
		return true;
	}
	return (*(m_Pos - 1) == '\n');
}

parsed_item_t ChordProParser::item_starting()
{
	if (*m_Pos == '\n')						return PARSED_ITEM_NEWLINE;
	if (isLineBegin() && (*m_Pos == '#'))	return PARSED_ITEM_COMMENT;
	if (*m_Pos == '[')						return PARSED_ITEM_CHORD;
	if (*m_Pos == '{')						return PARSED_ITEM_DIRECTIVE_NONE;

	return PARSED_ITEM_NONE;
}

void ChordProParser::getComment(wstring &arg)
{
	m_Pos++;
	while (m_Pos < m_Input.c_str() + m_Input.size()) {
		if (*m_Pos == '\n') {
			// Comment stops at the end of line
			m_Pos++;	// skip '\n'
			return;
		}
		arg += *m_Pos;
		m_Pos++;
	}
}

void ChordProParser::getChord(wstring &arg)
{
	m_Pos++;	// skip '['
	while (m_Pos < m_Input.c_str() + m_Input.size()) {
		if (*m_Pos == ']') {
			// Chord stop when ] is found 
			m_Pos++;	// skip ']'
			return;
		}
		arg += *m_Pos;
		m_Pos++;
	}
}

parsed_item_t ChordProParser::getDirective(wstring &arg)
{
	string directive_name;
	bool separator_found = false;

	m_Pos++;	// skip '{'
	while (m_Pos < m_Input.c_str() + m_Input.size()) {

		if (*m_Pos == '}') {
			// Directive end
			m_Pos++;		// skip '}'
			return getEnDirective(directive_name);
		}
		if (separator_found) {
			// fill argument
			arg += *m_Pos;
		}
		else {
			// fill directive name string till ':' is reached
			if (*m_Pos == ':') {
				separator_found = true;
			}
			else {
				directive_name += *m_Pos;
			}
		}
		m_Pos++;
	}

	return PARSED_ITEM_NONE;
}

void ChordProParser::getText(wstring &arg)
{
	arg += *m_Pos;
	m_Pos++;

	while (m_Pos < m_Input.c_str() + m_Input.size()) {

		if ( item_starting() != PARSED_ITEM_NONE)  {
			// Something different from normal text is starting
			return;
		}
		arg += *m_Pos;
		m_Pos++;
	}
}

const wchar_t *ParserLabel(parsed_item_t it)
{
	switch (it) {
	case PARSED_ITEM_NONE:						return L"None";
	case PARSED_ITEM_NEWLINE:					return L"Newline";
	case PARSED_ITEM_COMMENT:					return L"Comment";
	case PARSED_ITEM_CHORD:						return L"Chord";
	case PARSED_ITEM_TEXT:						return L"Text";

	// Directives
	case PARSED_ITEM_DIRECTIVE_NONE:			return L"Directive - None";
	case PARSED_ITEM_DIRECTIVE_NEW_SONG:		return L"Directive - New song";
	case PARSED_ITEM_DIRECTIVE_TITLE:			return L"Directive - Title";
	case PARSED_ITEM_DIRECTIVE_SUBTITLE:		return L"Directive - Subtitle";
	case PARSED_ITEM_DIRECTIVE_COMMENT:			return L"Directive - Comment";
	case PARSED_ITEM_DIRECTIVE_COMMENT_ITALIC:	return L"Directive - Comment italic";
	case PARSED_ITEM_DIRECTIVE_COMMENT_BOX:		return L"Directive - Comment box";
	case PARSED_ITEM_DIRECTIVE_CHORUS_START:	return L"Directive - Chorus start";
	case PARSED_ITEM_DIRECTIVE_CHORUS_END:		return L"Directive - Chorus end";
	case PARSED_ITEM_DIRECTIVE_TAB_START:		return L"Directive - Tab start";
	case PARSED_ITEM_DIRECTIVE_TAB_END:			return L"Directive - Tab end";
	case PARSED_ITEM_DIRECTIVE_DEFINE:			return L"Directive - Define";
	}
	return L"?";
}
