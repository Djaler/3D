#include "vector.h"

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3 &Vec3::operator+=(const Vec3 &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vec3 Vec3::operator+(const Vec3 &other) const
{
	return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3 &other) const
{
	return Vec3(x - other.x, y - other.y, z - other.z);
}

float Vec3::dotProduct(const Vec3 &v1, const Vec3 &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 Vec3::crossProduct(const Vec3 &v1, const Vec3 &v2)
{
	return Vec3(v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x);
}

Vec3 Vec3::normal(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3)
{
	return crossProduct(v2 - v1, v3 - v1).normalized();
}

float Vec3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalized() const
{
	float l = length();
	return Vec3(x / l, y / l, z / l);
}

float &Vec3::operator[](int i)
{
	if(i == 0) return x;
	else if(i == 1) return y;
	else return z;
}

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

float &Vec4::operator[](int i)
{
	if(i == 0) return x;
	else if(i == 1) return y;
	else if(i == 2) return z;
	else return w;
}

Vec3 Vec4::toVec3()
{
	return Vec3(x / w, y / w, z / w);
}

Vec4 Vec3::toVec4()
{
	return Vec4(x, y, z, 1);
}
