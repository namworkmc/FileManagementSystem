#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "ReadSector.h"

using namespace std;

string HexConvert(BYTE sector[]);

int DecConvert(string hex);

void type(string hex);

void changeData(string hex, int start, int numbit);

void BootSectorFAT32(BYTE sector[]);