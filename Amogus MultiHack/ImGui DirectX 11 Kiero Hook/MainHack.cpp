#include "includes.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <d3d11.h>
#include "Functions.h"

ImVec2 GetFeet(DWORD PlayerControl, DWORD cum);
ImVec2 GetHead(DWORD PlayerControl, DWORD cum);

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

void ESP() {
    DWORD cum = get_current_Camera();
    ImDrawList* drawlist = ImGui::GetBackgroundDrawList();

    //LocalPlayer
    DWORD PlayerControl_c = *(DWORD*)((DWORD)GetModuleHandle("GameAssembly.dll") + 29436076);
    DWORD PlayerControl_StaticFields = *(DWORD*)(PlayerControl_c + 0x5C);
    DWORD LocalPlayer = *(DWORD*)(PlayerControl_StaticFields + 0x00);
    if (!LocalPlayer) { return; }

    //PlayerList
    std::vector<DWORD> oPlayerList{ 0xDC, 0x28, 0x48, 0x98, 0xC, 0x5C, 0x8, 0x8, 0x10 };
    DWORD baseAdress = GameAssembly + 0x01BEABF4;
    DWORD playerList = GetOffset(baseAdress, oPlayerList);

    for (int i = 0; i <= 128; i = i + 4) {
        DWORD playerControl = *(DWORD*)(playerList + i);
        if (playerControl == 0) { break; }
        if (playerControl == LocalPlayer) {
            continue;
        }
        
        ImColor espcolor;
        if (Role(playerControl) == "Impostor" || Role(playerControl) == "Shapeshifter") {
            espcolor = ImColor(255,0,0,255);
        }
        else {
            espcolor = ImColor(0, 0, 255, 255);
        }

        ImVec2 LocalFeet = GetFeet(LocalPlayer, cum);
        ImVec2 PlayerHead = GetHead(playerControl, cum);
        ImVec2 PlayerFeet = GetFeet(playerControl, cum);

        if (esplines) {
            drawlist->AddLine(LocalFeet, PlayerFeet, espcolor);
        }
        if (espbox) {
            float height = PlayerHead.y - PlayerFeet.y;
            float width = (height / 2) + 100.f;
            drawlist->AddRect(ImVec2(PlayerHead.x - (width/2), PlayerHead.y), ImVec2(PlayerFeet.x + (width/2), PlayerFeet.y), espcolor);
        }
    }
}

ImVec2 GetHead(DWORD PlayerControl, DWORD cum) {
    DWORD PlayerTransform = get_Transform((void*)PlayerControl);
    Vec3 PlayerPos = get_Position((void*)PlayerTransform);
    PlayerPos.y += .4f;
    Vec3 w2s = WorldToScreenPoint((void*)cum, PlayerPos);
    return ImVec2(w2s.x, ImGui::GetIO().DisplaySize.y - w2s.y);
}

ImVec2 GetFeet(DWORD PlayerControl, DWORD cum) {
    DWORD PlayerTransform = get_Transform((void*)PlayerControl);
    Vec3 PlayerPos = get_Position((void*)PlayerTransform);
    PlayerPos.y -= .4f;
    Vec3 w2s = WorldToScreenPoint((void*)cum, PlayerPos);
    return ImVec2(w2s.x, ImGui::GetIO().DisplaySize.y - w2s.y);
}