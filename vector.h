#pragma once

#include <cmath>

class Vec4;

class Vec3
{
	public:
		float x, y, z;

		Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f);

		Vec3& operator+=(const Vec3 &other);

		Vec3 operator+(const Vec3 &other) const;

		Vec3 operator-(const Vec3 &other) const;

		static float dotProduct(const Vec3 &v1, const Vec3 &v2);

		static Vec3 crossProduct(const Vec3 &v1, const Vec3 &v2);

		static Vec3 normal(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3);

		float length() const;

		Vec3 normalized() const;

		float& operator[](int i);

		Vec4 toVec4();

};

class Vec4
{
	public:
		float x, y, z, w;

		Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);

		float& operator[](int i);

		Vec3 toVec3();
};


