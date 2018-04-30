//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

/* System Library Include
*/
#include <fstream>

/* Application Local Include
*/
#include "../version.h"
#include "chordpro_parser.h"
#include "console_utf8.h"
#include "file_system.h"
#include "html_writer.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//                 C O N S T A N T S   D E F I N I T I O N S                //
//////////////////////////////////////////////////////////////////////////////

// No local constants


//////////////////////////////////////////////////////////////////////////////
//                  M A C R O S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

// No local macros


//////////////////////////////////////////////////////////////////////////////
//                    T Y P E S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

// No local type definitions


//////////////////////////////////////////////////////////////////////////////
//                     P U B L I C   F U N C T I O N S                      //
//////////////////////////////////////////////////////////////////////////////


void fill_chordpro_file_list(list <ChordProParser *> &list_ref)
{
	string pathIn = "../default_in";
	vector<FileInfo> filesInfo;

	// List of all files in input directory (not necessarily all are chordpro files)
	getDirFiles(pathIn, filesInfo);

	vector<FileInfo>::iterator it;
	for (it = filesInfo.begin(); it != filesInfo.end(); ++it) {

		// A valid ChordPro file it should be less than 100k (it's a plain text file!)
		if (it->size < (100 * 1024)) {
			ChordProParser *parser = new ChordProParser(pathIn + "/" + it->name);

			if (parser->loadFile()) {
				// To be a valid ChordPro file it should contain title directive in first 1k
				if (parser->parseTitle()) {
					list_ref.push_back(parser);
				}
			}
		}
	}
}

// Create the svg file for the given song
void createHTML(ChordProParser *act_song)
{
	// Set the destination .svg file name changing extension to the source .pro file name
	const string path = "../default_out/test.html";

	// List of parsed elements
	ChordProData chord_pro_data;

	// Parse all elements of the song
	act_song->parseAll(chord_pro_data);

	act_song->removeMultipleSpaces(chord_pro_data);


	HTMLWriter writer(path);

	writer.paint(chord_pro_data);


}


/*
	Important note: all strings are represented internally in utf-8 coding.
	See http://www.nubaria.com/en/blog/?p=289.
	Strings can be seen correctly on Visual Studio watch appending ',s8' to	the variable name.
*/

int main(int argc, char *argv[])
{
	list <ChordProParser *> song_list;

	UTF8::init();

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	// setvbuf(stdout, nullptr, _IOFBF, 1000);

	string test1 = u8"UTF-8 console test";
	string test2 = u8"This is a fi: ф";
	string test3 = u8"These are notes: ♩(quarter) ♪(eighth) ♫(beamed eighth) ♬(beamed sixteenth)";
	UTF8::cout << test1 << endl;
	UTF8::cout << test2 << endl;
	UTF8::cout << test3 << endl;
	UTF8::cout << endl;

	// Set the logging file
	// check which a path to file you use 
	wofstream logFile(
		"../default_out/log.txt", ios_base::out | ios_base::app);

	// Fill a list of valid ChordPro files, each file will also be fully loaded into memory (as an utf8 encoded string)
	fill_chordpro_file_list(song_list);

	// Print a list of found ChordPro files
	string size_str = to_string(song_list.size());
	UTF8::cout << "Found " << size_str << " ChordPro files:" << endl;
	
	list<ChordProParser *>::iterator it;
	for (it = song_list.begin(); it != song_list.end(); ++it) {
		UTF8::cout << " - " << (*it)->title() << endl;
	}
	UTF8::cout << endl;

	ChordProParser *act_song = song_list.front();
	UTF8::cout << act_song->m_Input << endl;

	// Create the svg file of the song
	createHTML(act_song);

	// Windows specific pause
	system("pause");

}

