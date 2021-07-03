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

struct MainEntry {
    string name;
    string type;
    string state;
    uint16_t attribute;
    uint16_t startCluster;
    uint16_t startSector;
    uint16_t size;

};

struct ExtraEntry {
    string extraName1;
    string extraName2;
    string extraName3;
};
#endif // !_ENTRY_
