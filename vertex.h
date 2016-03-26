#ifndef VERTEX_H
#define VERTEX_H

#include <QGenericMatrix>

class Vertex
{
	public:
		float x;
		float y;
		float z;
		Vertex()
		{
			x=0;
			y=0;
			z=0;
		}

		Vertex(float x_, float y_, float z_)
		{
			x = x_;
			y = y_;
			z = z_;
		}
		void translate(int x_, int y_)
		{
			x+=x_;
			y+=y_;
		}
		void scale(float factor)
		{
			x*=factor;
			y*=factor;
			z*=factor;
		}
};

#endif // VERTEX_H
