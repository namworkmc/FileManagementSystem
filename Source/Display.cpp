#include "Display.h"

void chooseTypeDisplay()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 7);
    cout << "\t||--";
    SetConsoleTextAttribute(hConsole, 12);
    cout << "> ";
    SetConsoleTextAttribute(hConsole, 11);
    cout << 1 << ". ";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "FAT32" << endl;

    SetConsoleTextAttribute(hConsole, 7);
    cout << "\t||--";
    SetConsoleTextAttribute(hConsole, 12);
    cout << "> ";
    SetConsoleTextAttribute(hConsole, 11);
    cout << 2 << ". ";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "NTFS" << endl;

    SetConsoleTextAttribute(hConsole, 7);
    cout << "\t||" << endl;

    SetConsoleTextAttribute(hConsole, 7);
    cout << "\t|L";
    SetConsoleTextAttribute(hConsole, 12);
    cout << ">";

    SetConsoleTextAttribute(hConsole, 7);
}