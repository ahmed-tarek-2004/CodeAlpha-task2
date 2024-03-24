#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream> 
using namespace std;
class FileCMD
{
enum colors {
    black = 0, electric = 1, leaf = 2, lightblue = 3, red = 4, darkpurple = 5, gold = 6, lightgrey = 7, grey = 8, blue = 9, green = 10, aqua = 11, lightred = 12, purple = 13, yellow = 14, white = 15,
};
private:
    HANDLE consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
    wstring currentDirectory;
    void underline();
    void Interface();
    void listDir(const wstring& path);

    bool MakeDir(const wstring& path) {
        DWORD attributes = GetFileAttributesW(path.c_str());
        if (attributes != INVALID_FILE_ATTRIBUTES) {
            if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
                wcout << L"Directory already exists." << endl;
                return true;
            }
            else {
                wcerr << L"A file with the same name already exists." << endl;
                return false;
            }
        }

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
    bool Copy_File(const wstring& sourcePath, const wstring& destinationPath);

    void catFile(const wstring& filename);

    bool Move_File(const wstring& source, const wstring& destination);
    
    bool Remove_File(const wstring& path);
    
    bool Touch_File(const wstring& filename);

public:
    void Execute();
};
