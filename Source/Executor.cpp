#include "Executor.h"

void Executor::execute()
{
	chooseTypeDisplay();
	int choice;
	cin >> choice;
	cin.ignore();
	system("cls");

	BYTE sector[512];
	switch (choice)
	{
	case 1:
		ReadSector(driver, 0, sector);
		BootSectorFAT32(sector);
		break;

	case 2:
		ReadSector(driver, 0, sector);

		int bps, spc, lcn;
		asNTFS(sector, bps, spc, lcn);
		system("pause");

		FolderTree(driver, 0, bps, spc, lcn);
		break;
	default:
		break;
	}
}
