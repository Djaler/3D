#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex.h"

struct Triangle
{
	Vertex v1;
	Vertex v2;
	Vertex v3;
	Triangle(Vertex v1_, Vertex v2_, Vertex v3_)
	{
		v1 = v1_;
		v2 = v2_;
		v3 = v3_;
	}
};

#endif // TRIANGLE_H
