#include <iostream>
#include <Windows.h>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream>
#include "FileCMD.h"
using namespace std;
void FileCMD::underline()
{
    for (int i = 0; i < 110; ++i) {
        cout << '-';
    }
    cout << "\n";
}
void FileCMD::Interface()
{
    SetConsoleTextAttribute(consolehwnd, lightblue);
    underline();
    cout << "|" << "\t\t\t" << "-----------------------------------------------------------------\t\t" << "     |" << endl
        << "|" << "\t\t\t" << "|   \t Welcome To CMD File Manger (*^_^*)\t\t\t|\t\t" << "     |" << endl
        << "|" << "\t\t\t" << "-----------------------------------------------------------------\t\t" << "     |" << endl;
    underline();
}
void FileCMD::listDir(const wstring& path) {
    SetConsoleTextAttribute(consolehwnd, electric);
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW((path + L"\\*").c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
            string fileName = converter.to_bytes(findFileData.cFileName);
            cout << fileName << endl;
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}


bool FileCMD::Move_File(const wstring& source, const wstring& destination) {
    if (MoveFileW(source.c_str(), destination.c_str()) != 0) {
        wcout << L"File moved successfully." << endl;
        return true;
    }

    DWORD sourceAttributes = GetFileAttributesW(source.c_str());
    if (sourceAttributes == INVALID_FILE_ATTRIBUTES) {
        wcerr << L"Source file does not exist." << endl;
        return false;
    }

    DWORD destinationAttributes = GetFileAttributesW(destination.c_str());
    if (destinationAttributes != INVALID_FILE_ATTRIBUTES) {
        wcout << L"Destination file already exists. Overwriting..." << endl;
    }

    if (MoveFileW(source.c_str(), destination.c_str()) != 0) {
        wcout << L"File moved successfully." << endl;
        return true;
    }
    else {
        wcerr << L"Failed to move file. Error code: " << GetLastError() << endl;
        return false;
    }
    if (source == destination) {
        wcout << L"Source and destination paths are the same." << endl;
        return true;
    }
}

bool FileCMD::Copy_File(const std::wstring& source, const std::wstring& destination) {
    if (::CopyFileW(source.c_str(), destination.c_str(), FALSE) != 0) {
        std::wcout << L"File copied successfully." << std::endl;
        return true;
    }
    else {
        DWORD errorCode = GetLastError();
        switch (errorCode) {
        case ERROR_ACCESS_DENIED:
            std::wcerr << L"Access denied for destination file." << std::endl;
            break;
        case ERROR_DISK_FULL:
            std::wcerr << L"Insufficient disk space." << std::endl;
            break;
        case ERROR_FILE_NOT_FOUND:
            std::wcerr << L"The system cannot find the path specified." << std::endl;
            break;
        default:
            std::wcerr << L"Failed to copy file. Error code: " << errorCode << std::endl;
            break;
        }
        return false;
    }
}

bool FileCMD::Remove_File(const wstring& path) {
    if (DeleteFileW(path.c_str()) != 0) {
        wcout << L"File deleted successfully." << endl;
        return true;
    }
    else {
        DWORD errorCode = GetLastError();
        if (errorCode == ERROR_FILE_NOT_FOUND) {
            wcerr << L"File does not exist." << endl;
        }
        else {
            wcerr << L"Failed to delete file. Error code: " << errorCode << endl;
        }
        return false;
    }
}

void FileCMD::catFile(const wstring& filename) {
    wifstream file(filename);
    if (file.is_open()) {
        wstring line;
        while (getline(file, line)) {
            SetConsoleTextAttribute(consolehwnd, yellow);
            wcout << line << endl;
        }
        file.close();
    }
    else {
        wcerr << L"Error: Unable to open file " << filename << endl;
    }
}



bool FileCMD::Touch_File(const wstring& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file.close();
        return true;
    }
    else {
        wcerr << L"Failed to create file." << endl;
        return false;
    }
}

