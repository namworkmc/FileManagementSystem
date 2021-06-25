#include "MFT.h"
#include "NTFSPartitionBootSector.h"
#include "BSFAT32.h"

#include <sstream>
#include <codecvt>

int main(int argc, char** argv)
{
    BYTE sector[512];

    string diskName;
    cout << "Choose your disk: ";
    getline(cin, diskName);

    stringstream buffer;
    buffer << "\\\\.\\" << diskName << ":";

    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    wstring wide = converter.from_bytes(buffer.str());
    LPCWSTR driver = wide.c_str();

    ReadSector(driver, 0, sector);

    int bps, spc, lcn;
    asNTFS(sector, bps, spc, lcn);
    system("pause");

    FolderTree(L"\\\\.\\J:", 0, bps, spc, lcn);
    
    /*BYTE sector[512];
    ReadSector(L"\\\\.\\J:", 0, sector);
    BootSectorFAT32(sector);*/

    return 0;
}
