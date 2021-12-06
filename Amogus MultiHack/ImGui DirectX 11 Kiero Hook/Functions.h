#pragma once
#include <Windows.h>

struct Vec3 {
    float x, y, z;
};

extern DWORD GameAssembly;

//Function pointers
extern DWORD(__cdecl* get_current_Camera)();
extern DWORD(__cdecl* get_Transform)(void* pThis);
extern Vec3(__cdecl* get_Position)(void* pThis);
extern Vec3(__cdecl* WorldToScreenPoint)(void* pThis, Vec3 position);

void InitFunctions();