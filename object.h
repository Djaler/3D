#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "vector.h"
#include "matrix.h"
#include "polygon.h"

using namespace std;

class Object
{
	vector<Polygon> polygons;
	float xRotate, yRotate, zRotate;
	float xTranslate, yTranslate, zTranslate;
	float xScale, yScale, zScale;

	Mat4 rotate, translate, scale, _model;

	public:
		Object() : polygons()
		{
			xRotate = yRotate = zRotate = xTranslate = yTranslate = zTranslate = 0;
			xScale = yScale = zScale = 1;

			rotate = Mat4::rotate(xRotate, yRotate, zRotate);
			scale = Mat4::scale(xScale, yScale, zScale);
			translate = Mat4::translate(xTranslate, yTranslate, zTranslate);
		}

		Object(const char *filename) : Object()
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
			cerr << "v " << vertexes.size() << " p "  << polygonsCount() << endl;
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

		void setScale(float xScale, float yScale, float zScale)
		{
			this->xScale = xScale;
			this->yScale = yScale;
			this->zScale = zScale;

			scale = Mat4::scale(xScale, yScale, zScale);
			updateModel();
		}

		void setTranslate(float xTranslate, float yTranslate, float zTranslate)
		{
			this->xTranslate = xTranslate;
			this->yTranslate = yTranslate;
			this->zTranslate = zTranslate;

			translate = Mat4::translate(xTranslate, yTranslate, zTranslate);
			updateModel();
		}

		void setRotate(float xRotate, float yRotate, float zRotate)
		{
			this->xRotate = xRotate;
			this->yRotate = yRotate;
			this->zRotate = zRotate;

			rotate = Mat4::rotate(xRotate, yRotate, zRotate);
			updateModel();
		}

		void updateModel()
		{
			 _model = translate * rotate * scale;
		}

		Mat4 model()
		{
			return _model;
		}

};
