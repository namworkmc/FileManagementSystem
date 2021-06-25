#ifndef _NTFS_PARTITION_BOOT_SECTOR_
#define _NTFS_PARTITION_BOOT_SECTOR_

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

string subB(BYTE sector[], int x, int y);

long subB_dec(BYTE sector[], int x, int y);

void subB_hex(BYTE sector[], int x, int y);

// sau lenh ReadSector(...) goi asNTFS(sector);
void asNTFS(BYTE s[512], int& bps, int& spc, int& lcn);

#endif // !_NTFS_PARTITION_BOOT_SECTOR_
