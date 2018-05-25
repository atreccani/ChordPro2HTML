//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

/* System Library Include
*/
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>

#include <fcntl.h>  
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#if defined(__linux__)
#include <dirent.h>
#include <sys/stat.h>
#endif

/* Application Local Include
*/
#include "file_system.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//                 C O N S T A N T S   D E F I N I T I O N S                //
//////////////////////////////////////////////////////////////////////////////

// No local constants


//////////////////////////////////////////////////////////////////////////////
//                  M A C R O S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


//////////////////////////////////////////////////////////////////////////////
//                    T Y P E S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////

// No local type definitions


//////////////////////////////////////////////////////////////////////////////
//                   L O C A L S    D E F I N I T I O N S                   //
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
//                    C L A S S    D E F I N I T I O N S                    //
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//                     P U B L I C   F U N C T I O N S                      //
//////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32) || defined(_WIN64)
const string getDirFiles(string folder, vector<FileInfo> &listing)
{
	FileInfo file;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd; 
	LARGE_INTEGER filesize;
	ostringstream fail;


	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	// converting from UTF-8 to UTF-16
	wstring str16 = converter.from_bytes(search_path);

	HANDLE hFind = ::FindFirstFile(str16.c_str(), &fd); 

	if(hFind == INVALID_HANDLE_VALUE) {
		fail << u8"Error(" << errno << ") opening " << folder << endl;
		return fail.str();
	}

	do {
		// read all (real) files in current folder
		// , delete '!' read other 2 default folder . and ..
		if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {

			// converting from UTF-16 to UTF-8
			file.name = converter.to_bytes(fd.cFileName);

			filesize.LowPart = fd.nFileSizeLow;
			filesize.HighPart = fd.nFileSizeHigh;
			file.size = filesize.QuadPart;

			listing.push_back(file);
		}
	}while(::FindNextFile(hFind, &fd)); 
	::FindClose(hFind); 
	return fail.str();
}
#endif

#if defined(__linux__)
const string getDirFiles(string folder, vector<FileInfo> &listing)
{
	FileInfo file;
	DIR *dp;
	struct dirent *dirp;
	ostringstream fail;
	struct stat buf;
	int exists;

	
	if((dp  = opendir(folder.c_str())) == NULL) {
		fail << "Error(" << errno << ") opening " << folder << endl;
		return fail.str();
	}

	while ((dirp = readdir(dp)) != NULL) {
		
		exists = stat(dirp->d_name, &buf);
		
		if (exists >= 0) {
			file.name = string(dirp->d_name);
			file.size = buf.st_size;
			listing.push_back(file);
		}
	}
	closedir(dp);
	return fail.str();
}
#endif

bool loadTextFile(string &dst, string &fileName)
{
	// Read file assumed to be UTF-8 encoded
	ifstream ifs(fileName);

	// Open the file for reading
	if (!ifs.is_open()) {
		wcout << "Unable to open file" << endl;
		return false;
	}

	stringstream ss;
	ss << ifs.rdbuf();

	dst = ss.str();

	return true;

}

