#include "includes.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
void SpeedHack() {
	DWORD BaseAddress = (DWORD)GetModuleHandle("GameAssembly.dll");
	DWORD BasePointer = BaseAddress + 0x01C0C924;

	//std::cout << std::hex << BasePointer << std::endl;

	std::vector<DWORD> offsets{ 0x334, 0x3C, 0x28, 0x24, 0x5C, 0x14, 0x14};

	DWORD PlayerSpeed = GetOffset(BasePointer, offsets);
	if (!PlayerSpeed) { return; }

	*(float*)(PlayerSpeed) = PlayerModSpeed;
}

DWORD GetOffset(DWORD BaseAddress, std::vector<DWORD> offsets) {
	DWORD CurrentPointer = *(DWORD*)(BaseAddress);

	for (int i = 0; i < offsets.size() - 1; i++) {
		if (!CurrentPointer) { return 0; }
		CurrentPointer = *(DWORD*)(CurrentPointer + offsets.at(i));
	}

	return CurrentPointer + offsets.back();
}

void ClearTask() {
	std::vector<DWORD> oPlayerList{ 0xDC, 0x28, 0x48, 0x98, 0xC, 0x5C, 0x0 };
	DWORD BaseAddress = (DWORD)GetModuleHandle("GameAssembly.dll");
	DWORD BasePointer = BaseAddress + 0x01BEABF4;

	DWORD LocalPlayerPointer = *(DWORD*)(GetOffset(BasePointer, oPlayerList));
	DWORD ClearTaskAddress = BaseAddress + 0x63F8A0;

	void(__cdecl * ClearTask)(void* pThis);
	ClearTask = (void(__cdecl*)(void*))ClearTaskAddress;

	ClearTask((void*)LocalPlayerPointer);
}

void AllKnowingHock() {
	DWORD GameAssemblyDll = (DWORD)GetModuleHandle("GameAssembly.dll");
	std::vector<DWORD> oPlayerList{ 0xDC, 0x28, 0x48, 0x98, 0xC, 0x5C, 0x8, 0x8, 0x10 };
	DWORD baseAdress = GameAssemblyDll + 0x01BEABF4;
	DWORD playerList = GetOffset(baseAdress, oPlayerList);

    std::string ToBePrinted;

    for (int i = 0; i <= 128; i = i + 4) {
        DWORD playerControl = *(DWORD*)(playerList + i);
        if (playerControl == 0) { break; }
        std::string PlayerData = MainAllKnowingHack(GameAssemblyDll, playerControl);
        ToBePrinted += PlayerData;
    }
    TextToPrint = ToBePrinted;
}

std::string MainAllKnowingHack(DWORD ModuleAddress, DWORD PlayerControl) {
    int PlayerId = *(int*)(PlayerControl + 0x28);
    std::string Color = PlayerColor(PlayerControl, ModuleAddress);
    std::string role = Role(PlayerControl);
    DWORD PlayerInfo = *(DWORD*)(PlayerControl + 0x3C);

    std::string Data;
    Data = "ID: " + std::to_string(PlayerId);
    Data += " | Color: " + Color;
    Data += " | Role: " + role;
    Data += " | More Info:";
    if (*(bool*)(PlayerControl + 0x3A)) {
        Data += " ShapeShifting";
    }
    if (*(bool*)(PlayerControl + 0x38)) {
        Data += " Invent";
    }
    if (*(bool*)(PlayerInfo + 0x20)) {
        Data += " Dead";
    }
    Data += '\n';
    return Data;
}

std::string PlayerColor(DWORD PlayerControl, DWORD ModuleAddress) {
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

std::string Role(DWORD PlayerControl) {
    DWORD PlayerInfo = *(DWORD*)(PlayerControl + 0x3C);
    DWORD RoleBehaviour = *(DWORD*)(PlayerInfo + 0x18);
    BYTE RoleType = *(BYTE*)(RoleBehaviour + 0xC);

    switch (RoleType) {
    case 0:
        return "Crewmate";
    case 1:
        return "Impostor";
    case 2:
        return "Scientist";
    case 3:
        return "Engineer";
    case 4:
        return "GuardianAngel";
    case 5:
        return "Shapeshifter";
    }

    return "";
}