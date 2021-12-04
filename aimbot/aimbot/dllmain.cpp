#include "pch.h"
#include <windows.h>
#include <iostream>
#include "VectorMath.h"

HMODULE Hmod;
void MainHack();

void EjectThread() {
    Sleep(1000);
    FreeLibraryAndExitThread(Hmod, 0);
}

BOOL WINAPI MainThread() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout << "TEST" << std::endl;

    MainHack();

    FreeConsole();
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)EjectThread, NULL, NULL, NULL);
    return S_OK;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Hmod = hModule;
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void MainHack() {
    DWORD clientDll = (DWORD)GetModuleHandle(L"client.dll");
    DWORD engineDll = (DWORD)GetModuleHandle(L"Engine.dll");
    DWORD LocalPlayer = *(DWORD*)(clientDll + 0xDA746C);
    DWORD ClientState = *(DWORD*)(engineDll + 0x589FCC);
    Rotate ViewAngles = *(Rotate*)(ClientState + 0x4D90);

    DWORD Target = *(DWORD*)(clientDll + 0x4DC178C + 0x50);

    while (TRUE) {
        Vec3 PlayerPos = *(Vec3*)(LocalPlayer + 0x138);
        Vec3 TargetPos = *(Vec3*)(Target + 0x138);

        Rotate rotate = CalcAngle(PlayerPos, TargetPos);
        *(Rotate*)(ClientState + 0x4D90) = rotate;
        if (GetAsyncKeyState(VK_NUMPAD0)) {
            break;
        }
        Sleep(10);
    }
}