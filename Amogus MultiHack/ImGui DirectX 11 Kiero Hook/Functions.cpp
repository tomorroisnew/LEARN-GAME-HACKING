#include "Functions.h"

DWORD(__cdecl* get_current_Camera)();
DWORD(__cdecl* get_Transform)(void* pThis);
Vec3(__cdecl* get_Position)(void* pThis);
Vec3(__cdecl* WorldToScreenPoint)(void* pThis, Vec3 position);
DWORD GameAssembly;

void InitFunctions() {
    GameAssembly = (DWORD)GetModuleHandle("GameAssembly.dll");
    get_current_Camera = (DWORD(__cdecl*)())(GameAssembly + 0xB404C0);
    get_Transform = (DWORD(__cdecl*)(void*))(GameAssembly + 0xB43800);
    get_Position = (Vec3(__cdecl*)(void*))(GameAssembly + 0xE98230);
    WorldToScreenPoint = (Vec3(__cdecl*)(void*, Vec3))(GameAssembly + 0xB3FF00);
}