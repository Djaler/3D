#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "vector.h"
#include "polygon.h"

using namespace std;

class Object
{
	vector<Vec3> vertexes;
	vector<Polygon> polygons;

	public:
		Object(const char *filename) : vertexes(), polygons()
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
					addVertex(Vec3(x, y, z));

				}
				else if (!line.compare(0, 2, "f "))
				{
					vector<Vec3> polygon;
					size_t itrash, idx;
					iss >> trash;
					while (iss >> idx >> trash >> itrash >> trash >> itrash)
					{
						idx--;
						polygon.push_back(vertexes[idx]);
					}
					addPolygon(Polygon(polygon[0], polygon[1], polygon[2]));
				}
			}
			cerr << "v " << vertexesCount() << "t "  << polygonsCount() << endl;
		}

		void addVertex(Vec3 vertex)
		{
			vertexes.push_back(vertex);
		}

		void addPolygon(Polygon polygon)
		{
			polygons.push_back(polygon);
		}

		Polygon& polygon(size_t index)
		{
			return polygons[index];
		}

		size_t vertexesCount()
		{
			return vertexes.size();
		}

		size_t polygonsCount()
		{
			return polygons.size();
		}
};
