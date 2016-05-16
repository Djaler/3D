#pragma once

#include "vector.h"

struct Polygon
{
	Vec3 v1;
	Vec3 v2;
	Vec3 v3;
	float intensity;

	Polygon(Vec3 v1, Vec3 v2, Vec3 v3, float intensity = 0) :
		v1(v1), v2(v2), v3(v3), intensity(intensity) {}
};
