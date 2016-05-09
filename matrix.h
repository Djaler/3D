#pragma once

#include <QDebug>
#include <QtMath>
#include "vector.h"

class Mat4
{
	float m[16];

	public:
		Mat4() : m
		{
					 0, 0, 0, 0,
					 0, 0, 0, 0,
					 0, 0, 0, 0,
					 0, 0, 0, 0
		} {}

		Mat4 operator*(const Mat4 &other) const
		{
			Mat4 result;

			for(int i = 0; i < 4; i++)
			{
				for(int j = 0 ; j < 4; j++)
				{
					for(int k = 0; k < 4; k++)
					{
						result.m[j + i * 4] += other.m[k + i * 4] * m[j + k * 4];
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

		float& operator[](int i)
		{
			return m[i];
		}

		static Mat4 identity()
		{
			Mat4 identity;

			identity[0 + 0 * 4] = 1;
			identity[1 + 1 * 4] = 1;
			identity[2 + 2 * 4] = 1;
			identity[3 + 3 * 4] = 1;

			return identity;
		}

		static Mat4 rotate(float xAngle = 0.0f, float yAngle = 0.0f, float zAngle = 0.0f)
		{
			float radians = qDegreesToRadians(xAngle);
			float sine = sin(radians);
			float cosine = cos(radians);
			Mat4 x = Mat4::identity();
			x[1 + 1 * 4] = cosine;
			x[2 + 1 * 4] = sine;
			x[1 + 2 * 4] = -sine;
			x[2 + 2 * 4] = cosine;

			radians = qDegreesToRadians(yAngle);
			sine = sin(radians);
			cosine = cos(radians);
			Mat4 y = Mat4::identity();
			y[0 + 0 * 4] = cosine;
			y[2 + 0 * 4] = -sine;
			y[0 + 2 * 4] = sine;
			y[2 + 2 * 4] = cosine;

			radians = qDegreesToRadians(zAngle);
			sine = sin(radians);
			cosine = cos(radians);
			Mat4 z = Mat4::identity();
			z[0 + 0 * 4] = cosine;
			z[1 + 0 * 4] = sine;
			z[0 + 1 * 4] = -sine;
			z[1 + 1 * 4] = cosine;

			return z * y * x;
		}

		static Mat4 translate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
		{
			Mat4 translate = Mat4::identity();

			translate[0 + 3 * 4] = x;
			translate[1 + 3 * 4] = y;
			translate[2 + 3 * 4] = z;

			return translate;
		}

		static Mat4 scale(float x = 1.0f, float y = 1.0f, float z = 1.0f)
		{
			Mat4 scale = Mat4::identity();

			scale[0 + 0 * 4] = x;
			scale[1 + 1 * 4] = y;
			scale[2 + 2 * 4] = z;

			return scale;
		}

		static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
		{
			Vec3 forward = (center - eye).normalized();
			Vec3 side = Vec3::crossProduct(up, forward).normalized();
			Vec3 upVector = Vec3::crossProduct(forward, side);

			Mat4 lookAt = Mat4::identity();

			lookAt[0 + 0 * 4] = side.x;
			lookAt[1 + 0 * 4] = upVector.x;
			lookAt[2 + 0 * 4] = forward.x;

			lookAt[0 + 1 * 4] = side.y;
			lookAt[1 + 1 * 4] = upVector.y;
			lookAt[2 + 1 * 4] = forward.y;

			lookAt[0 + 2 * 4] = side.z;
			lookAt[1 + 2 * 4] = upVector.z;
			lookAt[2 + 2 * 4] = forward.z;

			lookAt[0 + 3 * 4] = -Vec3::dotProduct(side, eye);
			lookAt[1 + 3 * 4] = -Vec3::dotProduct(upVector, eye);
			lookAt[2 + 3 * 4] = -Vec3::dotProduct(forward, eye);

			return lookAt;
		}

		void print()
		{
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					qDebug() << m[j + i * 4] << " ";
				}
				qDebug() << endl;
			}
		}
};
