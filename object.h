#pragma once

#include <vector>
#include "vector.h"
#include "matrix.h"
#include "polygon.h"

using namespace std;

class Object
{
	vector<Polygon> polygons;
	float _xRotate, _yRotate, _zRotate;
	float _xTranslate, _yTranslate, _zTranslate;
	float _xScale, _yScale, _zScale;

	Mat4 rotate, translate, scale, _model;

	public:
		Object();
		Object(const char *filename);
		void addPolygon(Polygon polygon);
		Polygon& polygon(size_t index);
		Mat4 model();
		size_t polygonsCount();

		float xRotate() const;
		float yRotate() const;
		float zRotate() const;;
		float xTranslate() const;
		float yTranslate() const;
		float zTranslate() const;
		float xScale() const;
		float yScale() const;
		float zScale() const;

		void setScale(float xScale, float yScale, float zScale);
		void setTranslate(float xTranslate, float yTranslate, float zTranslate);
		void setRotate(float xRotate, float yRotate, float zRotate);

		void updateModel();

};
