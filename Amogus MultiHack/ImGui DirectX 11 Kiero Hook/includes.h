#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <vector>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include <iostream>

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

DWORD __stdcall EjectThread(LPVOID lpParameter);
DWORD GetOffset(DWORD BaseAddress, std::vector<DWORD> offsets);
void SpeedHack();
void ClearTask();
void AllKnowingHock();
std::string PlayerColor(DWORD PlayerControl, DWORD ModuleAddress);
std::string MainAllKnowingHack(DWORD ModuleAddress, DWORD PlayerControl);
std::string Role(DWORD PlayerControl);

extern bool EnableAllKnowingHack;
extern bool EnableSpeedHack;
extern float PlayerModSpeed;
extern std::string TextToPrint;