#include "BSFAT32.h"

using namespace std;

//Convert to hex 
string HexConvert(BYTE sector[])
{
    stringstream hexSs;
    for (int i = 0; i < 512; i++)
        hexSs << hex << setw(2) << setfill('0') << (int)sector[i];
    return hexSs.str();
}

//Convert hex to dec
int DecConvert(string hex)
{
    unsigned long result = 0;

    for (int i = 0; i < hex.length(); i++) {
        if (hex[i] >= 48 && hex[i] <= 57)
        {
            result += (hex[i] - 48) * pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 65 && hex[i] <= 70) {
            result += (hex[i] - 55) * pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 97 && hex[i] <= 102) {
            result += (hex[i] - 87) * pow(16, hex.length() - i - 1);
        }
    }
    return result;
}

void type(string hex)
{
    string str = hex.substr(0x52 * 2, 16);
    for (int i = 0; i < 16; i += 2)
    {
        string sub = str.substr(i, 2);
        cout << char(DecConvert(sub));
    }
    cout << endl;
}

int changeData(string hex, int start, int numbit)
{
    string a = hex.substr(start, numbit);

    string b = "";

    for (int i = a.size() - 2; i >= 0; i -= 2)
    {
        b += a.substr(i, 2);

    }
    return DecConvert(b);
}

void BootSectorFAT32(BYTE sector[])
{
    string hex = HexConvert(sector);
    cout << "** Information is described in the Boot Sector: **" << endl;
    cout << "Type of FAT: ";
    type(hex);
    cout << "Number of bytes per sector: ";
    cout << changeData(hex, 0x0B * 2, 4) << endl;
    cout << "Number of sector per cluster: ";
    cout << changeData(hex, 0x0D * 2, 2) << endl;
    cout << "Number of sectors in the bootsector: ";
    cout << changeData(hex, 0x0E * 2, 4) << endl;
    cout << "Number of FAT table: ";
    cout << changeData(hex, 0x10 * 2, 2) << endl;
    cout << "Size of volume : ";
    cout << changeData(hex, 0x20 * 2, 8) << endl;
    cout << "Size of per FAT table: ";
    cout << changeData(hex, 0x24 * 2, 8) << endl;
    cout << "Begin cluster of RDET: ";
    cout << changeData(hex, 0x2C * 2, 8) << endl;
    cout << "Sector contains extra information ( about empty cluster ): ";
    cout << changeData(hex, 0x30 * 2, 4) << endl;
    cout << "Sector contains the save version of Boot Sector: ";
    cout << changeData(hex, 0x32 * 2, 4) << endl;
}