void FileCMD::Execute()
{
    wstring currentDirectory = L"  .  ";
    wstring command;
    Interface();
    while (true) {

        SetConsoleTextAttribute(consolehwnd, white);
        wcout << L"\nCurrent Directory : " << currentDirectory << endl;
        wcout << "\t\t\tEnter A Command : - \n"
            << "\t\t\t1- ls \n"
            << "\t\t\t2- mkdir\n"
            << "\t\t\t3- touch\n"
            << "\t\t\t4- cp\n"
            << "\t\t\t5- mv\n"
            << "\t\t\t6- rm\n"
            << "\t\t\t7- cd\n"
            << "\t\t\t7- cat\n"
            << "\t\t\t9- clear\n"
            << "\t\t\t10- Exit\n"
            << "\t\t\tEnter The \" CMD \" or it's \" Number \": ";
        wcin >> command;

        if (command == L"ls" || command == L"1") {
            underline();
            listDir(currentDirectory);
            SetConsoleTextAttribute(consolehwnd, white);
            underline();
        }
        else if (command == L"mkdir" || command == L"2") {
            wstring newDirName;
            wcout << L"Enter directory name: ";
             wcin >> newDirName;
            if (MakeDir(currentDirectory + L"\\" + newDirName))
            {
                SetConsoleTextAttribute(consolehwnd, green);
                wcout << L"Directory created successfully." << endl;
            }
            else
            {
                SetConsoleTextAttribute(consolehwnd, lightred);
                wcout << L"Failed to create directory." << endl;
            }
        }
        else if (command == L"touch" || command == L"7") {
            wstring fileName;
            wcout << L"Enter file name: ";
            wcin >> fileName;
            if (Touch_File(currentDirectory + L"\\" + fileName))
            {
                SetConsoleTextAttribute(consolehwnd, green);
                wcout << L"File created successfully." << endl;
            }
            else
            {
                SetConsoleTextAttribute(consolehwnd, lightred);
                wcout << L"Failed to create file." << endl;
            }
        }
        else if (command == L"cp" || command == L"4") {
            wstring source, destination;
            wcout << L"Enter source file path: ";
            wcin >> source;
            wcout << L"Enter destination file path: ";
            wcin >> destination;
            if (Copy_File(source, destination))
            {
                SetConsoleTextAttribute(consolehwnd, green);
                wcout << L"File copied successfully." << endl;
            }
            else
            {
                SetConsoleTextAttribute(consolehwnd, lightred);
                wcout << L"Failed to copy file." << endl;
            }
        }


        else if (command == L"mv" || command == L"5") {
            wstring source, destination;
            wcout << L"Enter source file path: ";
            wcin >> source;
            wcout << L"Enter destination file path: ";
            wcin >> destination;
            if (Move_File(source, destination))
            {
                SetConsoleTextAttribute(consolehwnd, green);
                wcout << L"File moved successfully." << endl;
            }
            else
            {
                SetConsoleTextAttribute(consolehwnd, lightred);
                wcout << L"Failed to move file." << endl;
            }
        }
        else if (command == L"rm" || command == L"6") {
            wstring filePath;
            wcout << L"Enter file path: ";
            wcin >> filePath;
            if (Remove_File(filePath))
            {
                SetConsoleTextAttribute(consolehwnd, green);
                wcout << L"File deleted successfully." << endl;
            }
            else
            {
                SetConsoleTextAttribute(consolehwnd, lightred);
                wcout << L"Failed to delete file." << endl;
            }
        }
        else if (command == L"cd" || command == L"7") {
            wstring newDir;
            wcout << L"Enter directory path: ";
            while (wcin.get() != L'\n') {}

            wcin>>newDir;
            DWORD attributes = GetFileAttributesW(newDir.c_str());
            if (attributes != INVALID_FILE_ATTRIBUTES && attributes & FILE_ATTRIBUTE_DIRECTORY) {
                currentDirectory = newDir;
                {
                    SetConsoleTextAttribute(consolehwnd, green);
                    wcout << L"Directory changed successfully." << endl;
                }
            }
            else {
                SetConsoleTextAttribute(consolehwnd, lightred);
                wcerr << L"Invalid directory path or directory does not exist." << endl;
            }
        }

        else if (command == L"cat" || command == L"8") {
            wstring filename;
            wcout << L"Enter file name: ";
            wcin >> filename;
            catFile(currentDirectory + L"\\" + filename);
        }

        else if (command == L"clear" || command == L"9")
        {
            system("cls");
            Interface();
        }
        else if (command == L"exit" || command == L"10") {
            break;
        }
        else {
            wcout << L"Invalid command. Please try again." << endl;
            SetConsoleTextAttribute(consolehwnd, lightred);
        }
    }
}
