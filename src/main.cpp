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


void fill_chordpro_file_list(string pathIn, list <ChordProData> &list_ref)
{
	vector<FileInfo> filesInfo;

	// List of parsed elements
	ChordProData chord_pro_data;

	// List of all files in input directory (not necessarily all are chordpro files)
	const string err_str = getDirFiles(pathIn, filesInfo);

	if (!err_str.empty()) {
		UTF8::cout << err_str;
		return;
	}

	vector<FileInfo>::iterator it;
	for (it = filesInfo.begin(); it != filesInfo.end(); ++it) {

		// A valid ChordPro file it should be less than 100k (it's a plain text file!)
		if (it->size < (100 * 1024)) {

			// Load file in memory
			if (loadTextFile(chord_pro_data.m_Input, pathIn + "/" + it->name)) {
				// To be a valid ChordPro file it should contain title directive in first 1k
				ChordProParser *parser = new ChordProParser(chord_pro_data);
				if(parser->parseAll()) {
					list_ref.push_back(chord_pro_data);
				}
			}
		}
	}
}

/*
	Important note: all strings are represented internally in utf-8 coding.
	See http://www.nubaria.com/en/blog/?p=289.
	Strings can be seen correctly on Visual Studio watch appending ',s8' to	the variable name.
*/

int main(int argc, char *argv[])
{
	list <ChordProData> song_list;

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
	fill_chordpro_file_list("../../../default_in", song_list);

	// Print a list of found ChordPro files
	string size_str = to_string(song_list.size());
	UTF8::cout << "Found " << size_str << " ChordPro files:" << endl;
	
	list<ChordProData>::iterator it;
	for (it = song_list.begin(); it != song_list.end(); ++it) {
		UTF8::cout << " - " << it->m_Title << endl;
	}
	UTF8::cout << endl;

	if(!song_list.empty()) {
		ChordProData &act_song = song_list.front();
		UTF8::cout << act_song.m_Input << endl;

		act_song.removeMultipleSpaces();

		// Create the HTML file of the song
		// Set the destination .svg file name changing extension to the source .pro file name

		HTMLWriter writer("../../../default_out/test.html");

		writer.open();

		writer.paint(act_song);

		writer.close();

	}

	// Windows specific pause
	system("pause");

}

