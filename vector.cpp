#include "vector.h"

Vec4 Vec3ToVec4(const Vec3& v)
{
	return Vec4(v.x,v.y,v.z,1.0f);
}

Vec3 Vec4ToVec3(const Vec4& v)
{
	Vec3 vector(v.x,v.y,v.z);
	vector /= v.w;
	return vector;
}
