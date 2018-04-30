/****************************************************************************
**
** HTML writer.
**
****************************************************************************/

//Include only once
#ifndef CONSOLE_UTF8_H
#define CONSOLE_UTF8_H

//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

/* System Library Include
*/
#include <codecvt>
#include <list>
#include <sstream>
#include <string>

/* Application Local Include
*/
// No application library includes

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

/* It seems that you have to first convert from UTF-8 to UTF-16, and then use 
	wprintf or wcout to print the UTF-16-encoded text. 
	This isn’t optimal, but at least it seems to work.
*/
// override cout

class u8ostream
{
public:
	friend u8ostream& operator<< (u8ostream &u8out, const string &utf8_str);
};

u8ostream& operator<< (u8ostream &u8out, const string &utf8_str);

// insertion operator overload  for I/O manipulator like endl
u8ostream& operator<<(u8ostream& u8out, std::ostream& (*pf)(std::ostream&));


class u8istream
{
public:
	friend u8istream& operator>> (u8istream &u8in, string &utf8_str);
};

u8istream& operator>> (u8istream &u8in, string &utf8_str);


class UTF8
{
public:
	static void init();
public:
	static u8ostream cout;
	static u8istream cin;
};



#endif // CONSOLE_UTF8_H
