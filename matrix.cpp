#include <iostream>
#include <QtMath>
#include "vector.h"
#include "matrix.h"

using namespace std;

Mat4::Mat4() : m
{
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
} {}

Vec3 Mat4::operator*(const Vec3 &v) const
{
	Vec3 result;

	for (int i = 0; i < 3; i++)
	{
		result[i] = v.x * m[i + 0 * 4] + v.y * m[i + 1 * 4] + v.z * m[i + 2 * 4];
	}

	return result;
}

float &Mat4::operator[](int i)
{
	return m[i];
}

Mat4 Mat4::identity()
{
	Mat4 identity;

	identity[0 + 0 * 4] = 1;
	identity[1 + 1 * 4] = 1;
	identity[2 + 2 * 4] = 1;
	identity[3 + 3 * 4] = 1;

	return identity;
}

Mat4 Mat4::rotate(float xAngle, float yAngle, float zAngle)
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

Mat4 Mat4::translate(float x, float y, float z)
{
	Mat4 translate = Mat4::identity();

	translate[0 + 3 * 4] = x;
	translate[1 + 3 * 4] = y;
	translate[2 + 3 * 4] = z;

	return translate;
}

Mat4 Mat4::scale(float x, float y, float z)
{
	Mat4 scale = Mat4::identity();

	scale[0 + 0 * 4] = x;
	scale[1 + 1 * 4] = y;
	scale[2 + 2 * 4] = z;

	return scale;
}

Mat4 Mat4::lookAt(const Vec3 &eye, const Vec3 &center)
{
	Vec3 forward = (center - eye).normalized();
	Vec3 side = Vec3::crossProduct(Vec3(0, 1, 0), forward).normalized();
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

Mat4 Mat4::perspective(float fov, float aspect, float near, float far)
{
	Mat4 perspective;

	float yScale = 1 / tan(qDegreesToRadians(fov / 2));
	float xScale = yScale / aspect;

	perspective[0 + 0 * 4] = xScale;
	perspective[1 + 1 * 4] = yScale;
	perspective[2 + 2 * 4] = far / (far - near);
	perspective[3 + 2 * 4] = 1;
	perspective[2 + 3 * 4] = -near * far / (far - near);

	return perspective;
}

Mat4 Mat4::viewport(float width, float height)
{
	Mat4 viewport = Mat4::identity();

	viewport[0 + 0 * 4] = width / 2;
	viewport[0 + 3 * 4] = width / 2;
	viewport[1 + 1 * 4] = height / 2;
	viewport[1 + 3 * 4] = height / 2;

	return viewport;
}

Vec4 Mat4::operator*(const Vec4 &v) const
{
	Vec4 result;

	for (int i = 0; i < 4; i++)
	{
		result[i] = v.x * m[i + 0 * 4] + v.y * m[i + 1 * 4] + v.z * m[i + 2 * 4] + v.w * m[i + 3 * 4];
	}

	return result;
}

Mat4 Mat4::operator*(const Mat4 &other) const
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
