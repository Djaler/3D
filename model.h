#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "vertex.h"
#include "triangle.h"

using namespace std;

class Model
{
	private:
		vector<Vertex> verts_;

	public:
		vector<Triangle> faces_;
		Model(const char *filename);
		int nfaces();
		Vertex vert(int i);
		Triangle face(int idx);
};
#endif //MODEL_H
