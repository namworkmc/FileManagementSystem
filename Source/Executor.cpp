#include "Executor.h"

void Executor::execute()
{
	string diskName;
	cout << "\tChoose your disk: ";
	getline(cin, diskName);

	stringstream buffer;
	buffer << "\\\\.\\" << diskName << ":";

	wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	wstring wide = converter.from_bytes(buffer.str());
	LPCWSTR driver = wide.c_str();

	chooseTypeDisplay();
	int choice;
	cin >> choice;
	cin.ignore();
	system("cls");
	

	BYTE sector[512];
	RDET rdet(driver);
	switch (choice)
	{
	case 1:
		ReadSector(driver, 0, sector);
		BootSectorFAT32(sector);

		system("pause");
		system("cls");

		rdet.execute();
		break;

	case 2:
		ReadSector(driver, 0, sector);

		int bps, spc, lcn;
		asNTFS(sector, bps, spc, lcn);

		system("pause");
		system("cls");

		FolderTree(driver, 0, bps, spc, lcn);
		break;
	}
}
