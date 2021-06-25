#include "MFT.h"

void ReadSystemFile()
{
    fstream f;
    f.open("./SystemFile.txt", ios::in);

    while (!f.eof())
    {
        f >> systemFile[systemFileSize];
        systemFileSize++;
    }

    f.close();        
}

string DecToBinary(int n)
{
    // array to store binary number
    string binary = "";
    
    // counter for binary array
    int i = 0;
    
    while (n > 0) {        
        // storing remainder in binary array
        binary += to_string(n%2);
        n = n / 2;
        i++;
    }
    
    return binary;
}

bool checkEndEntry(LPCWSTR  drive, int readPoint)
{
    BYTE sector[512];
    int chain = 0; 
    
    ReadSector(drive, readPoint, sector);

    while (subB_dec(sector, 0, 50) == 0)
    {
        chain++;
        
        if (chain == 100)
            return true;

        readPoint += 2 * bps;

        ReadSector(drive, readPoint, sector);
    }

    if (subB(sector, 0x03, 0x0a) == "NTFS    ")
        return true;
    
    return false;
}

int AttributeType(BYTE sector[512], int startOffset)
{
    return subB_dec(sector, startOffset, startOffset + 3);
}

int AttributeLength(BYTE sector[512], int startOffset) //include header
{
    return subB_dec(sector, startOffset + 4, startOffset + 7);
}

int MoveToAttribute(BYTE sector[512], int code)
{
    int offset = 56; // begin offset of $STANDARD_INFORMATION

    int AType = AttributeType(sector, offset); // attribute type
    int ALength = AttributeLength(sector, offset); // attribute length

    if (ALength > 512 || ALength <= 22)
        return 512;

    while (AType != code && ALength > 22 && ALength < 512 && offset <= 512) // move to attribute $FILE_NAME
    {
        offset += ALength;

        AType = AttributeType(sector, offset);
        ALength = AttributeLength(sector, offset);
    }

    if (AType < 16)
        return 512;

    return offset;
}

string AttributeName(BYTE sector[512])
{
    cout.precision(0);

    int offset = MoveToAttribute(sector, 48);
    
    if (offset == 512)
        return "";

    int nameLength = subB_dec(sector, offset + 88, offset + 88);
    
    string name = subB(sector, offset + 90, offset + 90 + nameLength * 2 - 1);
    string temp = "";

    for (int i = 0; i < name.length(); i++) // delete '\0' char
    {
        if (name[i] != '\0')
            temp += name[i];
    }
    
    name = temp;

    for (int i = 0; i < systemFileSize; i++) // check whether this file is system file
        if (name.find(systemFile[i]) >=0 && name.find(systemFile[i]) < name.length())
            return "";

    return name;
}

bool ResidentData(BYTE sector[512])
{
    int offset = MoveToAttribute(sector, 128); // $DATA attribute
    int non_ResidentFlat = subB_dec(sector, offset + 8, offset + 8);

    return !non_ResidentFlat;
}

long SizeData(BYTE sector[512])
{
    //cout <<"resident:"<< ResidentData(sector) << endl;

    int offset = MoveToAttribute(sector, 128); // $DATA attribute

    if (ResidentData(sector))
    {
        return subB_dec(sector, offset + 16, offset + 19);
    }
    else
        return subB_dec(sector, offset + 40, offset + 47);
}

int OffsetOfAttributeData(BYTE sector[512], int startOffset)
{
    return subB_dec(sector, startOffset + 20, startOffset + 21);
}

string GetEntryStatus(BYTE sector[512])
{
    string fileAttribute, filePermission;

    // move to Offset "File permission"
    int offset = 0;

    offset = MoveToAttribute(sector, 16);

    if (offset >= 512)
        filePermission = "";
    else
    {
        offset += 56;

        filePermission = DecToBinary(subB_dec(sector, offset, offset + 3)); // Transform "file attribute" value to binary
    }
    
    // move to Offset "File attribute"
    offset = 0;

    offset = MoveToAttribute(sector, 48);

    if (offset >= 512)
        fileAttribute = "";
    else
    {
        offset += OffsetOfAttributeData(sector, offset); // move to offset begin of data of attribute

        offset += 56; // move to Offset "File attribute"

        fileAttribute = DecToBinary(subB_dec(sector, offset, offset + 3)); // Transform "file attribute" value to binary
    }

    string file = "";
    string min = "";

    if (fileAttribute.length() > filePermission.length())
    {
        file = fileAttribute;
        min = filePermission;
        
        if (filePermission.length() == 0)
            return fileAttribute;
    }
    else
    {
        file = filePermission;
        min = fileAttribute;
        
        if (fileAttribute.length() == 0)
            return filePermission;
    }

    for (int i = 0; i < min.length(); i++)
    {
        if (fileAttribute[i] == filePermission[i])
            file[i] = fileAttribute[i];
        else
            file[i] = '1';
    }
    
    return file;
}

