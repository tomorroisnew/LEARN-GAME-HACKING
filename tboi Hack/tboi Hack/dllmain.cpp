// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include "Isaac.h"
#include "minhook/include/MinHook.h"
#pragma comment(lib, "minhook/build/VC16/lib/Debug/libMinHook.x86.lib")

Player* LocalPlayer;

void MainHack(Player* player);
void(__fastcall* PlayerHook)(Player* pThis);
void __fastcall PlayerHookH(Player* pThis) {
    std::cout << "Player Address Found: " << std::hex << pThis << std::endl;
    LocalPlayer = pThis;
    if (GetAsyncKeyState(VK_F1)) {
        std::cout << "HACK" << std::endl;
        MainHack(LocalPlayer);
    }
    return PlayerHook(pThis);
}

void Main() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout << "TEST" << std::endl;

    DWORD BaseAddress = (DWORD)GetModuleHandle(L"isaac-ng.exe");
    DWORD PlayerHookA = BaseAddress + 0x341FE0;

    MH_Initialize();
    MH_CreateHook(reinterpret_cast<void**>(PlayerHookA), &PlayerHookH, reinterpret_cast<void**>(&PlayerHook));
    MH_EnableHook(MH_ALL_HOOKS);
}

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void MainHack(Player* player) {
    std::cout << "Giving Bomb, Keys, and Coins" << std::endl;
    player->Bombs = 99;
    player->Coins = 99;
    player->Keys = 99;
    std::cout << "SETTING DAMAGE and Luck TO 50" << std::endl;
    player->Damage = 50.f;
    player->Luck = 50.f;
    std::cout << "MAXING OUT HEALTH" << std::endl;
    player->MaxHeart = 24;
    player->Heart = 24;
    std::cout << "MAXING OUT CHARGE" << std::endl;
    player->Charge = 99;
}
