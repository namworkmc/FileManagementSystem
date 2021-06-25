#ifndef _READ_SECTOR_
#define _READ_SECTOR_

#include <windows.h>
#include <stdio.h>

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512]);

#endif // !_READ_SECTOR_