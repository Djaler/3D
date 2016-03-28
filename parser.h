#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "vertex.h"
#include "triangle.h"

using namespace std;

class Parser
{
	vector<Vertex> verts_;

	public:
		vector<Triangle> faces_;
		Parser(const char *filename) : verts_(), faces_()
		{
			ifstream in;
			in.open (filename, ifstream::in);
			if (in.fail()) return;
			string line;
			while (!in.eof())
			{
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
					cerr << "# v# " << verts_.size()<<endl;
				}
				else if (!line.compare(0, 2, "f "))
				{
					vector<long> f;
					int itrash, idx;
					iss >> trash;
					while (iss >> idx >> trash >> itrash >> trash >> itrash)
					{
						idx--; // in wavefront obj all indices start at 1, not zero
						f.push_back(idx);
					}
					Triangle triangle(verts_[f[0]],verts_[f[1]],verts_[f[2]]);
					faces_.push_back(triangle);
					cerr << "# f# " << faces_.size()<<endl;
				}
			}
			cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << endl;
		}
		long nfaces()
		{
			return faces_.size();
		}
		Triangle face(long i)
		{
			return faces_[i];
		}
};
#endif //MODEL_H
