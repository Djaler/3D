#include <QtMath>
#include "camera.h"

float Camera::near() const
{
	return _near;
}

float Camera::far() const
{
	return _far;
}

Camera::Camera(Vec3 eye, Vec3 center, float fov, float width, float height, float near, float far) :
	_eye(eye), _center(center), _fov(fov), width(width), height(height), _near(near), _far(far), _rotateAroundCenter(true)
{
	calculateView();
	calculateProjectionViewport();
}

Mat4 Camera::projectionViewport()
{
	return _projectionViewport;
}

bool Camera::isRotateAroundCenter()
{
	return _rotateAroundCenter;
}

void Camera::setRotateAroundCenter(bool rotateAroundCenter)
{
	_rotateAroundCenter = rotateAroundCenter;
}

Vec3 Camera::eye()
{
	return _eye;
}

Vec3 Camera::center()
{
	return _center;
}

Mat4 Camera::view()
{
	return _view;
}

float Camera::fov()
{
	return _fov;
}

void Camera::setFov(float fov)
{
	_fov = fov;
	calculateProjectionViewport();
}

void Camera::setEye(Vec3 eye)
{
	_eye = eye;
	calculateView();
}

void Camera::setCenter(Vec3 center)
{
	_center = center;
	calculateView();
}

void Camera::rotate(float xRotate, float yRotate)
{
	if(_rotateAroundCenter)
	{
		rotateAroundCenter(xRotate, yRotate);
	}
	else
	{
		rotateAroundEye(xRotate, yRotate);
	}
	calculateView();
}

float Camera::xRotate()
{
	if(_rotateAroundCenter)
	{
		return xRotateAroundCenter();
	}
	else
	{
		return xRotateAroundEye();
	}
}

float Camera::yRotate()
{
	if(_rotateAroundCenter)
	{
		return yRotateAroundCenter();
	}
	else
	{
		return yRotateAroundEye();
	}
}

void Camera::rotateAroundCenter(float xRotate, float yRotate)
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
}

float Camera::xRotateAroundCenter()
{
	Vec3 camera = _eye - _center;
	return 90 - qRadiansToDegrees(acos(camera.y / camera.length()));
}

float Camera::yRotateAroundCenter()
{
	Vec3 camera = _eye - _center;
	return 90 + qRadiansToDegrees(atan(camera.z / camera.x));
}

void Camera::rotateAroundEye(float xRotate, float yRotate)
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
}

float Camera::xRotateAroundEye()
{
	Vec3 camera = _center - _eye;
	return qRadiansToDegrees(acos(camera.y / camera.length())) - 90;
}

float Camera::yRotateAroundEye()
{
	Vec3 camera = _center - _eye;
	return qRadiansToDegrees(atan(camera.z / camera.x)) - 90;
}

void Camera::calculateProjectionViewport()
{
	Mat4 projection = Mat4::perspective(_fov, width / height, _near, _far);
	Mat4 viewport = Mat4::viewport(width, height);

	_projectionViewport = viewport * projection;
}

void Camera::calculateView()
{
	_view = Mat4::lookAt(_eye, _center);
}
