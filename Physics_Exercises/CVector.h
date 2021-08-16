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

	static Line ShortestLine(Capsule cap1, Capsule cap2) {
		

		double distAA = Vector3::Distance(cap1.a, cap2.a);
		double distAB = Vector3::Distance(cap1.a, cap2.b);
		double distBA = Vector3::Distance(cap1.b, cap2.a);
		double distBB = Vector3::Distance(cap1.b, cap2.b);

		double shortest = fmin(fmin(distAA, distAB), fmin(distBA, distBB));

		Vector3 intPoint = Vector3::LineIntersectsLine( cap1.a, cap1.b, cap2.a, cap2.b, true);
		
		Vector3 a = Vector3::Infinity();
		Vector3 b = Vector3::Infinity();

		if (shortest == distAA) {
			//return { cap1.a, cap2.a };
			a = intPoint - cap1.a;
			b = intPoint - cap2.a;
		}
		else if (shortest == distAB) {
			//return { cap1.a, cap2.b };
			a = intPoint - cap1.a;
			b = intPoint - cap2.b;
		}
		else if(shortest == distBA) {
			//return { cap1.b, cap2.a };
			a = intPoint - cap1.b;
			b = intPoint - cap2.a;
		}
		else if(shortest == distBB) {
			//return { cap1.b, cap2.b };
			a = intPoint - cap1.b;
			b = intPoint - cap2.b;
		}
		else {
			return { Vector3::Infinity(), Vector3::Infinity() };
		}

		double angle = acos((Vector3::Dot(a, b)) * (1 / (a.Mag() * b.Mag()))) ;

		double backDistA = cos(angle) * a.Mag();
		Vector3 closestPointA = intPoint - (b.Normalized() * backDistA);
		Line returnLineA = { closestPointA, intPoint - a };

		double backDistB = cos(angle) * b.Mag();
		Vector3 closestPointB = intPoint - (a.Normalized() * backDistB);
		Line returnLineB = { closestPointB, intPoint - b };

		//if (!Line{ cap1.a, cap1.b }.ContainsPoint(closestPointA.x, closestPointA.y) && !Line{ cap2.a, cap2.b }.ContainsPoint(closestPointB.x, closestPointB.y)) return { Vector3::Infinity(), Vector3::Infinity() };

		return (fmin(returnLineA.Length(), returnLineB.Length()) == returnLineA.Length() ? returnLineA : returnLineB);

		//return toReturn;
		
	}

	//Distance check here http://paulbourke.net/geometry/pointlineplane/
};