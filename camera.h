#pragma once

#include "vector.h"
#include "matrix.h"

class Camera
{
	Vec3 _eye;
	Vec3 _center;
	Mat4 _view;
	Mat4 _projectionViewport;

	bool _rotateAroundCenter;

	public:
		Camera(Vec3 eye, Vec3 center, float fov, float width, float height, float near, float far) :
			_eye(eye), _center(center), _rotateAroundCenter(true)
		{
			calculateView();
			calculateProjectionViewport(fov, width, height, near, far);
		}

		Mat4 projectionViewport()
		{
			return _projectionViewport;
		}

		bool isRotateAroundCenter()
		{
			return _rotateAroundCenter;
		}

		void setRotateAroundCenter(bool rotateAroundCenter)
		{
			_rotateAroundCenter = rotateAroundCenter;
		}

		Vec3 eye()
		{
			return _eye;
		}

		Vec3 center()
		{
			return _center;
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

		void rotate(float xRotate, float yRotate)
		{
			if(_rotateAroundCenter)
			{
				rotateAroundCenter(xRotate, yRotate);
			}
			else
			{
				rotateAroundEye(xRotate, yRotate);
			}
		}

		void rotateAroundCenter(float xRotate, float yRotate)
		{
			Vec3 camera = _eye - _center;
			float r = camera.length();
			float O = acos(camera.y / r);
			float f = atan(camera.z / camera.x);

			f = qDegreesToRadians(yRotate + 90);
			O = qDegreesToRadians(xRotate - 90);

			camera.x = r * sin(O) * cos(f);
			camera.y = r * cos(O);
			camera.z = r * sin(O) * sin(f);

			_eye = camera + _center;
			calculateView();
		}

		float xRotateArountCenter()
		{
			Vec3 camera = _eye - _center;
			return qRadiansToDegrees(acos(camera.y / camera.length()));
		}

		void rotateAroundEye(float xRotate, float yRotate)
		{
			Vec3 camera = _center - _eye;
			float r = camera.length();
			float O = acos(camera.y / r);
			float f = atan(camera.z / camera.x);

			f = qDegreesToRadians(yRotate + 90);
			O = qDegreesToRadians(xRotate + 90);

			camera.x = r * sin(O) * cos(f);
			camera.y = r * cos(O);
			camera.z = r * sin(O) * sin(f);

			_center = camera + _eye;
			calculateView();
		}

		float xRotateArountEye()
		{
			Vec3 camera = _center - _eye;
			return qRadiansToDegrees(acos(camera.y / camera.length()));
		}

	private:
		void calculateProjectionViewport(float fov, float width, float height, float near, float far)
		{
			Mat4 projection = Mat4::perspective(fov, width / height, near, far);
			Mat4 viewport = Mat4::viewport(width, height);

			_projectionViewport = viewport * projection;
		}

		void calculateView()
		{
			_view = Mat4::lookAt(_eye, _center);
		}

};
