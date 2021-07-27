#pragma once
#include <string>

class CVector
{
public:
};


class Vector3 : public CVector {
public:
	Vector3 operator*(float _f);
	Vector3 operator-(Vector3 _v);
	Vector3 operator+(Vector3 _v);
	bool operator==(Vector3 _v);

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	float x = 0;
	float y = 0;
	float z = 0;

	static Vector3 Cross(Vector3 a, Vector3 b) {
		Vector3 c = {0,0,0};

		c.x = a.y * b.z - a.z * b.y;
		c.y = a.z * b.x - a.x * b.z;
		c.z = a.x * b.y - a.y * b.x;

		return c;
	}

	static float Dot(Vector3 a, Vector3 b) {
		float result = 0;

		result += a.x * b.x;
		result += a.y * b.y;
		result += a.z * b.z;

		return result;
	}

	float Mag() {
		return (float)sqrt(double(x) * double(x) + double(y) * double(y) + double(z) * double(z));
	}

	static float Diff(Vector3 a, Vector3 b) {
		return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
	}

	std::string ToString() {
		return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")";
	}
};
