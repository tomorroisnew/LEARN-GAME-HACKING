#include "pch.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

HMODULE myhModule;

DWORD WINAPI MainThread();
const char* PlayerColor(DWORD PlayerControl, DWORD ModuleAddress);
DWORD getOffset(DWORD baseAddress, std::vector<DWORD> offsets);
DWORD __stdcall EjectThread(LPVOID lpParameter);
void MainHack(DWORD ModuleAddress, DWORD playerControl);
bool isImpostor(DWORD ModuleAddress, DWORD playerControl);

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        myhModule = hModule;
        CreateThread(NULL, 0x100,(LPTHREAD_START_ROUTINE)MainThread, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

DWORD WINAPI MainThread() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout << "TEST" << std::endl;

    DWORD GameAssemblyDll = (DWORD)GetModuleHandle(L"GameAssembly.dll");
    std::vector<DWORD> oPlayerList{0xDC, 0x28, 0x48, 0x98, 0xC, 0x5C, 0x8, 0x8, 0x10};
    DWORD baseAdress = GameAssemblyDll + 0x01BEABF4;
    DWORD playerList = getOffset(baseAdress, oPlayerList);

    bool pause = true;

    while (!GetAsyncKeyState(VK_NUMPAD0)) {
        if (GetAsyncKeyState('P')) {
            if (pause == true) {
                pause = false;
            }
            else if (pause == false) {
                pause = true;
            }
        }
        if (pause == false) {
            for (int i = 0; i <= 128; i = i + 4) {
                DWORD playerControl = *(DWORD*)(playerList + i);
                if (playerControl == 0) { break; }

                MainHack(GameAssemblyDll, playerControl);
            }
        }

        if (pause == true) {
            std::cout << "PAUSED" << std::endl;
        }
        Sleep(300);
        system("cls");
    }

    FreeConsole();
    CreateThread(0, 0, EjectThread, 0, 0, 0);
    int i = 0;
    return S_OK;
}

DWORD __stdcall EjectThread(LPVOID lpParameter) {
    Sleep(100);
    FreeLibraryAndExitThread(myhModule, 0);
}

const char* PlayerColor(DWORD PlayerControl, DWORD ModuleAddress) {
    DWORD PlayerInfo = *(DWORD*)(PlayerControl + 0x3C);
    DWORD aGetDefaultOutfit = ModuleAddress + 0x74F6E0;
    
    DWORD(__cdecl * GetDefaultOutfit)(void* pThis);
    GetDefaultOutfit = (DWORD(__cdecl*)(void*))aGetDefaultOutfit;

    DWORD PlayerOutfit = GetDefaultOutfit((void*)PlayerInfo);
    int colorId = *(int*)(PlayerOutfit + 0xC);

    switch (colorId) {
    case 0:
        return "RED";
    case 1:
        return "BLUE";
    case 2:
        return "GREEN";
    case 3:
        return "PINK";
    case 4:
        return "ORANGE";
    case 5:
        return "YELLOW";
    case 6:
        return "BLACK";
    case 7:
        return "WHITE";
    case 8:
        return "PURPLE";
    case 9:
        return "BROWN";
    case 10:
        return "CYAN";
    case 11:
        return "LIME";
    case 12:
        return "MAROON";
    case 13:
        return "ROSE";
    case 14:
        return "BANANA";
    case 15:
        return "GRAY";
    case 16:
        return "TAN";
    case 17:
        return "CORAL";
    }
}

DWORD getOffset(DWORD baseAddress, std::vector<DWORD> offsets) {
    DWORD basePointer = *(DWORD*)(baseAddress);

    DWORD currentPointer = basePointer;

    for (int i = 0; i < offsets.size() - 1; i++) {
        currentPointer = *(DWORD*)(currentPointer + offsets.at(i));
    }

    return currentPointer + offsets.back();
}

void MainHack(DWORD ModuleAddress, DWORD playerControl) {
    DWORD playerID = *(DWORD*)(playerControl + 0x28);
    const char* color = PlayerColor(playerControl, ModuleAddress);
    bool isImpo = isImpostor(ModuleAddress, playerControl);
    DWORD playerInfo = *(DWORD*)(playerControl + 0x3C);

    std::cout << "ID: " << playerID << " | COLOR: " << color << " | IsImpostor: " << isImpo;
    std::cout << " | More Info: ";
    if (*(BYTE*)(playerControl + 0x38) == 1) {
        std::cout << "InVent ";
    }
    if (*(bool*)(playerInfo + 0x20) == true) {
        std::cout << "DEAD";
    }
    std::cout << std::endl;
}

bool isImpostor(DWORD ModuleAddress, DWORD playerControl) {
    bool(__cdecl * IsImpostor)(void* pthis);
    IsImpostor = (bool(__cdecl*)(void*))(ModuleAddress+ 0x56C860);

    DWORD playerInfo = *(DWORD*)(playerControl + 0x3C);
    DWORD role = * (DWORD*)(playerInfo + 0x18);

    return IsImpostor((void*)role);
}