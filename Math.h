#pragma once
#ifndef HELLO
#define HELLO

#include<glm.hpp>
class Vector3f {
public:
	float x, y, z;

	Vector3f() {
		x = y = z = 0.0f;
	}

	Vector3f(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3f(const Vector3f& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	bool operator==(const Vector3f& v) {
		if (v.x == x && v.y == y && v.z == z)
			return true;
		return false;
	}

	bool operator!=(const Vector3f& v) {
		if (v.x != x || v.y != y || v.z || z)
			return true;
		return false;
	}

	bool isZeroVector() {
		if (x == 0 && y == 0 && z == 0)
			return true;
		return false;
	}
};

class Vector2f {
public:
	float x, y;

	Vector2f() {
		x = y = 0.0f;
	}

	Vector2f(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Vector2f(const Vector2f& v) {
		x = v.x;
		y = v.y;
	}

	bool operator==(const Vector2f& v) {
		if (v.x == x && v.y == y)
			return true;
		return false;
	}

	Vector2f operator*(float m) {
		return Vector2f(x*m, y*m);
	}

	bool isZeroVector() {
		if (x == 0 && y == 0)
			return true;
		return false;
	}
};


#endif // !HELLO