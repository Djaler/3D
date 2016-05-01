#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "vector.h"
#include "polygon.h"

using namespace std;

struct Parser
{
	vector<Vec3> vertexes;
	vector<Polygon> polygons;

	Parser(const char *filename, float scale_factor=1) : vertexes(), polygons()
	{
		ifstream in;
		in.open (filename, ifstream::in);
		if (in.fail())
		{
			return;
		}
		string line;
		while (!in.eof())
		{
			getline(in, line);
			istringstream iss(line.c_str());
			char trash;
			if (!line.compare(0, 2, "v "))
			{
				iss >> trash;

				float x, y, z;
				iss >> x;
				iss >> y;
				iss >> z;
				Vec3 v(x, y, z);
				if(scale_factor != 1)
				{
					v *= scale_factor;
				}
				vertexes.push_back(v);
				cerr << "v " << vertexes.size()<<endl;
			}
			else if (!line.compare(0, 2, "f "))
			{
				vector<Vec3> t;
				int itrash, idx;
				iss >> trash;
				while (iss >> idx >> trash >> itrash >> trash >> itrash)
				{
					idx--;
					t.push_back(vertexes[idx]);
				}
				polygons.push_back(Polygon(t[0], t[1], t[2]));
				cerr << "t " << polygons.size()<<endl;
			}
		}
		cerr << "v " << vertexes.size() << "t "  << polygons.size() << endl;
	}
};
#endif //MODEL_H
