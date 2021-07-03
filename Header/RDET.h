#ifndef _RDET_
#define _RDET_

#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#include "BSFAT32.h"
#include "Entry.h"

using namespace std;

class RDET
{
private:

    LPCWSTR driver;
    int startSectorOfRDET;
    int sectorPerClustor;

public:
    RDET(LPCWSTR driver)
    {
        this->driver = driver;
    }

public:

    void execute()
    {
        BYTE sector[512];
        ReadSector(driver, 0, sector);
        getStartSectorOfRDET(sector);
        getClustorPerSector(sector);

        int readPoint = 512 * startSectorOfRDET;
        showRDET(sector, readPoint);
    }

private:

    void getStartSectorOfRDET(BYTE sector[]);

    void getClustorPerSector(BYTE sector[]);

    string readName(string str, int size);

    int convertData(string str);

    MainEntry readMainEntry(string entry);

    ExtraEntry readExtraEntry(string entry);

    void showData(BYTE sector[512], MainEntry main);

    void showInfor(MainEntry main, vector<ExtraEntry> extra, int numOfExtraEntry);

    void showRDET(BYTE sector[512], int readPoint);
};

#endif // !_RDET_
