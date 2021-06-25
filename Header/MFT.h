#ifndef _MFT_
#define _MFT_

#include <iostream>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <iomanip>
#include <string>
#include <math.h>
#include <fstream>

#include "NTFSPartitionBootSector.h"
#include "ReadSector.h"
#include "Entry.h"
#include "ReadOperator.h"

using namespace std;

static int bps = 0; // byte per sector
static int spc = 0; // sector per cluster
static int lcn = 0; // logical cluster number

static bool EndFlat = false; // mark of end file
static string systemFile[100];
static int systemFileSize = 0;
//static int numEntryFile = 0;
//static  Entry entry[500];

string DecToBinary(int n);

Entry AnalysisSector(LPCWSTR  drive, int readPoint);

bool checkEndEntry(LPCWSTR  drive, int readPoint);

void ShowEntry(LPCWSTR  drive, Entry e);

void FolderTree(LPCWSTR  drive, int readPoint, int bps, int spc, int lcn);

long SizeData(BYTE sector[512]);

int OffsetOfAttributeData(BYTE sector[512], int startOffset);

int AttributeType(BYTE sector[512], int startOffset); // check in Decimal

int AttributeLength(BYTE sector[512], int startOffset);

string AttributeName(BYTE sector[512]);

bool IsWhatEntry(BYTE sector[512],int bit);

string GetEntryStatus(BYTE sector[512]);

string ReadStatus(string file);

int MoveToAttribute(BYTE sector[512], int code);

#endif // !_MFT_