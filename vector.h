#pragma once
#include <cmath>

struct Vec4;

struct Vec3
{
	float x, y, z;

	Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

	Vec3 &operator+=(const Vec3 &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3 &operator*=(const float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	Vec3 &operator/=(const float f)
	{
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}

	Vec3 operator +(const Vec3 &other) const
	{
		return Vec3(x + other.x, y + other.y, z + other.z);
	}

	Vec3 operator -(const Vec3 &other) const
	{
		return Vec3(x - other.x, y - other.y, z - other.z);
	}

	static float dotProduct(const Vec3& v1, const Vec3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	static Vec3 crossProduct(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(	v1.y * v2.z - v1.z * v2.y,
						v1.z * v2.x - v1.x * v2.z,
						v1.x * v2.y - v1.y * v2.x);
	}

	static Vec3 normal(const Vec3& v1, const Vec3& v2, const Vec3& v3)
	{
		return crossProduct((v2 - v1), (v3 - v1)).normalized();
	}

	float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 normalized() const
	{
		float l = length();
		return Vec3(x / l, y / l, z / l);
	}

	float operator[](int i) const
	{
		if(i==0) return x;
		else if(i==1) return y;
		else return z;
	}

	float& operator[](int i)
	{
		if(i==0) return x;
		else if(i==1) return y;
		else return z;
	}
};

struct Vec4
{
		float x, y, z, w;

		Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w) {}

		Vec4 &operator+=(const Vec4 &other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		Vec4 &operator*=(const float f)
		{
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}

		Vec4 operator +(const Vec4 &other) const
		{
			return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		Vec4 operator -(const Vec4 &other) const
		{
			return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		Vec4 operator *(const Vec4 &other) const
		{
			return Vec4(x * other.x, y * other.y, z * other.z, w * other.w);
		}

		float operator[](int i) const
		{
			if(i==0) return x;
			else if(i==1) return y;
			else if(i==2) return z;
			else return w;
		}

		float& operator[](int i)
		{
			if(i==0) return x;
			else if(i==1) return y;
			else if(i==2) return z;
			else return w;
		}
};

Vec4 Vec3ToVec4(const Vec3& v);
Vec3 Vec4ToVec3(const Vec4& v);


