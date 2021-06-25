#include "ReadSector.h"
#include "NTFSPartitionBootSector.h"

string subB(BYTE sector[], int x, int y) {
    string s = "";

    for (int i = x; i <= y; ++i)
        s += sector[i];

    return s;
}

long subB_dec(BYTE sector[], int x, int y) {
    int result = 0;

    for (int i = y; i >= x; --i)
        result = (result << 8) | sector[i];

    return result;
}

void subB_hex(BYTE sector[], int x, int y) {
    cout << hex << setfill('0');

    for (int i = x; i <= y; ++i) {
        cout << setw(2) << int(sector[i]) << ' ';
    }

    cout << dec << endl;
}

// sau lenh ReadSector(...) goi asNTFS(sector);
void asNTFS(BYTE s[512], int& bps, int& spc, int& lcn) {

    cout << "JMP instruction: ";
    subB_hex(s, 0x00, 0x02);

    cout << "OEM ID: ";
    cout << subB(s, 0x03, 0x0a) << endl;

    cout << "Bytes per sector: ";
    bps = subB_dec(s, 0x0b, 0x0c);
    cout << bps << endl;

    cout << "Sectors per cluster: ";
    spc = subB_dec(s, 0x0d, 0x0d);
    cout << spc << endl;

    cout << "Reserved sectors: ";
    cout << subB_dec(s, 0x0e, 0x0f) << endl;

    cout << "(always zero): ";
    cout << subB_dec(s, 0x10, 0x12) << endl;

    cout << "Media descriptor: ";
    cout << subB_dec(s, 0x15, 0x15) << endl;

    cout << "Sectors per track: ";
    cout << subB_dec(s, 0x18, 0x19) << endl;

    cout << "Number of heads: ";
    cout << subB_dec(s, 0x1A, 0x1B) << endl;

    cout << "Hidden sectors: ";
    cout << subB_dec(s, 0x1C, 0x1F) << endl;

    cout << "Signature: ";
    subB_hex(s, 0x24, 0x27);

    cout << "Total sectors: ";
    cout << subB_dec(s, 0x28, 0x2f) << endl;

    cout << "$MFT logical cluster number: ";
    lcn = subB_dec(s, 0x30, 0x37);
    cout << lcn << endl;

    cout << "$MFTMirr logical cluster number: ";
    cout << subB_dec(s, 0x38, 0x3f) << endl;

    cout << "Clusters per File Record Segment: ";
    cout << subB_dec(s, 0x40, 0x43) << endl;

    cout << "Clusters per Index Buffer: ";
    cout << subB_dec(s, 0x44, 0x47) << endl;

    cout << "Volume serial number: ";
    subB_hex(s, 0x48, 0x4f);

    cout << "Checksum: ";
    cout << subB_dec(s, 0x50, 0x53) << endl;

    //cout << "Boostrap code: ";
    //subB_hex(s, 0x54, 0x1FD);

    cout << "Signature (55 AA): ";
    subB_hex(s, 0x1FE, 0x1FF);
}