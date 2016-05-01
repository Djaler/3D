#ifndef VECTOR_H
#define VECTOR_H

struct Vec3
{
	float x, y, z;

	Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

	Vec3 &operator+=(const Vec3 &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3 &operator*=(const float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	Vec3 operator +(const Vec3 &other) const
	{
		return Vec3(x + other.x, y + other.y, z + other.z);
	}

	Vec3 operator -(const Vec3 &other) const
	{
		return Vec3(x - other.x, y - other.y, z - other.z);
	}

	Vec3 operator *(const Vec3 &other) const
	{
		return Vec3(x * other.x, y * other.y, z * other.z);
	}

	Vec3 operator ^(const Vec3 &other) const
	{
		return Vec3(y * other.z - z * other.y,
					z * other.x - x * other.z,
					x * other.y - y * other.x);
	}

	float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 normalize() const
	{
		float l = 1.0f / length();
		return Vec3(x * l, y * l, z * l);
	}

	float operator[](int i) const
	{
		switch(i)
		{
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
		}
	}

	float& operator[](int i)
	{
		switch(i)
		{
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
		}
	}
};

struct Vec4
{
		float x, y, z, w;

		Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w) {}

		Vec4 &operator+=(const Vec4 &other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		Vec4 &operator*=(const float f)
		{
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}

		Vec4 operator +(const Vec4 &other) const
		{
			return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		Vec4 operator -(const Vec4 &other) const
		{
			return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		Vec4 operator *(const Vec4 &other) const
		{
			return Vec4(x * other.x, y * other.y, z * other.z, w * other.w);
		}

		Vec4 operator ^(const Vec4 &other) const
		{
			return Vec4(y * other.z - z * other.y,
						z * other.x - x * other.z,
						x * other.y - y * other.x,
						0.0f);
		}

		float length() const
		{
			return std::sqrt(x * x + y * y + z * z + w * w);
		}

		Vec4 normalize() const
		{
			float l = 1.0f / length();
			return Vec4(x * l, y * l, z * l, w * l);
		}

		float operator[](int i) const
		{
			switch(i)
			{
				case 0:
					return x;
					break;
				case 1:
					return y;
					break;
				case 2:
					return z;
					break;
				case 3:
					return w;
					break;
			}
		}

		float& operator[](int i)
		{
			switch(i)
			{
				case 0:
					return x;
					break;
				case 1:
					return y;
					break;
				case 2:
					return z;
					break;
				case 3:
					return w;
					break;
			}
		}
};

#endif // VECTOR_H
