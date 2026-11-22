#pragma once
#include<math.h>
class Vector2
{
public:
	Vector2() = default;
	~Vector2() = default;
	Vector2(float set_x, float set_y) : x(set_x), y(set_y) {}


	// 为Vector2重载常用运算符
	Vector2 operator+(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	Vector2 operator-(const Vector2& vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	void operator+=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;
	}

	void operator-=(const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;
	}

	// 点积
	float operator*(const Vector2& vec) const
	{
		return vec.x * x + vec.y * y;
	}

	// 数乘
	Vector2 operator*(float val) const
	{
		return Vector2(x * val, y * val);
	}

	void operator*=(float val) {
		x *= val;
		y *= val;
	}

	Vector2 operator/(float val) const
	{
		return Vector2(x / val, y / val);
	}

	void operator/=(float val) {
		x /= val;
		y /= val;
	}

	float length() const
	{
		return sqrt(x * x + y * y);
	}


	// 将向量的模长正则化为1
	Vector2 normalize() const {
		float len = length();

		if (len == 0) {
			return Vector2(0, 0);
		}

		return Vector2(x / len, y / len);
	}


	// 当两个向量差的模足够小, 则认为它们重叠
	bool approx(const Vector2& target) const {
		return (*this - target).length() <= 0.0001f;
	}


	// 坐标位置
	float x;
	float y;
};
