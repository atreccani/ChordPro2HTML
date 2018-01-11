#include <codecvt>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../version.h"
#include "chordpro_parser.h"
#include "html_writer.h"

bool load_file(string &file_name, ChordProParser *parser)
{
	// Read file in UTF-8
	wifstream wif(file_name);
	wif.imbue(std::locale(locale::empty(), new codecvt_utf8<wchar_t>));

	// Open the file for reading
	if (!wif.is_open()) {
		cout << "Unable to open file" << endl;
	}

	std::wstringstream wss;
	wss << wif.rdbuf();

	/*	To be a valid ChordPro file it should be
	- less than 100k (it's a plain text file!)
	- Contain title directive in first 1k
	*/
	if (wif.tellg() > (100 * 1024)) {
		return false;
	}

	if (!parser->parseTitle()) {
		return false;
	}

	return true;
}

void getDir(const char* d, list<string> & f)
{
	FILE* pipe =  NULL;
	string pCmd = "dir /B /S " + string(d);
	char buf[256];

	if( NULL == (pipe = _popen(pCmd.c_str(),"rt")))
	{
		cout<<"Shit"<<endl;
		return;
	}

	while (!feof(pipe))
	{
		if(fgets(buf,256,pipe) != NULL)
		{
			f.push_back(string(buf));
		}

	}

	_pclose(pipe);

}



void fill_chordpro_file_list(list <ChordProParser *> &list_ref)
{
	list<string> files;

	getDir("../default_in", files);

	for (int i = 0; i < files.size(); ++i) {
		string sName = files.at(i).filePath();
		ChordProParser *parser = new ChordProParser();

		if (load_file(sName, parser)) {
			list_ref.append(parser);
		}
	}
}

// Create the svg file for the given song
void createSVG(ChordProParser *act_song)
{
	// Set the destination .svg file name changing extension to the source .pro file name
	const string path = act_song->m_fileinfo.path() + "/" + act_song->m_fileinfo.completeBaseName() + "_pre.svg";

	// Parse all elements of the song
	act_song->parseAll();
	act_song->removeMultipleSpaces();


	HTMLWrite painter;


	// get actual size QPaintDevice 
	stdout << "generator.width:  " << generator.width() << endl;
	stdout << "generator.height: " << generator.height() << endl;

	// painter.scale(0.5, 0.5);
	paint_size = painter.paint(act_song);
	painter.end();

}


int main(int argc, char *argv[])
{
	list <ChordProParser *> song_list;

	// Set the logging file
	// check which a path to file you use 
	logFile.reset(new QFile("../Work/log.txt"));
	// Open the file logging
	logFile.data()->open(QFile::Append | QFile::Text);

	// Windows specific: use Code Page 850 (Western Europe) for console writing
	stdout.setCodec("CP-850");

	// Fill a list of valid ChordPro files, each file will also be fully loaded into memory (as a string)
	fill_chordpro_file_list(song_list);

	// Print a list of found ChordPro files
	stdout << "Found " << song_list.size() << " ChordPro files:" << endl;
	for (int ii = 0; ii < song_list.size(); ii++) {
		stdout << " - " << song_list[ii]->title() << endl;
	}
	stdout << endl;

	ChordProParser *act_song = song_list[0];
	stdout << *act_song << endl;

	// Create the svg file of the song
	createSVG(act_song);

	// Windows specific pause
	system("pause");

}

