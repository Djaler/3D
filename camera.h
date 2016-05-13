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
			Vec4 eye4 = _eye.toVec4();
			eye4 = Mat4::translate(-_center.x, -_center.y, -_center.z) * eye4;
			eye4 = Mat4::rotate(xRotate, yRotate) * eye4;
			eye4 = Mat4::translate(_center.x, _center.y, _center.z) * eye4;

			_eye = eye4.toVec3();

			calculateView();
		}

};
