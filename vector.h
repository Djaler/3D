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

	Vec3 operator *(const Vec3 &other) const
	{
		return Vec3(x * other.x, y * other.y, z * other.z);
	}

	Vec3 operator ^(const Vec3 &other) const
	{
		return Vec3(y * other.z - z * other.y,
					z * other.x - x * other.z,
					x * other.y - y * other.x);
	}

	float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 normalize() const
	{
		float l = 1.0f / length();
		return Vec3(x * l, y * l, z * l);
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

		Vec4 operator ^(const Vec4 &other) const
		{
			return Vec4(y * other.z - z * other.y,
						z * other.x - x * other.z,
						x * other.y - y * other.x);
		}

		float length() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		Vec4 normalize() const
		{
			float l = 1.0f / length();
			return Vec4(x * l, y * l, z * l);
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


