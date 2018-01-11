/****************************************************************************
**
** HTML writer.
**
****************************************************************************/

#ifndef HTML_WRITER_H
#define HTML_WRITER_H

//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

/* System Library Include
*/
#include <list>
#include <string>

/* Application Local Include
*/
#include "chordpro_parser.h"

using namespace std;

class HTMLWrite
{
public:
	HTMLWrite();
	size_t paint(ChordProParser *chproFile);

private:
	void reinit(void);
	void startline(void);
	void putlyrics(string scan);
	void putchord(string name);

private:
	int xLyrics;
	int xChords;
	int yLyrics;

	int xMax;
	int yMax;
};


#endif // HTML_WRITER_H
