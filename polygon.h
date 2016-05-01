#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

struct Polygon
{
	Vec3 v1;
	Vec3 v2;
	Vec3 v3;

	Polygon(Vec3 v1_, Vec3 v2_, Vec3 v3_)
	{
		v1 = v1_;
		v2 = v2_;
		v3 = v3_;
	}
};

#endif // TRIANGLE_H
