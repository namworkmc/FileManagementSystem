# FileManagementSystem

--- NTFS ---
- Xóa hàm  
bool IsWhatEntry(BYTE sector[512],int bit)

- Thêm hàm 
string Signature(BYTE sector[512])
{
    return subB(sector, 0x00, 0x03);
}

- Chỉnh hàm
Entry AnalysisSector(LPCWSTR  drive, int readPoint) {
    
    Entry e;

    ReadSector(drive, readPoint, e.sector);

    string file = GetEntryStatus(e.sector);

    //cout << "readpoint: " << readPoint << endl;
    if (Signature(e.sector)=="FILE" && AttributeName(e.sector) != "" && file[2] != '1')
    {
        e.name = AttributeName(e.sector);
        e.status = GetEntryStatus(e.sector);
        e.residentFlat = ResidentData(e.sector);

        if (e.status.length() >= 28 && file[28] != '1' || e.status.length() < 28) // if not directory file -> show size
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

