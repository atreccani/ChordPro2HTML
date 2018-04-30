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

#include <windows.h>
#include <fcntl.h>  
#include <io.h>  
#include <stdio.h>
#include <strsafe.h>

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
void getDirFiles(string folder, vector<FileInfo> &listing)
{
	FileInfo file;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd; 
	LARGE_INTEGER filesize;


	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	// converting from UTF-8 to UTF-16
	wstring str16 = converter.from_bytes(search_path);

	HANDLE hFind = ::FindFirstFile(str16.c_str(), &fd); 

	if(hFind != INVALID_HANDLE_VALUE) { 
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
	}
}

