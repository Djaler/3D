#pragma once

#include "vector.h"
#include "matrix.h"

class Camera
{
	Vec3 _eye;
	Vec3 _center;
	Mat4 _view;
	Mat4 _projectionViewport;

	float _fov, width, height, _near, _far;

	bool _rotateAroundCenter;
	bool _perspective;

	public:
		Camera(Vec3 eye, Vec3 center, float fov, float width, float height, float near, float far);

		Mat4 projectionViewport();
		bool isRotateAroundCenter();
		void setRotateAroundCenter(bool rotateAroundCenter);
		Vec3 eye();
		Vec3 center();
		Mat4 view();
		float fov();
		void setFov(float fov);
		void setEye(Vec3 eye);
		void setCenter(Vec3 center);
		void rotate(float xRotate, float yRotate);
		float xRotate();
		float yRotate();
		float near() const;
		float far() const;
		void setPerspective(bool perspective);
		void calculateProjectionViewport();

	private:
		void calculateView();
		void rotateAroundCenter(float xRotate, float yRotate);
		float xRotateAroundCenter();
		float yRotateAroundCenter();
		void rotateAroundEye(float xRotate, float yRotate);
		float xRotateAroundEye();
		float yRotateAroundEye();

};
