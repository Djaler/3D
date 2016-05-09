#pragma once

#include <fstream>
#include <sstream>
#include <QDebug>
#include <vector>
#include "vector.h"
#include "polygon.h"

using namespace std;

class Object
{
	vector<Polygon> polygons;

	public:
		Object() : polygons() {}

		Object(const char *filename) : polygons()
		{
			vector<Vec3> vertexes;
			ifstream in;
			in.open(filename);
			if(in.fail())
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
					vertexes.push_back(Vec3(x, y, z));

				}
				else if (!line.compare(0, 2, "f "))
				{
					vector<Vec3> polygon;
					size_t itrash, idx;
					iss >> trash;
					while (iss >> idx >> trash >> itrash >> trash >> itrash)
					{
						polygon.push_back(vertexes[idx - 1]);
					}
					addPolygon(Polygon(polygon[0], polygon[1], polygon[2]));
				}
			}
			qDebug() << "v " << vertexes.size() << "t "  << polygonsCount() << endl;
		}


		void addPolygon(Polygon polygon)
		{
			polygons.push_back(polygon);
		}

		Polygon& polygon(size_t index)
		{
			return polygons[index];
		}

		size_t polygonsCount()
		{
			return polygons.size();
		}
};
