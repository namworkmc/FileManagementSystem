#include "readOperator.h"
#include "MFT.h"

void ReadSize(int size)
{
    cout.setf(ios::fixed, ios::floatfield); // take 3 digit-
    cout.precision(3);                      // -behind comma

    int level = 0;
    float n = size;

    while (size / 1024 != 0)
    {
        n = size / 1024.0;
        level++;
        size = n;
    }

    if (round(n) == n)
        cout.precision(0);

    cout << "Size: " << n;

    switch (level)
    {
    case 0:
        cout << " B";
        break;
    case 1:
        cout << " KB";
        break;
    case 2:
        cout << " MB";
        break;
    case 3:
        cout << " GB";
        break;
    }
    cout << endl;
}

string ReadContent(LPCWSTR  drive, Entry e)
{
    int offset = MoveToAttribute(e.sector, 128);
    string s = "";

    if (e.residentFlat)
    {
        int length = AttributeLength(e.sector, offset + 16 - 4);
        int limited = 512 - offset - 24;

        if (limited >= length)
            return subB(e.sector, offset + 24, offset + 24 + length - 1);
        else
        {
            s = subB(e.sector, offset + 24, 512);

            BYTE sector[512];

            ReadSector(drive, (e.posSector + 1) * 512, sector);

            s += subB(sector, 0, length - limited);
        }
    }
    else {
        int size = subB_dec(e.sector, offset + 64, offset + 64);
        char hexString[3];

        _itoa_s(size, hexString, 16);

        //(int)hexString[0] - 48 char to int
        int FCSize = (int)hexString[0] - 48;
        int CCSize = (int)hexString[1] - 48;

        int clusterCount = subB_dec(e.sector, offset + 65, offset + 65 + CCSize - 1);
        int firstCluster = subB_dec(e.sector, offset + 67, offset + 67 + FCSize - 1); // cluster -> sector = cluster * spc
        int FCluster = 0;

        // use code line below to view DATA RUN
        // cout << "size: " << FCSize << CCSize << " - cluster count: " << clusterCount << " - first cluster: " << firstCluster << endl;

        offset = offset + 67 + FCSize;

        BYTE sector[512];
        int endMark = -1;
        int time = 0;
        int count = 1;

        while (endMark == -1)
        {
            while (count <= clusterCount * 8 || endMark == -1)
            {
                ReadSector(drive, ((FCluster + firstCluster) * spc + time) * bps, sector);

                for (int i = 0; i < 512; i++)
                    if (subB_dec(sector, i, i) == 0)
                        endMark = i;

                if (endMark == -1)
                    s += subB(sector, 0x00, 0xFF);
                else
                    s += subB(sector, 0x00, endMark);

                time++;
                count++;
            }

            endMark = -1;
            time = 0;
            count = 1;

            size = subB_dec(e.sector, offset, offset);

            if (size == 0)
                break;

            _itoa_s(size, hexString, 16);

            FCSize = (int)hexString[0] - 48;
            CCSize = (int)hexString[1] - 48;

            FCluster += firstCluster;

            clusterCount = subB_dec(e.sector, offset + 1, offset + 1 + CCSize - 1);
            firstCluster = subB_dec(e.sector, offset + 1 + CCSize, offset + 1 + CCSize + FCSize - 1); // cluster -> sector = cluster * spc
            offset = offset + 1 + CCSize + FCSize;

            // use code line below to view DATA RUN
            //cout << "size: " << FCSize << CCSize << " - cluster count: " << clusterCount << " - first cluster: " << firstCluster << endl;
        }
    }

    return s;
}

void ReadFileText(LPCWSTR  drive, string name, Entry entries[500], int numEntryFile)
{
    bool fileExist = false;

    system("cls");

    for (int i = 0; i < numEntryFile; i++)
        if (entries[i].name == name)
        {
            fileExist = true;

            ShowEntry(drive, entries[i]);

            if (entries[i].name.find(".txt") >= 0 && entries[i].name.find(".txt") < entries[i].name.length())
                cout << "Content: " << ReadContent(drive, entries[i]) << endl;
            else
                cout << "Conten: Su dung phan mem thich hop" << endl;
        }

    if (!fileExist)
    {
        cout << "Name: " << name << endl;
        cout << "File does not exist !!" << endl;
    }

    cout << endl;
    system("pause");
}