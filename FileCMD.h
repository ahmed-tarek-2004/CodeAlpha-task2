#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream> 
using namespace std;
enum colors {
    black = 0, electric = 1, leaf = 2, lightblue = 3, red = 4, darkpurple = 5, gold = 6, lightgrey = 7, grey = 8, blue = 9, green = 10, aqua = 11, lightred = 12, purple = 13, yellow = 14, white = 15,
};
class FileCMD
{
private:
    HANDLE consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
    wstring currentDirectory;
    void underline()
    {
        for (int i = 0; i < 110; ++i) {
            cout << '-';
        }
        cout << "\n";
    }
    void Interface()
    {
        SetConsoleTextAttribute(consolehwnd, lightblue);
        underline();
        cout << "|" << "\t\t\t" << "-----------------------------------------------------------------\t\t" << "     |" << endl
            << "|" << "\t\t\t" << "|   \t Welcome To CMD File Manger (*^_^*)\t\t\t|\t\t" << "     |" << endl
            << "|" << "\t\t\t" << "-----------------------------------------------------------------\t\t" << "     |" << endl;
        underline();
    }
    void listDir(const wstring& path) {
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

    bool MakeDir(const wstring& path) {
        if (CreateDirectoryW(path.c_str(), NULL) != 0) {
            return true;
        }
        else {
            DWORD errorCode = GetLastError();
            if (errorCode == ERROR_ALREADY_EXISTS) {
                wcout << L"Directory already exists." << endl;
            }
            else {
                wcerr << L"Failed to create directory. Error code: " << errorCode << endl;
            }
            return false;
        }
    }


    bool Copy_File(const wstring& source, const wstring& destination) {
        DWORD sourceAttributes = GetFileAttributesW(source.c_str());
        if (sourceAttributes == INVALID_FILE_ATTRIBUTES) {
            wcerr << L"Source file does not exist." << endl;
            return false;
        }

        wstring destDir = destination.substr(0, destination.find_last_of(L'\\'));
        DWORD destDirAttributes = GetFileAttributesW(destDir.c_str());
        if (destDirAttributes == INVALID_FILE_ATTRIBUTES || !(destDirAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            wcerr << L"Destination directory does not exist." << endl;
            return false;
        }

        DWORD destAttributes = GetFileAttributesW(destination.c_str());
        if (destAttributes != INVALID_FILE_ATTRIBUTES) {
            wcerr << L"Destination file already exists." << endl;
            return false;
        }


        if (source == destination) {
            wcerr << L"Source and destination are the same file." << endl;
            return false;
        }

        if (CopyFileW(source.c_str(), destination.c_str(), FALSE) != 0) {
            return true;
        }
        else {

            DWORD errorCode = GetLastError();
            switch (errorCode) {
            case ERROR_ACCESS_DENIED:
                wcerr << L"Access denied for destination file." << endl;
                break;
            case ERROR_DISK_FULL:
                wcerr << L"Insufficient disk space." << endl;
                break;
            default:
                wcerr << L"Failed to copy file. Error code: " << errorCode << endl;
                break;
            }
            return false;
        }
    }

    bool Move_File(const wstring& source, const wstring& destination) {
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

    bool Remove_File(const wstring& path) {
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

    bool Touch_File(const wstring& filename) {
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


public:
    void Execute()
    {
        wstring currentDirectory = L"  .  ";
        wstring command;
        Interface();
        while (true) {

            SetConsoleTextAttribute(consolehwnd, white);
            wcout << L"\nCurrent Directory : " << currentDirectory << endl;
            wcout <<"\t\t\tEnter A Command : - \n"
                << "\t\t\t1- ls\n"
                << "\t\t\t2- mkdir\n"
                << "\t\t\t3- Touch\n"
                << "\t\t\t4- cp\n"
                << "\t\t\t5- mv\n"
                << "\t\t\t6- rm\n"
                << "\t\t\t7- cd\n"
                << "\t\t\t8- clear\n"
                << "\t\t\t9- Exit\n"
                << "\t\t\tEnter Your CMD : ";
            wcin >> command;

            if (command == L"ls"||command==L"1") {
                listDir(currentDirectory);
            }
            else if (command == L"mkdir"||command==L"2") {
                wstring newDirName;
                wcout << L"Enter directory name: ";
                wcin >> newDirName;
                getline(wcin, newDirName);
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
            else if (command == L"cp"||command==L"4") {
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
            else if (command == L"mv"||command==L"5") {
                wstring source, destination;
                wcout << L"Enter source file path: ";
                wcin>>source;
                wcout << L"Enter destination file path: ";
                wcin>>destination;
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
            else if (command == L"rm"||command==L"6") {
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
                // Clear the input buffer
                while (wcin.get() != L'\n') {}

                getline(wcin, newDir);
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
            else if (command == L"clear"||command==L"8")
            {
                system("cls");
                Interface();
            }
            else if (command == L"exit"||command==L"9") {
                break;
            }
            else {
                wcout << L"Invalid command. Please try again." << endl;
                SetConsoleTextAttribute(consolehwnd, lightred);
            }
        }
    }
};
