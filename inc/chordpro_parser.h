/****************************************************************************
**
** ChordPro parser.
**
****************************************************************************/

//Include only once
#ifndef CHORDPROPARSER_H
#define CHORDPROPARSER_H

//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

/* System Library Include
*/
#include <list>
#include <string>

/* Application Local Include
*/
#include "chordpro_data.h"

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

// No public types


//////////////////////////////////////////////////////////////////////////////
//                   G L O B A L   V A R I A B L E S                        //
//////////////////////////////////////////////////////////////////////////////

// No global variables


//////////////////////////////////////////////////////////////////////////////
//                    C L A S S    D E C L A R A T I O N                    //
//////////////////////////////////////////////////////////////////////////////

class ChordProParser
{
public:
	ChordProParser(wstring &file_name) {
		m_FileName = file_name;
	}

	bool loadFile();

	bool parseTitle(void);
	wstring &title();
	void parseAll(ChordProData &dst);
	void removeMultipleSpaces(ChordProData &lst);
	void reinit(void);
	parsed_item_t get(wstring &arg);



private:
	bool isLineBegin(void);
	parsed_item_t item_starting();
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

};

extern const wchar_t *ParserLabel(parsed_item_t it);

#endif // CHORDPROPARSER_H
