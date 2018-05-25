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
	ChordProParser(ChordProData &dst) : m_dst(dst)
	{
	}

	bool parseAll();
	void reinit(void);
	song_element_t get(string &arg);



private:
	bool isLineBegin(void);
	song_element_t item_starting();
	void getComment(string &arg);
	void getChord(string &arg);
	song_element_t getDirective(string &arg);
	void getText(string &arg);

private:
	const char				*m_Pos;
	ChordProData			&m_dst;

};

#endif // CHORDPROPARSER_H
