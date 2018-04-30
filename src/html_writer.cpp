//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

/* System Library Include
*/
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>

/* Application Local Include
*/
#include "chordpro_parser.h"
#include "html_writer.h"

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

HTMLWriter::HTMLWriter(const string &file_name)
{
	m_FileName = file_name;
}

size_t HTMLWriter::paint(ChordProData &src)
{
	song_element_t it;

	string value;

	// both of these assume that the character can be represented with
	// a single char in the execution encoding
	char32_t b = U'\U00000444';
	char32_t a = 'ф'; // this line additionally assumes that the source character encoding supports 

	char utf8String[] = u8"hello"; //UTF-8 encoding.

	std::cout << L"Hello, ф or \u0444!\n";



	writeUtf8("Ciao2");

	reinit();

	for (const chordpro_element_t &it : src.elements) {

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

void HTMLWriter::reinit()
{
	xLyrics = 0;
	xChords = 0;
	yLyrics = 0;

	xMax = 0;
	yMax = 0;
}

void HTMLWriter::startline()
{
	xLyrics = 0;
	xChords = 0;

	yLyrics += LINE_SPACING;

	if (yLyrics > yMax) {
		yMax = yLyrics;
	}
}

void HTMLWriter::putlyrics(string scan)
{
	// Print Lyrics

	if (xLyrics > xMax) {
		xMax = xLyrics;
	}
}

void HTMLWriter::putchord(string name)
{
	if (xChords < xLyrics) {
		// align chord coordinate to lyrics 
		xChords = xLyrics;
	}

	if (xChords > xMax) {
		xMax = xChords;
	}
}

// To write std::u32string as utf8 text files:

// Write file in UTF-8
void HTMLWriter::writeUtf8(string str)
{
	// std::locale loc (std::locale(), new std::codecvt_utf8<wchar_t>);
	// std::basic_ofstream<wchar_t> ofs (m_FileName);
	// ofs.imbue(loc);

	// string is assumed to be UTF-8 encoded already
	std::ofstream ofs (m_FileName);
	ofs << str;
}

