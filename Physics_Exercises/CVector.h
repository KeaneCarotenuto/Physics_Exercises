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

	static Vector3 LineIntersectsLine(Vector3 l1p1, Vector3 l1p2, Vector3 l2p1, Vector3 l2p2)
	{
		//convert line 1 from vector to equation Ax + By = C
		double l1a = l1p2.y - l1p1.y;
		double l1b = l1p1.x - l1p2.x;
		double l1c = l1a * (l1p1.x) + l1b * (l1p1.y);

		//convert line 2 from vector to equation Ax + By = C
		double l2a = l2p2.y - l2p1.y;
		double l2b = l2p1.x - l2p2.x;
		double l3c = l2a * (l2p1.x) + l2b * (l2p1.y);

		//calc determinant to detect parallel
		double determinant = l1a * l2b - l2a * l1b;

		if (determinant == 0)
		{
			//If parallel return infinity
			return Infinity();
		}
		else
		{
			double x = (l2b * l1c - l1b * l3c) / determinant;
			double y = (l1a * l3c - l2a * l1c) / determinant;

			//check if actually intersecting between points
			if (std::min(l1p1.x, l1p2.x) <= x && x <= std::max(l1p1.x, l1p2.x) &&
				std::min(l1p1.y, l1p2.y) <= y && y <= std::max(l1p1.y, l1p2.y) &&
				std::min(l2p1.x, l2p2.x) <= x && x <= std::max(l2p1.x, l2p2.x) &&
				std::min(l2p1.y, l2p2.y) <= y && y <= std::max(l2p1.y, l2p2.y))
			{
				return { x, y, 0 };
			}
			else {
				//If not intersecting, return infinity
				return { INFINITY, INFINITY , INFINITY };
			}

			
		}
	}

};