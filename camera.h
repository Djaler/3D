#pragma once

#include "vector.h"
#include "matrix.h"

class Camera
{
	Vec3 _eye;
	Vec3 _center;
	Vec3 _light;
	Mat4 _view;

	public:
		Camera(Vec3 eye, Vec3 center, Vec3 light) : _eye(eye), _center(center), _light(light)
		{
			calculateView();
		}

		void calculateView()
		{
			_view = Mat4::lookAt(_eye, _center);
		}

		Vec3 eye()
		{
			return _eye;
		}

		Vec3 center()
		{
			return _center;
		}

		Vec3 light()
		{
			return _light;
		}

		Mat4 view()
		{
			return _view;
		}

		void setEye(Vec3 eye)
		{
			_eye = eye;
			calculateView();
		}

		void setCenter(Vec3 center)
		{
			_center = center;
			calculateView();
		}

		void rotateAroundCenter(float xRotate, float yRotate)
		{
			Vec3 camera = _eye - _center;
			float r = camera.length();
			float O = acos(camera.y / r);
			float f = atan(camera.z / camera.x);

			if(_center.x > 0)
			{
				yRotate += 180;
			}

			f += qDegreesToRadians(yRotate);
			O += qDegreesToRadians(xRotate);

			camera.x = r * sin(O) * cos(f);
			camera.y = r * cos(O);
			camera.z = r * sin(O) * sin(f);

			_eye = camera + _center;

			calculateView();
		}

};
