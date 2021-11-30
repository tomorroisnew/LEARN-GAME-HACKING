#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <Winuser.h>
#include <d3dx9.h>
#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include <iostream>
#define WINDOW_NAME "Dear ImGui DirectX9 Example"
typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);


struct Vec2 {
	float x, y;
};

struct Vec3 {
	float x, y, z;
};

struct Vec4 {
	float x, y, z, w;
};

struct Matrix {
	float matrix[16];
};

void MainUi(LPDIRECT3DDEVICE9 pDevice);
void EspLines();
void BunnyHop();
void GlowHackFunc();
void NoFlashFunc();
void TriggerBotFunc();
void TriggerBotFunc();
bool WorldToScreen(Vec3 pos, Vec2& screen, Matrix matrix, int windowWidth, int windowHeight);

extern RECT rect;