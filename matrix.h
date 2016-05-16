#pragma once

class Mat4
{
	float m[16];

	public:
		Mat4();

		Mat4 operator*(const Mat4 &other) const;
		Vec4 operator*(const Vec4 &v) const;
		Vec3 operator*(const Vec3 &v) const;
		float& operator[](int i);

		static Mat4 identity();
		static Mat4 rotate(float xAngle = 0.0f, float yAngle = 0.0f, float zAngle = 0.0f);
		static Mat4 translate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		static Mat4 scale(float x = 1.0f, float y = 1.0f, float z = 1.0f);
		static Mat4 lookAt(const Vec3& eye, const Vec3& center);
		static Mat4 perspective(float fov, float aspect, float near, float far);
		static Mat4 orthographic(float width, float height, float near, float far);
		static Mat4 viewport(float width, float height);
};
