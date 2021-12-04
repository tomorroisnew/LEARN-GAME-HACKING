#pragma once
#include <cmath>

struct Vec3 {
	float x, y, z;
};

struct Rotate {
	float pitch, yaw; //yaw right to left, pitch up an down
};

Rotate CalcAngle(Vec3 player, Vec3 target) {
	Rotate rotate;

	//Calculate yaw
	float a = target.x - player.x;
	float b = target.y - player.y;
	float c = sqrt(a*a + b*b);
	float angle = atan2(b,  a);
	rotate.yaw = angle * (180 / 3.14159265359);
	

	//Calculate pitch
	a = target.y - player.y;
	b = target.z - player.z;
	std::cout << a << ", " << b << std::endl;
	c = sqrt(a * a + b * b);
	angle = -atan(b / a);

	rotate.pitch = angle * (180 / 3.14159265359);

	return rotate;
}