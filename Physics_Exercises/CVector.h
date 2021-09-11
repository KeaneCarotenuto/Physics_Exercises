#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>
#include <iomanip>

#include"CPrint.h"


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

	/// <summary>
	/// returns as string
	/// </summary>
	/// <returns>"(x,y,z)"</returns>
	std::string ToString();

	Vector3 Normalized();

	double Mag();

	/// <summary>
	/// World angle
	/// </summary>
	/// <returns></returns>
	double Angle();

	double UnsignedAngle();

	/// <summary>
	/// If any values are zero
	/// </summary>
	/// <returns></returns>
	bool AnyZero();

	/// <summary>
	/// If any values are infinite
	/// </summary>
	/// <returns></returns>
	bool AnyInf();

	static double Angle(Vector3 _a, Vector3 _b);

	/// <summary>
	/// All Zero
	/// </summary>
	/// <returns></returns>
	static Vector3 Zero();

	/// <summary>
	/// All Infinity
	/// </summary>
	/// <returns></returns>
	static Vector3 Infinity();

	static Vector3 Cross(Vector3 a, Vector3 b);

	static double Dot(Vector3 a, Vector3 b);
	static double Det(Vector3 a, Vector3 b);

	/// <summary>
	/// The value difference between two vectors
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	static double Diff(Vector3 a, Vector3 b);

	/// <summary>
	/// Distance between two points
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
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
	/// Returns a point in space where a line intersects a plane
	/// </summary>
	/// <param name="pp"></param>
	/// <param name="pn"></param>
	/// <param name="lpa"></param>
	/// <param name="lpb"></param>
	/// <param name="doesIntersect"></param>
	/// <returns></returns>
	static Vector3 LineIntersectPlanePoint(Vector3 pp, Vector3 pn, Vector3 lpa, Vector3 lpb, bool* doesIntersect = nullptr);

	/// <summary>
	/// Returns true if a triangle intersects a plane
	/// </summary>
	/// <param name="pp"></param>
	/// <param name="pn"></param>
	/// <param name="ta"></param>
	/// <param name="tb"></param>
	/// <param name="tc"></param>
	/// <returns></returns>
	static bool TriangleIntersectsPlane(Vector3 pp, Vector3 pn, Vector3 ta, Vector3 tb, Vector3 tc);

	/// <summary>
	/// Returns then line-line intersection point
	/// </summary>
	/// <param name="l1p1"></param>
	/// <param name="l1p2"></param>
	/// <param name="l2p1"></param>
	/// <param name="l2p2"></param>
	/// <param name="isInfinite"></param>
	/// <returns></returns>
	static Vector3 LineIntersectsLine(Vector3 l1p1, Vector3 l1p2, Vector3 l2p1, Vector3 l2p2, bool isInfinite = false);

	static double FindCannonAngle(double l, double v, Vector3 P, int dp = 5, int maxTries = 1000, bool* passed = nullptr);

	static void Javelin(double h, double s, double a, double t);
};

struct Line {
	Vector3 a = Vector3::Infinity();
	Vector3 b = Vector3::Infinity();

	double Length() {
		return Vector3::Distance(a, b);
	}

	bool IsValid() {
		if (a.AnyInf() || b.AnyInf()) return false;
		else return true;
	}

	/// <summary>
	/// Simple + fast check to see if a point COULD be on the line (Actually checks rect)
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	bool ContainsPoint(double x, double y) {
		if (std::min(a.x, b.x) <= x && x <= std::max(a.x, b.x) &&
			std::min(a.y, b.y) <= y && y <= std::max(a.y, b.y))
			return true;
		else return false;
	}

	/// <summary>
	/// Returns shortest line
	/// </summary>
	/// <param name="_a"></param>
	/// <param name="_b"></param>
	/// <returns></returns>
	static Line Shortest(Line _a, Line _b) {
		return (fmin(_a.Length(), _b.Length()) == _a.Length() ? _a : _b);
	}
};

struct Triangle {
	Vector3 a = Vector3::Infinity();
	Vector3 b = Vector3::Infinity();
	Vector3 c = Vector3::Infinity();

	sf::Color col = sf::Color::White;

	bool IsValid() {
		if (a.AnyInf() || b.AnyInf() || c.AnyInf()) return false;
		else return true;
	}


	bool AnglePoint(Vector3 _p, bool print = false);
	bool BaryPoint(Vector3 _p, bool print = false);
};

struct RegPolygon {
	std::vector<Vector3> points;

	sf::Color col = sf::Color::White;

	bool IsValid() {
		if (points.size() <= 0) return false;

		for (Vector3 _point : points) {
			if (_point.AnyInf()) return false;
		}
		return true;
	}

	static bool intersect(RegPolygon* a, RegPolygon* b, std::vector<Vector3>* points = nullptr);
};

struct Capsule {
	Capsule(Vector3 _a, Vector3 _b, double _rad, sf::Color _col) {
		a = _a;
		b = _b;
		radius = _rad;
		col = _col;
	}

	Vector3 a = Vector3::Zero();
	Vector3 b = Vector3::Zero();
	double radius = 0;

	sf::Color col = sf::Color::White;

	bool IsValid() {
		if (a.AnyInf() || b.AnyInf() || radius < 0.0 || radius == (double)INFINITY) return false;
		else return true;
	}

	/// <summary>
	/// Returns the shortest distance between the two capsules/lines
	/// </summary>
	/// <param name="cap1"></param>
	/// <param name="cap2"></param>
	/// <returns></returns>
	static Line ShortestDistanceBetween(Capsule cap1, Capsule cap2) {

		Vector3 intPoint = Vector3::LineIntersectsLine(cap1.a, cap1.b, cap2.a, cap2.b);

		if (intPoint != Vector3::Infinity()) return { intPoint, intPoint };

		Line lineAA = ShortestPerpLine(cap1, cap2, cap1.a, cap2.a);
		Line lineAB = ShortestPerpLine(cap1, cap2, cap1.a, cap2.b);
		Line lineBA = ShortestPerpLine(cap1, cap2, cap1.b, cap2.a);
		Line lineBB = ShortestPerpLine(cap1, cap2, cap1.b, cap2.b);

		return Line::Shortest( Line::Shortest(lineAA, lineAB), Line::Shortest(lineBA, lineBB));
	}

	/// <summary>
	/// Finds the shortest perpendicular line between two ends of capsules/lines
	/// </summary>
	/// <param name="cap1"></param>
	/// <param name="cap2"></param>
	/// <param name="cap1End"></param>
	/// <param name="cap2End"></param>
	/// <returns></returns>
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