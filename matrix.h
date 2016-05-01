#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

struct Mat4
{
	float m[16];
	Mat4(float m[16]) : m
	{
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15],
	} { }

	Mat4() : m
	{
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	} { }

	Mat4 operator *(const Mat4 &other) const
	{
		Mat4 result;

		for(int i = 0; i < 4; i++)
		{
			for(int j = 0 ; j < 4; j++)
			{
				result.m[i + j * 4] = m[i + 0 * 4] * other.m[0 + j * 4] +
									  m[i + 1 * 4] * other.m[1 + j * 4] +
									  m[i + 2 * 4] * other.m[2 + j * 4] +
									  m[i + 3 * 4] * other.m[3 + j * 4];
			}
		}

		return result;
	}

	Mat4 operator *(const float f) const
	{
		Mat4 result;

		for(int i = 0; i < 16; i++)
		{
			result.m[i] = m[i] * f;
		}

		return result;
	}

	Vec4 operator*(const Vec4 &v) const
	{
		Vec4 result;

		for (int i = 0; i < 4; i++)
		{
			result[i] = v[0] * m[i + 0 * 4] + v[1] * m[i + 1 * 4] + v[2] * m[i + 2 * 4] + v[3] * m[i + 3 * 4];
		}

		return result;
	}

	Vec3 operator*(const Vec3 &v) const
	{
		Vec3 result;

		for (int i = 0; i < 3; i++)
		{
			result[i] = v[0] * m[i + 0 * 4] + v[1] * m[i + 1 * 4] + v[2] * m[i + 2 * 4];
		}

		return result;
	}

	float operator[](int i) const
	{
		return m[i];
	}

	float &operator[](int i)
	{
		return m[i];
	}

	Mat4 clear()
	{
		for(int i = 0; i < 16; i++)
		{
			m[i] = 0;
		}

		return *this;
	}

	Mat4 identity()
	{
		clear();

		m[0 + 0 * 4] = 1;
		m[1 + 1 * 4] = 1;
		m[2 + 2 * 4] = 1;
		m[3 + 3 * 4] = 1;

		return *this;
	}
};

#endif // MATRIX_H
