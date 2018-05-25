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
#include "console_utf8.h"
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

HTMLWriter::HTMLWriter(const string &file_name)
{
	m_FileName = file_name;
}

void HTMLWriter::open(void)
{
	ofs.open(m_FileName, std::ofstream::out);

	// If the function fails to open a file, the failbit state flag is set for the stream 
	if (!ofs.is_open()) {
		UTF8::cout << "Fail opening file: " << m_FileName;
	}
}

size_t HTMLWriter::paint(ChordProData &src)
{
	song_element_t it;

	string value;

	if (!ofs.is_open()) {
		return 0;
	}

	/*
	<html>
		<head>
		<title>Nel blu dipinto di blu</title>
		<link href = "./cantalagioia.css" rel = "stylesheet" type = "text/css">
		</head>
	*/
	ofs << "<html>" << endl;
	ofs << "\t<head>" << endl;
	ofs << "\t\t<title>" << src.m_Title << "</title>" << endl;
	ofs << "\t</head>" << endl;

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

void HTMLWriter::close(void)
{
	ofs.close();
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

