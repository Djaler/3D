#include <fstream>
#include <sstream>
#include <iostream>
#include "object.h"

float Object::xRotate() const
{
	return _xRotate;
}

float Object::yRotate() const
{
	return _yRotate;
}

float Object::zRotate() const
{
	return _zRotate;
}

float Object::xTranslate() const
{
	return _xTranslate;
}

float Object::yTranslate() const
{
	return _yTranslate;
}

float Object::zTranslate() const
{
	return _zTranslate;
}

float Object::xScale() const
{
	return _xScale;
}

float Object::yScale() const
{
	return _yScale;
}

float Object::zScale() const
{
	return _zScale;
}

void Object::setScale(float xScale, float yScale, float zScale)
{
	_xScale = xScale;
	_yScale = yScale;
	_zScale = zScale;

	scale = Mat4::scale(xScale, yScale, zScale);
}

void Object::setTranslate(float xTranslate, float yTranslate, float zTranslate)
{
	_xTranslate = xTranslate;
	_yTranslate = yTranslate;
	_zTranslate = zTranslate;

	translate = Mat4::translate(xTranslate, yTranslate, zTranslate);
}

void Object::setRotate(float xRotate, float yRotate, float zRotate)
{
	_xRotate = xRotate;
	_yRotate = yRotate;
	_zRotate = zRotate;

	rotate = Mat4::rotate(xRotate, yRotate, zRotate);
}

Object::Object() : polygons()
{
	_xRotate = _yRotate = _zRotate = _xTranslate = _yTranslate = _zTranslate = 0;
	_xScale = _yScale = _zScale = 1;

	rotate = Mat4::rotate(_xRotate, _yRotate, _zRotate);
	scale = Mat4::scale(_xScale, _yScale, _zScale);
	translate = Mat4::translate(_xTranslate, _yTranslate, _zTranslate);

	_model = Mat4::identity();
}

Object::Object(const char *filename) : Object()
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

void Object::addPolygon(Polygon polygon)
{
	polygons.push_back(polygon);
}

Polygon &Object::polygon(size_t index)
{
	return polygons[index];
}

size_t Object::polygonsCount()
{
	return polygons.size();
}

void Object::updateModel()
{
	_model = translate * rotate * scale;
}

Mat4 Object::model()
{
	return _model;
}
