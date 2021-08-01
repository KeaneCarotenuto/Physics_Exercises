#pragma once
#include <SFML/Graphics.hpp>
#include <string>


class CVector
{
public:
};


class Vector3 : public CVector {
public:
	Vector3 operator*(double _f);
	Vector3 operator-(Vector3 _v);
	Vector3 operator+(Vector3 _v);
	bool operator==(Vector3 _v);
	bool operator!=(Vector3 _v);

	Vector3(const sf::Vector2i a) { x = a.x; y = a.y; z = 0; };
	Vector3(const sf::Vector2f a) { x = a.x; y = a.y; z = 0; };

	operator sf::Vector2f() const { return { (float)x, (float)y }; };

	std::string ToString() {
		return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")";
	}

	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

	double x = 0;
	double y = 0;
	double z = 0;

	double Mag() {
		return (double)sqrt(double(x) * double(x) + double(y) * double(y) + double(z) * double(z));
	}

	Vector3 Normalized() {
		double mag = this->Mag();
		return Vector3(x / mag, y / mag, z / mag);
	}

	bool AnyZero() {
		if (x == 0 || y == 0 || z == 0) return true;
		else return false;
	}

	bool AnyInf() {
		if (x == INFINITY || y == INFINITY || z == INFINITY) return true;
		else return false;
	}

	static Vector3 Zero() {
		return { 0,0,0 };
	}

	static Vector3 Infinity() {
		return { INFINITY,INFINITY,INFINITY };
	}

	static Vector3 Cross(Vector3 a, Vector3 b) {
		Vector3 c = {0,0,0};

		c.x = a.y * b.z - a.z * b.y;
		c.y = a.z * b.x - a.x * b.z;
		c.z = a.x * b.y - a.y * b.x;

		return c;
	}

	static double Dot(Vector3 a, Vector3 b) {
		double result = 0;

		result += a.x * b.x;
		result += a.y * b.y;
		result += a.z * b.z;

		return result;
	}

	static double Diff(Vector3 a, Vector3 b) {
		return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
	}

	/// <summary>
	/// Returns true if given point (p) lies on plane (k n)
	/// </summary>
	/// <param name="p">point to test</param>
	/// <param name="k">known point on plane</param>
	/// <param name="n">normal of plane</param>
	/// <returns></returns>
	static bool IsPointOnPlane(Vector3 p, Vector3 k, Vector3 n) {
		double dot = Vector3::Dot(p - k, n);

		return (dot == 0.0);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pp">known plane point</param>
	/// <param name="pn">plane normal</param>
	/// <param name="lpa">line point A</param>
	/// <param name="lpb">line point B</param>
	/// <param name="doesIntersect">bool pointer to return result</param>
	/// <returns></returns>
	static Vector3 LineIntersectPlanePoint(Vector3 pp, Vector3 pn, Vector3 lpa, Vector3 lpb, bool* doesIntersect = nullptr) {
		Vector3 lineDirection = (lpb - lpa).Normalized();

		if (Dot(lineDirection, pn) == 0) {
			if (doesIntersect) (*doesIntersect) = false;
			return Vector3(INFINITY, INFINITY, INFINITY);
		}
		else {
			double distance = Dot(pp - lpa, pn) / Dot(lineDirection, pn);
			double lineDistance = (lpb - lpa).Mag();

			if (distance <= lineDistance) {
				if (doesIntersect) (*doesIntersect) = true;
				Vector3 intersectPoint = lpa + (lineDirection * distance);
				return intersectPoint;
			}
			else {
				if (doesIntersect) (*doesIntersect) = false;
				return Vector3(INFINITY, INFINITY, INFINITY);
			}
		}
	}

	static bool TriangleIntersectsPlane(Vector3 pp, Vector3 pn, Vector3 ta, Vector3 tb, Vector3 tc) {
		bool doesInt = false;
		LineIntersectPlanePoint(pp, pn, ta, tb, &doesInt);
		if (doesInt) return doesInt;
		LineIntersectPlanePoint(pp, pn, tb, tc, &doesInt);
		if (doesInt) return doesInt;
		LineIntersectPlanePoint(pp, pn, tc, ta, &doesInt);
		if (doesInt) return doesInt;

		return false;
	}

	//LITERALLY CHANGE THIS LMAO
	static Vector3 LineIntersectsLine(Vector3 A, Vector3 B, Vector3 C, Vector3 D)
	{
		// Line AB represented as a1x + b1y = c1
		double a1 = B.y - A.y;
		double b1 = A.x - B.x;
		double c1 = a1 * (A.x) + b1 * (A.y);

		// Line CD represented as a2x + b2y = c2
		double a2 = D.y - C.y;
		double b2 = C.x - D.x;
		double c2 = a2 * (C.x) + b2 * (C.y);

		double determinant = a1 * b2 - a2 * b1;

		if (determinant == 0)
		{
			// The lines are parallel. This is simplified
			// by returning a pair of FLT_MAX
			return { INFINITY, INFINITY , INFINITY };
		}
		else
		{
			double x = (b2 * c1 - b1 * c2) / determinant;
			double y = (a1 * c2 - a2 * c1) / determinant;

			if (std::min(A.x, B.x) <= x && x <= std::max(A.x, B.x) &&
				std::min(A.y, B.y) <= y && y <= std::max(A.y, B.y)) {
				return { x, y, 0 };
			}
			else {
				return { INFINITY, INFINITY , INFINITY };
			}

			
		}
	}

};