string ReadStatus(string file)
{
    string stt = "";
    int idx = 0;

    for (int i = 0; i < file.length(); i++)
    {
        if (file[i] == '1')
        {
            if (idx != 0)
                stt+= ", ";

            idx++;

            switch (i)
            {
            case 0:
                stt += "Read-only";
                break;
            case 1:
                stt += "Hidden";
                break;
            case 2:
                stt += "System";
                break;
            case 5:
                stt += "Archive";
                break;
            case 6:
                stt += "Device";
                break;
            case 7:
                stt += "Normal";
                break;
            case 8:
                stt += "Temporary";
                break;
            case 9:
                stt += "Sparse File";
                break;
            case 10:
                stt += "Reparse Point";
                break;
            case 11:
                stt += "Compress";
                break;
            case 12:
                stt += "Offline";
                break;
            case 13:
                stt += "Not Content Indexed";
                break;
            case 14:
                stt += "Encrypted";
                break;
            case 28:
                stt += "Directory";
                break;
            case 29:
                stt += "Index View";
                break;
            }
        }
    }
    return stt;
}

bool IsWhatEntry(BYTE sector[512],int bit)
{
    int offset = 0;

    offset = MoveToAttribute(sector, 48);

    if (offset >= 512)
        return false;

    offset += OffsetOfAttributeData(sector, offset); // move to offset begin of data of attribute

    offset += 56; // move to Offset "File attribute"

    string fileAttribute = DecToBinary(subB_dec(sector, offset, offset + 3)); // Transform "file attribute" value to binary
    
    offset = 0;

    // bit 0: Read-only
    // bit 1: hidden
    // bit 2: system
    // bit 5: archive...

    return fileAttribute[bit] == '1';
}

Entry AnalysisSector(LPCWSTR  drive, int readPoint) {
    
    Entry e;

    ReadSector(drive, readPoint, e.sector);

    string file = GetEntryStatus(e.sector);

    //cout << "readpoint: " << readPoint << endl;
    if (!IsWhatEntry(e.sector, 2) && AttributeName(e.sector) != "" && file[2] != '1')
    {
        e.name = AttributeName(e.sector);
        e.status = GetEntryStatus(e.sector);
        e.residentFlat = ResidentData(e.sector);

        if ((e.status.length() >= 28 && !IsWhatEntry(e.sector, 28)) || e.status.length() < 28) // if not directory file -> show size
            e.size = SizeData(e.sector);
        else
            e.size = 0;

        e.posSector = readPoint / 512;
        e.isFile = true;
    }
    else
        e.isFile = false;
    
    return e;
}

void ShowEntry(LPCWSTR  drive, Entry e)
{
    cout << "---------------------------" << endl;
    cout << "Name: "<<e.name << " " << endl;
    cout << "Status: "<< ReadStatus(e.status) << " " << endl;

    if (e.size != 0) // not a folder
        ReadSize(e.size);

    cout << "Sector: " << e.posSector << endl;
}

void FolderTree(LPCWSTR  drive, int readPoint, int r_bps, int r_spc, int r_lcn)
{
    bps = r_bps;
    spc = r_spc;
    lcn = r_lcn;
    int startOffset = lcn * spc * bps; // start sector is the sector of $MFT file

    int index = 0;

    ReadSystemFile();
    
    // use code below to analysis each sector
    // AnalysisSector(drive, 69566464);
    Entry entries[500];
    int numEntryFile = 0;
    while (true)
    {
        system("cls");

        while (!checkEndEntry(drive, startOffset))
        {
            if (AnalysisSector(drive, startOffset).isFile == true)
                entries[numEntryFile++] = AnalysisSector(drive, startOffset);

            startOffset += 2 * bps; // move to next sector
        }

        for (int i = 0; i < numEntryFile; i++)
            ShowEntry(drive, entries[i]);

        string name;
        cout << "\n\n=====================================" << endl;
        cout << "Read file: ";
        
        getline(cin, name);

        ReadFileText(drive, name, entries, numEntryFile);
    }
}

// Hex to time Stamp
// 15EA9228A665D701
// 20 june 2021 7:30