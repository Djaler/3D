#ifndef VERTEX_H
#define VERTEX_H

#include "matrix.h"

struct Vertex
{
	float x;
	float y;
	float z;

	Vertex()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vertex(float x_, float y_, float z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	void translate(int x_, int y_)
	{
		x += x_;
		y += y_;
	}

	void scale(float factor)
	{
		x *= factor;
		y *= factor;
		z *= factor;
	}

	Vertex transformed(Matrix transform)
	{
		return Vertex(	x * transform[0] + y * transform[3] + z * transform[6],
						x * transform[1] + y * transform[4] + z * transform[7],
						x * transform[2] + y * transform[5] + z * transform[8]);
	}

	Vertex operator -(const Vertex& v)
	{
		return Vertex(x - v.x, y - v.y, z - v.z);
	}

	Vertex operator *(const Vertex& v)
	{
		return Vertex(y * v.z - z * v.y,
					  z * v.x - x * v.z,
					  x * v.y - y * v.x);
	}

	void normalize()
	{
		float length = sqrt(x * x + y * y + z * z);
		x /= length;
		y /= length;
		z /= length;
	}
};

#endif // VERTEX_H
