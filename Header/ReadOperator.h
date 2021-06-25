#ifndef _READ_OPERATOR_
#define _READ_OPERATOR_

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <iomanip>

#include "Entry.h"

using namespace std;

void ReadSize(int size);

string ReadContent(LPCWSTR  drive, Entry e);

void ReadFileText(LPCWSTR  drive, string name, Entry entries[500], int numEntryFile);

#endif // !_READ_OPERATOR_