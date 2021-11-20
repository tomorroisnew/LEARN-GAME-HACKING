#include "Functions.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

DWORD GetpID() {
    HANDLE pHandle;
    PROCESSENTRY32 pe32;

    pHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(pHandle, &pe32)) {
        std::cout << "error fetching pid" << std::endl;
    }

    while (Process32Next(pHandle, &pe32)) {
        std::wstring binPath = pe32.szExeFile;
        if (binPath.find(L"csgo.exe") != std::wstring::npos) {
            return pe32.th32ProcessID;
        }
    }

    return 0;
}