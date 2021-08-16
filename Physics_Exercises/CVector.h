#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>


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

	double Angle();

	bool AnyZero();

	bool AnyInf();

	static Vector3 Zero();

	static Vector3 Infinity();

	static Vector3 Cross(Vector3 a, Vector3 b);

	static double Dot(Vector3 a, Vector3 b);

	static double Diff(Vector3 a, Vector3 b);

	static double Distance(Vector3 a, Vector3 b);

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

	static Vector3 LineIntersectsLine(Vector3 l1p1, Vector3 l1p2, Vector3 l2p1, Vector3 l2p2, bool isInfinite = false);

};

struct Line {
	Vector3 a = Vector3::Zero();
	Vector3 b = Vector3::Zero();

	double Length() {
		return Vector3::Distance(a, b);
	}

	bool IsValid() {
		if (a.AnyInf() || b.AnyInf()) return false;
		else return true;
	}

	bool ContainsPoint(double x, double y) {
		if (std::min(a.x, b.x) <= x && x <= std::max(a.x, b.x) &&
			std::min(a.y, b.y) <= y && y <= std::max(a.y, b.y))
			return true;
		else return false;
	}

	static Line Shortest(Line _a, Line _b) {
		return (fmin(_a.Length(), _b.Length()) == _a.Length() ? _a : _b);
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

struct Capsule {
	Capsule(Vector3 _a, Vector3 _b, double _rad) {
		a = _a;
		b = _b;
		radius = _rad;
	}

	Vector3 a = Vector3::Zero();
	Vector3 b = Vector3::Zero();
	double radius = 0;

	sf::Color col = sf::Color::White;

	bool IsValid() {
		if (a.AnyInf() || b.AnyInf() || radius < 0.0 || radius == (double)INFINITY) return false;
		else return true;
	}

	static Line ShortestDistanceBetween(Capsule cap1, Capsule cap2) {

		Line lineAA = ShortestPerpLine(cap1, cap2, cap1.a, cap2.a);
		Line lineAB = ShortestPerpLine(cap1, cap2, cap1.a, cap2.b);
		Line lineBA = ShortestPerpLine(cap1, cap2, cap1.b, cap2.a);
		Line lineBB = ShortestPerpLine(cap1, cap2, cap1.b, cap2.b);

		return Line::Shortest( Line::Shortest(lineAA, lineAB), Line::Shortest(lineBA, lineBB));
	}

	static Line ShortestPerpLine(Capsule cap1, Capsule cap2, Vector3 cap1End, Vector3 cap2End)
	{
		Vector3 intPoint = Vector3::LineIntersectsLine(cap1.a, cap1.b, cap2.a, cap2.b, true);

		Vector3 cap1Dir = intPoint - cap1End;
		Vector3 cap2Dir = intPoint - cap2End;

		double angle = acos((Vector3::Dot(cap1Dir, cap2Dir)) * (1 / (cap1Dir.Mag() * cap2Dir.Mag())));

		double backDistOnCap2 = cos(angle) * cap1Dir.Mag();
		Vector3 closestPointOnCap2 = intPoint - (cap2Dir.Normalized() * backDistOnCap2);
		Line returnLineA = { closestPointOnCap2, intPoint - cap1Dir };
		if (!Line{ cap2.a, cap2.b }.ContainsPoint(closestPointOnCap2.x, closestPointOnCap2.y)) returnLineA = { Vector3::Infinity(), Vector3::Infinity() };

		double backDistOnCap1 = cos(angle) * cap2Dir.Mag();
		Vector3 closestPointOnCap1 = intPoint - (cap1Dir.Normalized() * backDistOnCap1);
		Line returnLineB = { closestPointOnCap1, intPoint - cap2Dir };
		if (!Line{ cap1.a, cap1.b }.ContainsPoint(closestPointOnCap1.x, closestPointOnCap1.y)) returnLineB = { Vector3::Infinity(), Vector3::Infinity() };

		Line tipToTip = { cap1End , cap2End };

		return  Line::Shortest(Line::Shortest(returnLineA, returnLineB), tipToTip);
	}
};