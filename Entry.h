#ifndef _ENTRY_
#define _ENTRY_

#include <windows.h>
#include <string>

using namespace std;

struct Entry {
	int posSector;
	BYTE sector[512];
	string name;
	string status; // bit
	string content;
	long size;
	bool isFile;
	bool residentFlat;
};

#endif // !_ENTRY_
