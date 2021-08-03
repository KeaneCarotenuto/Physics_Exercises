#pragma once
#include <SFML/Graphics.hpp>
#include <string>


class CVector
{
public:
};


class Vector3 : public CVector {
public:
	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	Vector3(const sf::Vector2i a) { x = a.x; y = a.y; z = 0; };
	Vector3(const sf::Vector2f a) { x = a.x; y = a.y; z = 0; };

	Vector3 operator*(double _f);
	Vector3 operator-(Vector3 _v);
	Vector3 operator+(Vector3 _v);
	bool operator==(Vector3 _v);
	bool operator!=(Vector3 _v);

	operator sf::Vector2f() const { return { (float)x, (float)y }; };

	double x = 0;
	double y = 0;
	double z = 0;

	std::string ToString();

	Vector3 Normalized();

	double Mag();

	bool AnyZero();

	bool AnyInf();

	static Vector3 Zero();

	static Vector3 Infinity();

	static Vector3 Cross(Vector3 a, Vector3 b);

	static double Dot(Vector3 a, Vector3 b);

	static double Diff(Vector3 a, Vector3 b);

	/// <summary>
	/// Returns true if given point (p) lies on plane (k n)
	/// </summary>
	/// <param name="p">point to test</param>
	/// <param name="k">known point on plane</param>
	/// <param name="n">normal of plane</param>
	/// <returns></returns>
	static bool IsPointOnPlane(Vector3 p, Vector3 k, Vector3 n);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pp">known plane point</param>
	/// <param name="pn">plane normal</param>
	/// <param name="lpa">line point A</param>
	/// <param name="lpb">line point B</param>
	/// <param name="doesIntersect">bool pointer to return result</param>
	/// <returns></returns>
	static Vector3 LineIntersectPlanePoint(Vector3 pp, Vector3 pn, Vector3 lpa, Vector3 lpb, bool* doesIntersect = nullptr);

	static bool TriangleIntersectsPlane(Vector3 pp, Vector3 pn, Vector3 ta, Vector3 tb, Vector3 tc);

	static Vector3 LineIntersectsLine(Vector3 l1p1, Vector3 l1p2, Vector3 l2p1, Vector3 l2p2);

};

struct Line {
	Vector3 a = Vector3::Zero();
	Vector3 b = Vector3::Zero();

	bool IsValid() {
		if (a.AnyInf() || b.AnyInf()) return false;
		else return true;
	}
};

struct Triangle {
	Vector3 a = Vector3::Zero();
	Vector3 b = Vector3::Zero();
	Vector3 c = Vector3::Zero();

	sf::Color col = sf::Color::White;

	bool IsValid() {
		if (a.AnyInf() || b.AnyInf() || c.AnyInf()) return false;
		else return true;
	}
};