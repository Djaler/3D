#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_()
{
	ifstream in;
	in.open (filename, ifstream::in);
	if (in.fail()) return;
	string line;
	while (!in.eof()) {
		getline(in, line);
		istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;

			float x,y,z;
			iss >> x;
			iss >> y;
			iss >> z;

			Vertex v(x,y,z);
			verts_.push_back(v);
		}
		else if (!line.compare(0, 2, "f "))
		{
			vector<int> f;
			int itrash, idx;
			iss >> trash;
			while (iss >> idx >> trash >> itrash >> trash >> itrash)
			{
				idx--; // in wavefront obj all indices start at 1, not zero
				f.push_back(idx);
			}
			Triangle triangle(vert(f[0]),vert(f[1]),vert(f[2]));
			faces_.push_back(triangle);
		}
	}
	cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << endl;
}

int Model::nfaces() {
	return (int)faces_.size();
}

Triangle Model::face(int idx) {
	return faces_[idx];
}

Vertex Model::vert(int i) {
	return verts_[i];
}
