#include <iostream>
#include <windows.h>
#include <tlhelp32.h>

DWORD GetpID();
DWORD GetModule(DWORD Pid);
DWORD getLocalPlayerPointer(HANDLE process, DWORD modBaseAddress);

namespace offsets {
    DWORD m_fFlags = 0x104;
    DWORD dwForceJump = 0x526B590;
    DWORD dwLocalPlayer = 0xDA747C;
}

int main()
{
    DWORD pID = GetpID();
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, pID);
    DWORD modAddress = GetModule(pID);
    DWORD localplayerAddress = getLocalPlayerPointer(hProc, modAddress);
    DWORD isOnGroundAddress = localplayerAddress + offsets::m_fFlags;
    DWORD forceJump = modAddress + offsets::dwForceJump;

    std::cout << std::hex << forceJump << std::endl;

    BYTE jump = 6;

    while (TRUE) {
        BYTE isOnGround;
        ReadProcessMemory(hProc, (LPVOID)isOnGroundAddress, &isOnGround, sizeof(BYTE), NULL);

        //std::cout << (DWORD)isOnGround << std::endl;

        if (isOnGround == 1 && GetAsyncKeyState(VK_SPACE)) {
            WriteProcessMemory(hProc, (LPVOID)forceJump, &jump, sizeof(BYTE), NULL);
        }

        //Sleep(1);
        //system("cls");
    }
    //WriteProcessMemory(hProc, (LPVOID)(modAddress + 0x526B590), &jump, sizeof(BYTE), NULL);

    std::cout << std::hex << localplayerAddress;
}

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

DWORD GetModule(DWORD Pid) {
    HANDLE hSnap;
    MODULEENTRY32 me32;

    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, Pid);
    me32.dwSize = sizeof(MODULEENTRY32);

    while (Module32Next(hSnap, &me32)) {
        std::wstring binPath = me32.szExePath;
        if (binPath.find(L"\\client.dll") != std::wstring::npos) {
            return (DWORD)me32.modBaseAddr;
        }
    }

    return 0;
}

DWORD getLocalPlayerPointer(HANDLE process, DWORD modBaseAddress) {
    DWORD localPlayerPointer;
    ReadProcessMemory(process, (LPCVOID)(modBaseAddress + offsets::dwLocalPlayer), &localPlayerPointer, sizeof(DWORD), NULL);

    return localPlayerPointer;
}