#include "pch.h"
#include <windows.h>
#include <iostream>

namespace offsets {
    DWORD oLocalPlayer = 0xDA747C;
    DWORD oForceJump = 0x526B5B0;
    DWORD oOnGround = 0x104;
}

DWORD WINAPI MainThread();

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0x250, (LPTHREAD_START_ROUTINE)MainThread, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

DWORD WINAPI MainThread() {

    HMODULE clientdll = GetModuleHandle(L"client.dll");

    while (TRUE) {
        DWORD playerOffset = *(DWORD*)((DWORD)clientdll + offsets::oLocalPlayer);
        BYTE onGround = *(BYTE*)(playerOffset + offsets::oOnGround);
        
        if (GetAsyncKeyState(VK_SPACE) && onGround == 1){
            *(BYTE*)((DWORD)clientdll + offsets::oForceJump) = (BYTE)6;
        }

        if (GetAsyncKeyState(VK_F11)) {
            break;
        }
    }

    FreeConsole();

    return S_OK;
}