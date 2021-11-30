#include "includes.h"

LPD3DXFONT font;
bool Bhop;
bool GlowHack;
bool NoFlash;
bool TriggerBot;
bool espLines;
bool espBoxes;

void MainUi(LPDIRECT3DDEVICE9 pDevice) {
	ImGui::Checkbox("Enable Bhop", &Bhop);
	ImGui::Checkbox("Enable Glow Hack", &GlowHack);
	ImGui::Checkbox("Enable No Flash Hack", &NoFlash);
	if (NoFlash) {
		NoFlashFunc();
	}
	ImGui::Checkbox("Enable TriggerBot", &TriggerBot);
	if (TriggerBot) {
		TriggerBotFunc();
	}
	ImGui::Checkbox("Enable Esp Lines", &espLines);
	ImGui::Checkbox("Enable Esp Boxes", &espBoxes);
	if (espLines || espBoxes) {
		EspLines();
	}
}

bool WorldToScreen(Vec3 pos, Vec2& screen, Matrix matrix, int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vec4 clipCoords;
	clipCoords.x = pos.x * matrix.matrix[0] + pos.y * matrix.matrix[1] + pos.z * matrix.matrix[2] + matrix.matrix[3];
	clipCoords.y = pos.x * matrix.matrix[4] + pos.y * matrix.matrix[5] + pos.z * matrix.matrix[6] + matrix.matrix[7];
	clipCoords.z = pos.x * matrix.matrix[8] + pos.y * matrix.matrix[9] + pos.z * matrix.matrix[10] + matrix.matrix[11];
	clipCoords.w = pos.x * matrix.matrix[12] + pos.y * matrix.matrix[13] + pos.z * matrix.matrix[14] + matrix.matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

void BunnyHop() {
	DWORD clientDll = (DWORD)GetModuleHandle("client.dll");

	while (TRUE) {
		if (!Bhop) { Sleep(300); continue; }
		DWORD dwLocalPlayer = 0xDA746C;
		DWORD oLocalPlayer = *(DWORD*)(clientDll + dwLocalPlayer);
		if (!oLocalPlayer) { continue; }
		BYTE onGround = *(BYTE*)(oLocalPlayer + 0x104);
		if (Bhop && onGround == 1) {
			if (GetAsyncKeyState(VK_SPACE)) {
				*(BYTE*)((DWORD)clientDll + 0x526B5A0) = (BYTE)6;
			}
		}
	}
}

void GlowHackFunc() {
	HMODULE clientdll = GetModuleHandle("client.dll");
	DWORD dwGlowObjectManager = 0x5309C90;
	DWORD m_iTeamNum = 0xF4;
	DWORD m_iGlowIndex = 0x10488;
	DWORD dwEntityList = 0x4DC178C;

	while (TRUE) {
		if (GlowHack) {

			DWORD glowManager = *(DWORD*)((DWORD)clientdll + dwGlowObjectManager);

			for (int i = 1; i <= 32; i++) {
				DWORD entity = *(DWORD*)((DWORD)clientdll + dwEntityList + (i * 0x10));

				if (entity) {
					BYTE teamNum = *(BYTE*)(entity + m_iTeamNum);
					DWORD entityGlow = *(DWORD*)(entity + m_iGlowIndex);

					if (teamNum == 2) {
						*(float*)(glowManager + (entityGlow * 0x38) + 0x8) = (float)1; //R
						*(float*)(glowManager + (entityGlow * 0x38) + 0xC) = (float)0; //G
						*(float*)(glowManager + (entityGlow * 0x38) + 0x10) = (float)0; //B
						*(float*)(glowManager + (entityGlow * 0x38) + 0x14) = (float)1; //Alpha
						*(DWORD*)(glowManager + (entityGlow * 0x38) + 0x28) = 1; //Enable Glow
					}
					else if (teamNum == 3) {
						*(float*)(glowManager + (entityGlow * 0x38) + 0x8) = (float)0; //R
						*(float*)(glowManager + (entityGlow * 0x38) + 0xC) = (float)0; //G
						*(float*)(glowManager + (entityGlow * 0x38) + 0x10) = (float)1; //B
						*(float*)(glowManager + (entityGlow * 0x38) + 0x14) = (float)1; //Alpha
						*(DWORD*)(glowManager + (entityGlow * 0x38) + 0x28) = 1; //Enable Glow
					}
				}
			}
		}
		else if (!GlowHack) {
			Sleep(300);
		}
	}
}

void NoFlashFunc() {
	DWORD clientdll = (DWORD)GetModuleHandle("client.dll");
	DWORD dwLocalPlayer = 0xDA746C;
	DWORD m_flFlashMaxAlpha = 0x1046C;

	DWORD LocalPlayerPointer = *(DWORD*)(clientdll + dwLocalPlayer);
	if (LocalPlayerPointer) {
		*(float*)(LocalPlayerPointer + m_flFlashMaxAlpha) = (float)0;
	}
}

void TriggerBotFunc() {
	if (!GetAsyncKeyState(VK_LSHIFT)) {
		return;
	}
	DWORD clientdll = (DWORD)GetModuleHandle("client.dll");

	DWORD dwLocalPlayer = 0xDA746C;
	DWORD dwEntityList = 0x4DC178C;
	DWORD m_iTeamNum = 0xF4;
	DWORD dwForceAttack = 0x31F1D14;
	DWORD m_iCrosshairId = 0x11838;

	DWORD LocalPlayerPointer = *(DWORD*)(clientdll + dwLocalPlayer);
	if (!LocalPlayerPointer) { return; }
	BYTE LocalPlayerTeam = *(BYTE*)(LocalPlayerPointer + m_iTeamNum);
	BYTE CrossHairId = *(BYTE*)(LocalPlayerPointer + m_iCrosshairId);
	if (!CrossHairId) { return; }
	DWORD Entity = *(DWORD*)(clientdll + dwEntityList + (CrossHairId - 1) * 0x10);
	if (!Entity) { return; }
	BYTE EntityTeam = *(BYTE*)(Entity + m_iTeamNum);

	if (CrossHairId > 0 && CrossHairId <= 32 && EntityTeam != LocalPlayerTeam) {
		*(BYTE*)(clientdll + dwForceAttack) = (BYTE)6;
	}
}

void EspLines() {
	ImDrawList* drawlist = ImGui::GetBackgroundDrawList();

	int width = ImGui::GetIO().DisplaySize.x;
	int height = ImGui::GetIO().DisplaySize.y;

	DWORD Module = (DWORD)GetModuleHandle("client.dll");
	DWORD ViewMatrix = Module + 0x4DB30A4;
	DWORD LocalPlayer = *(DWORD*)(Module + 0xDA746C);
	DWORD EntityList = *(DWORD*)(Module + 0x4DC178C);
	int MyTeam = *(DWORD*)(LocalPlayer + 0xF4);

	Matrix localMatrix = *(Matrix*)(ViewMatrix);

	for (int i = 1; i < 64; i++) {

		DWORD Player = *(DWORD*)(Module + 0x4DC178C + i * 0x10);
		if (!Player || Player == LocalPlayer) { continue; }

		DWORD dwBoneMatrix = *(DWORD*)(Player + 0x26A8);

		Vec3 PlayerPosition = *(Vec3*)(Player + 0x138);

		int Team = *(int*)(Player + 0xF4);
		int Health = *(int*)(Player + 0x100);
		int Dormant = *(int*)(Player + 0xED);

		if (Dormant == 0) {
			if (Health > 0 && Health < 101) {
				Vec2 Screen;
				if (WorldToScreen(PlayerPosition, Screen, localMatrix, width, height)) {
					Vec3 HeadPosition;
					HeadPosition.x = *(float*)(dwBoneMatrix + 0x30 * 9 + 0x0C);
					HeadPosition.y = *(float*)(dwBoneMatrix + 0x30 * 9 + 0x1C);
					HeadPosition.z = *(float*)(dwBoneMatrix + 0x30 * 9 + 0x2C);

					Vec2 Head;
					if (WorldToScreen(HeadPosition, Head, localMatrix, width, height)) {
						float head = Head.y - Screen.y;
						float widths = head / 2;
						float center = widths / 2;
						ImVec2 UpperLeft = ImVec2(Screen.x - (widths / 2), Head.y);
						ImVec2 LowerRight = ImVec2(Screen.x + (widths / 2), Screen.y);

						if (MyTeam == Team) {
							if (espBoxes){
								drawlist->AddRect(UpperLeft, LowerRight, ImColor(0, 0, 255, 255), 0.0F, 15, 2.0F);
							}
							if (espLines) {
								drawlist->AddLine(ImVec2(width / 2, height), ImVec2(Screen.x, Screen.y), ImColor(0, 0, 255, 255), 2.0F);
							}
						}
						else {
							if (espBoxes) {
								drawlist->AddRect(UpperLeft, LowerRight, ImColor(255, 0, 0, 255), 0.0F, 15, 2.0F);
							}
							if (espLines) {
								drawlist->AddLine(ImVec2(width / 2, height), ImVec2(Screen.x, Screen.y), ImColor(255, 0, 0, 255), 2.0F);
							}
						}
					}
				}
			}
		}
	}
}