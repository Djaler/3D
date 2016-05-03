#pragma once

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
		result.clear();
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0 ; j < 4; j++)
			{
				for(int k = 0; k < 4; k++)
				{
					result.m[i + j * 4] += m[i + k * 4] * other.m[k + j * 4];
				}
			}
		}

		return result;
	}

	Vec4 operator*(const Vec4 &v) const
	{
		Vec4 result;

		for (int i = 0; i < 4; i++)
		{
			result[i] = v.x * m[i + 0 * 4] + v.y * m[i + 1 * 4] + v.z * m[i + 2 * 4] + v.w * m[i + 3 * 4];
		}

		return result;
	}

	Vec3 operator*(const Vec3 &v) const
	{
		Vec3 result;

		for (int i = 0; i < 3; i++)
		{
			result[i] = v.x * m[i + 0 * 4] + v.y * m[i + 1 * 4] + v.z * m[i + 2 * 4];
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

	static Mat4 rotate(float xAngle = 0.0f, float yAngle = 0.0f, float zAngle = 0.0f)
	{
		xAngle *= M_PI / 180;
		yAngle *= M_PI / 180;
		zAngle *= M_PI / 180;

		float a = cos(xAngle);
		float b = sin(xAngle);
		float c = cos(yAngle);
		float d = sin(yAngle);
		float e = cos(zAngle);
		float f = sin(zAngle);

		Mat4 rotate;
		rotate.identity();
		rotate[0 + 0 * 4] = c * e;
		rotate[1 + 0 * 4] = -c * f;
		rotate[2 + 0 * 4] = -d;
		rotate[0 + 1 * 4] = -b * d * e + a * f;
		rotate[1 + 1 * 4] = b * d * f + a * e;
		rotate[2 + 1 * 4] = -b * c;
		rotate[0 + 2 * 4] = a * d * e + b * f;
		rotate[1 + 2 * 4] = -a * d * f + b * e;
		rotate[2 + 2 * 4] = a * c;


		return rotate;
	}

	static Mat4 translate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		Mat4 translate;
		translate.identity();
		translate[0 + 3 * 4] = x;
		translate[1 + 3 * 4] = y;
		translate[2 + 3 * 4] = z;
		return translate;
	}

	void print()
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				cerr<<m[j + i * 4]<<" ";
			}
			cerr<<endl;
		}
	}
};
