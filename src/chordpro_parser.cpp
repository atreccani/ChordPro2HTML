/****************************************************************************
**
** ChordPro parser.
**
****************************************************************************/

#include "chordpro_parser.h"

#define DIM(a) (sizeof(a) / sizeof(a[0]))

typedef struct {
	parsed_item_t	id;
	const char		*label_long;
	const char		*label_short;
} directive_define_t;

static const directive_define_t directive_list[] = {

	// Preamble directives
	{ PARSED_ITEM_DIRECTIVE_NEW_SONG		, "new_song"			, "ns" },

	// Metadata directives
	{ PARSED_ITEM_DIRECTIVE_TITLE			, "title"				, "t" },
	{ PARSED_ITEM_DIRECTIVE_SUBTITLE		, "subtitle"			, "st" },

	// Formatting directives
	{ PARSED_ITEM_DIRECTIVE_COMMENT			, "comment"				, "c" },
	{ PARSED_ITEM_DIRECTIVE_COMMENT_ITALIC	, "comment_italic"		, "ci" },
	{ PARSED_ITEM_DIRECTIVE_COMMENT_BOX		, "comment_box"			, "cb" },
	{ PARSED_ITEM_DIRECTIVE_CHORUS_START	, "start_of_chorus"		, "soc" },
	{ PARSED_ITEM_DIRECTIVE_CHORUS_END		, "end_of_chorus"		, "eoc" },
	{ PARSED_ITEM_DIRECTIVE_TAB_START		, "start_of_tab"		, "sot" },
	{ PARSED_ITEM_DIRECTIVE_TAB_END			, "end_of_tab"			, "eot" },
	{ PARSED_ITEM_DIRECTIVE_DEFINE			, "define"				, NULL }
};

string &ChordProParser::operator=(const string &other)
{
	return string::operator=(other);
}

bool ChordProParser::parseTitle(void)
{
	string dir_value;
	parsed_item_t it;

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

string &ChordProParser::title()
{
	return m_Title;
}

void ChordProParser::parseAll(void)
{
	// Create list of parsed data
	ParsedSongItem song_elem;

	reinit();
	while ((song_elem.id = get(song_elem.value)) != PARSED_ITEM_NONE) {
		m_AllItems.push_back(song_elem);
	}

}

void ChordProParser::removeMultipleSpaces(void)
{
	bool newline_found = false;

	list<ParsedSongItem>::iterator it = m_AllItems.begin();
	while (it != m_AllItems.end()) {
		if (newline_found) {
			if (
				(it->id == PARSED_ITEM_NEWLINE) || 
				((it->id == PARSED_ITEM_TEXT) && (it->value.empty()))	/* nothing but whitespace */
			) {
				string Log = string("Removing Id: ") + ParserLabel(it->id) + string(", Val: ") + it->value;

				it = m_AllItems.erase(it);
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
		m_AllItems.pop_back();
	}
}

list <ParsedSongItem> &ChordProParser::all()
{
	return m_AllItems;
}

void ChordProParser::reinit(void)
{
	m_Pos = (char *)c_str();
}

parsed_item_t ChordProParser::get(string &arg)
{
	if (m_Pos >= c_str() + size()) {
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
	if (m_Pos == c_str()) {
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

parsed_item_t ChordProParser::parseDirective(string &label)
{
	// Look for metadata directives entry
	for (int ii = 0; ii < DIM(directive_list); ii++) {
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

void ChordProParser::getComment(string &arg)
{
	m_Pos++;
	while (m_Pos < c_str() + size()) {
		if (*m_Pos == '\n') {
			// Comment stops at the end of line
			m_Pos++;	// skip '\n'
			return;
		}
		arg += *m_Pos;
		m_Pos++;
	}
}

void ChordProParser::getChord(string &arg)
{
	m_Pos++;	// skip '['
	while (m_Pos < c_str() + size()) {
		if (*m_Pos == ']') {
			// Chord stop when ] is found 
			m_Pos++;	// skip ']'
			return;
		}
		arg += *m_Pos;
		m_Pos++;
	}
}

parsed_item_t ChordProParser::getDirective(string &arg)
{
	string directive_name;
	bool separator_found = false;

	m_Pos++;	// skip '{'
	while (m_Pos < c_str() + size()) {

		if (*m_Pos == '}') {
			// Directive end
			m_Pos++;		// skip '}'
			return parseDirective(directive_name);
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

void ChordProParser::getText(string &arg)
{
	arg += *m_Pos;
	m_Pos++;

	while (m_Pos < c_str() + size()) {

		if ( item_starting() != PARSED_ITEM_NONE)  {
			// Something different from normal text is starting
			return;
		}
		arg += *m_Pos;
		m_Pos++;
	}
}

const char *ParserLabel(parsed_item_t it)
{
	switch (it) {
	case PARSED_ITEM_NONE:						return "None";
	case PARSED_ITEM_NEWLINE:					return "Newline";
	case PARSED_ITEM_COMMENT:					return "Comment";
	case PARSED_ITEM_CHORD:						return "Chord";
	case PARSED_ITEM_TEXT:						return "Text";

	// Directives
	case PARSED_ITEM_DIRECTIVE_NONE:			return "Directive - None";
	case PARSED_ITEM_DIRECTIVE_NEW_SONG:		return "Directive - New song";
	case PARSED_ITEM_DIRECTIVE_TITLE:			return "Directive - Title";
	case PARSED_ITEM_DIRECTIVE_SUBTITLE:		return "Directive - Subtitle";
	case PARSED_ITEM_DIRECTIVE_COMMENT:			return "Directive - Comment";
	case PARSED_ITEM_DIRECTIVE_COMMENT_ITALIC:	return "Directive - Comment italic";
	case PARSED_ITEM_DIRECTIVE_COMMENT_BOX:		return "Directive - Comment box";
	case PARSED_ITEM_DIRECTIVE_CHORUS_START:	return "Directive - Chorus start";
	case PARSED_ITEM_DIRECTIVE_CHORUS_END:		return "Directive - Chorus end";
	case PARSED_ITEM_DIRECTIVE_TAB_START:		return "Directive - Tab start";
	case PARSED_ITEM_DIRECTIVE_TAB_END:			return "Directive - Tab end";
	case PARSED_ITEM_DIRECTIVE_DEFINE:			return "Directive - Define";
	}
	return "?";
}
