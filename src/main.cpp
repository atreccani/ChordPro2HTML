#include <fstream>
#include <iostream>
#include <vector>

#include <windows.h>
#include <fcntl.h>  
#include <io.h>  
#include <stdio.h>
#include <strsafe.h>

#include "../version.h"
#include "chordpro_parser.h"
#include "html_writer.h"



using namespace std;

typedef struct {
	wstring		name;
	uint64_t	size;
} FileInfo;

void getDirFiles(wstring folder, vector<FileInfo> &listing)
{
	FileInfo file;
	wstring search_path = folder + L"/*.*";
	WIN32_FIND_DATA fd; 
	LARGE_INTEGER filesize;

	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd); 
	if(hFind != INVALID_HANDLE_VALUE) { 
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
				file.name = fd.cFileName;

				filesize.LowPart = fd.nFileSizeLow;
				filesize.HighPart = fd.nFileSizeHigh;
				file.size = filesize.QuadPart;

				listing.push_back(file);
			}
		}while(::FindNextFile(hFind, &fd)); 
		::FindClose(hFind); 
	}
}

void fill_chordpro_file_list(list <ChordProParser *> &list_ref)
{
	wstring pathIn = L"../default_in";
	vector<FileInfo> filesInfo;

	// List of all files in input directory (not necessarily all are chordpro files)
	getDirFiles(pathIn, filesInfo);

	std::vector<FileInfo>::iterator it;
	for (it = filesInfo.begin(); it != filesInfo.end(); ++it) {

		// A valid ChordPro file it should be less than 100k (it's a plain text file!)
		if (it->size < (100 * 1024)) {
			ChordProParser *parser = new ChordProParser(pathIn + L"/" + it->name);

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
void createSVG(ChordProParser *act_song)
{
	// Set the destination .svg file name changing extension to the source .pro file name
	const string path = "../default_out/test.html";

	// Parse all elements of the song
	act_song->parseAll();
	act_song->removeMultipleSpaces();


	HTMLWrite writer;


}

int main(int argc, char *argv[])
{
	list <ChordProParser *> song_list;

	// set output console in unicode mode (UTF16)
	_setmode(_fileno(stdout), _O_WTEXT);


	// Set the logging file
	// check which a path to file you use 
	std::wofstream logFile(
		"../default_out/log.txt", std::ios_base::out | std::ios_base::app);

	// Fill a list of valid ChordPro files, each file will also be fully loaded into memory (as a wstring)
	fill_chordpro_file_list(song_list);

	// Print a list of found ChordPro files
	wcout << L"Found " << song_list.size() << L" ChordPro files:" << endl;
	
	list<ChordProParser *>::iterator it;
	for (it = song_list.begin(); it != song_list.end(); ++it) {
		wcout << " - " << (*it)->title() << endl;
	}
	wcout << endl;

	ChordProParser *act_song = song_list.front();
	wcout << act_song->m_Input << endl;

	// Create the svg file of the song
	createSVG(act_song);

	// Windows specific pause
	system("pause");

}

