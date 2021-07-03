#include "RDET.h"

void RDET::getStartSectorOfRDET(BYTE sector[])
{
	string hex = HexConvert(sector);
	startSectorOfRDET = changeData(hex, 0x0E * 2, 4) + changeData(hex, 0x10 * 2, 2) * changeData(hex, 0x24 * 2, 8);
}

void RDET::getClustorPerSector(BYTE sector[])
{
	string hex = HexConvert(sector);
	sectorPerClustor = changeData(hex, 0x0D * 2, 2);
}

string RDET::readName(string str, int size)
{
    stringstream name;
    for (int i = 0; i < size * 2; i += 2)
    {
        string sub = str.substr(i, 2);
        char temp = char(DecConvert(sub));
        if (temp != ' ')
            name << temp;
    }
    return name.str();
}

int RDET::convertData(string str)
{
    string b = "";

    for (int i = str.size() - 2; i >= 0; i -= 2)
        b += str.substr(i, 2);

    return DecConvert(b);
}

MainEntry RDET::readMainEntry(string entry)
{
    MainEntry mainentry;
    mainentry.name = readName(entry.substr(0x00 * 2, 8 * 2), 8);
    mainentry.startCluster = convertData(entry.substr(0x14 * 2, 2 * 2)) + convertData(entry.substr(0x1A * 2, 2 * 2));
    mainentry.startSector = startSectorOfRDET + (mainentry.startCluster - 2) * sectorPerClustor;
    mainentry.size = convertData(entry.substr(0x1C * 2, 4 * 2));
    mainentry.attribute = convertData(entry.substr(0x0B * 2, 2));

    if (entry.substr(0x00 * 2, 2) == "e5")
        mainentry.state = "Deleted";
    else
        mainentry.state = "Exist";

    if (mainentry.attribute == 32)
        mainentry.type = readName(entry.substr(0x08 * 2, 3 * 2), 3);

    return mainentry;
}

ExtraEntry RDET::readExtraEntry(string entry)
{
    ExtraEntry extraentry;
    extraentry.extraName1 = readName(entry.substr(0x01 * 2, 10 * 2), 10);
    extraentry.extraName2 = readName(entry.substr(0x0E * 2, 12 * 2), 12);
    extraentry.extraName3 = readName(entry.substr(0x1C * 2, 4 * 2), 4);

    return extraentry;
}

void RDET::showData(BYTE sector[512], MainEntry main)
{
    int readPoint = main.startSector * 512;
    do {
        ReadSector(driver, readPoint, sector);
        cout << sector;
        string stringsector = HexConvert(sector);
        if (stringsector.substr(0x1FC * 2, 4 * 2) == "00000000")
            break;
        else
            readPoint += 512;
    } while (true);
    cout << endl;
}

void RDET::showInfor(MainEntry main, vector<ExtraEntry> extra, int numOfExtraEntry)
{
    cout << "////////// INFOR OF FILE //////////" << endl;
    stringstream fullname;

    if (numOfExtraEntry != 0) {
        for (int i = extra.size() - 1; numOfExtraEntry > 0; i--, numOfExtraEntry--) {
            fullname << extra[i].extraName1 << extra[i].extraName2 << extra[i].extraName3;
            cout << "*** Extra Entry ***" << endl;
            cout << "Extra name 1: " << extra[i].extraName1 << endl;
            cout << "Extra name 2: " << extra[i].extraName2 << endl;
            cout << "Extra name 3: " << extra[i].extraName3 << endl;
        }
    }
    else
        fullname << main.name;

    cout << "*** Main Entry ***" << endl;
    cout << "Name: " << main.name << endl;
    cout << "Type: " << main.type << endl;
    cout << "State:" << main.state << endl;
    cout << "Cluster start index: " << main.startCluster << endl;
    cout << "Sector start index: " << main.startSector << endl;
    cout << "Size: " << main.size << endl;
    cout << "Full Name: " << fullname.str() << endl;
    cout << "Data:";

    if (main.type == "TXT") {
        BYTE sector[512];
        showData(sector, main);
    }
    else
        cout << "*Use compatible software to read the content*" << endl;
}

void RDET::showRDET(BYTE sector[512], int readPoint)
{
    vector<MainEntry> mainentry;
    vector<ExtraEntry> extraentry;

    do {
        ReadSector(driver, readPoint, sector);
        string stringsector = HexConvert(sector);
        string endCondition = stringsector.substr(0x1FC * 2, 4 * 2);
        int numOfExtraEntry = 0;

        do {
            string entry = stringsector.substr(0, 64);

            if (entry.substr(0x00 * 2, 2) == "00")
                break;

            if (entry.substr(0x00 * 2, 2) != "2e") {

                if (entry.substr(0x0B * 2, 2) == "0f") {
                    ExtraEntry extra = readExtraEntry(entry);
                    extraentry.push_back(extra);
                    numOfExtraEntry += 1;
                }
                else {
                    MainEntry main = readMainEntry(entry);
                    mainentry.push_back(main);
                    showInfor(mainentry[mainentry.size() - 1], extraentry, numOfExtraEntry);
                    if (main.attribute == 16) {
                        BYTE sector1[512];
                        cout << "<<< SDET >>>" << endl;
                        showRDET(sector1, 512 * main.startSector);
                        cout << ">>> END SDET <<<" << endl;
                    }
                    numOfExtraEntry = 0;
                }
            }


            stringsector.erase(0, 64);
        } while (stringsector != "");

        if (endCondition == "00000000")
            break;
        else
            readPoint += 512;

    } while (true);
}
