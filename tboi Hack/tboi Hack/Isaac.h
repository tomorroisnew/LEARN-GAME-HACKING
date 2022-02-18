// Generated using ReClass 2016

struct Vector2 {
	float x, y;
};



class Player;
class CharacterInfo;

class Player
{
public:
	char pad_0x0000[0x28C]; //0x0000
	Vector2 CurrRoomPosition; //0x028C 
	char pad_0x0294[0xEE0]; //0x0294
	__int32 MaxHeart; //0x1174 
	__int32 Heart; //0x1178 
	char pad_0x117C[0x14]; //0x117C
	__int32 Keys; //0x1190 
	char pad_0x1194[0x4]; //0x1194
	__int32 Bombs; //0x1198 
	__int32 Coins; //0x119C 
	char pad_0x11A0[0x54]; //0x11A0
	CharacterInfo* CharacterInfo; //0x11F4 
	char pad_0x11F8[0x80]; //0x11F8
	float TearSpeed; //0x1278 
	char pad_0x127C[0x4]; //0x127C
	__int32 TotalTearCount; //0x1280 
	float Damage; //0x1284 
	char pad_0x1288[0xEC]; //0x1288
	float MovementSpeed; //0x1374 
	float Luck; //0x1378 
	char pad_0x137C[0xC]; //0x137C
	float DamageUi; //0x1388 
	char pad_0x138C[0x4]; //0x138C
	__int32 Charge; //0x1390 
	char pad_0x1394[0xCAC]; //0x1394

}; //Size=0x2040

class CharacterInfo
{
public:
	char CharacterName[8]; //0xBF5888 

}; //Size=0x0008

