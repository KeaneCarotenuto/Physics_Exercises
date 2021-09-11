#include "CVector.h"

Vector3 Vector3::operator*(double _f)
{
	return Vector3(x * _f, y * _f, z * _f);
}

Vector3 Vector3::operator-(Vector3 _v)
{
	return Vector3(x - _v.x, y - _v.y, z - _v.z);
}

Vector3 Vector3::operator+(Vector3 _v)
{
	return Vector3(x + _v.x, y + _v.y, z + _v.z);
}

bool Vector3::operator==(Vector3 _v)
{
	if (x == _v.x && y == _v.y && z == _v.z) return true;
	else return false;
}

bool Vector3::operator!=(Vector3 _v)
{
	return !(_v == (*this));
}

std::string Vector3::ToString() {
	return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")";
}

double Vector3::Mag() {
	return (double)abs(sqrt(double(x) * double(x) + double(y) * double(y) + double(z) * double(z)));
}

double Vector3::Angle()
{
	return atan2(y, x) / (M_PI) * 180;
}

double Vector3::Angle(Vector3 _a, Vector3 _b)
{
	double dot = _a.x * _b.x + _a.y * _b.y;
	double det = _a.x * _b.y - _a.y * _b.x;
	double angle = atan2(det, dot) / (M_PI) * 180;

	return angle;
}

double Vector3::UnsignedAngle()
{
	double angle = Angle();
	if (angle < 0) angle = 360 + angle;
	return angle;
}

Vector3 Vector3::Normalized() {
	double mag = this->Mag();
	return Vector3(x / mag, y / mag, z / mag);
}

bool Vector3::AnyZero() {
	if (x == 0 || y == 0 || z == 0) return true;
	else return false;
}

bool Vector3::AnyInf() {
	if (x == INFINITY || y == INFINITY || z == INFINITY) return true;
	else return false;
}

Vector3 Vector3::Zero() {
	return { 0,0,0 };
}

Vector3 Vector3::Infinity() {
	return { INFINITY,INFINITY,INFINITY };
}

Vector3 Vector3::Cross(Vector3 a, Vector3 b) {
	Vector3 c = { 0,0,0 };

	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;

	return c;
}

double Vector3::Dot(Vector3 a, Vector3 b) {
	double result = 0;

	result += a.x * b.x;
	result += a.y * b.y;
	result += a.z * b.z;

	return result;
}

double Vector3::Det(Vector3 a, Vector3 b) {
	
	double result = a.x * b.y - a.y * b.x;

	return result;
}

double Vector3::Diff(Vector3 a, Vector3 b) {
	return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
}

double Vector3::Distance(Vector3 a, Vector3 b)
{
	return (a - b).Mag();
}

bool Vector3::IsPointOnPlane(Vector3 p, Vector3 k, Vector3 n) {
	double dot = Vector3::Dot(p - k, n);

	return (dot == 0.0);
}

Vector3 Vector3::LineIntersectPlanePoint(Vector3 pp, Vector3 pn, Vector3 lpa, Vector3 lpb, bool* doesIntersect) {
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

bool Vector3::TriangleIntersectsPlane(Vector3 pp, Vector3 pn, Vector3 ta, Vector3 tb, Vector3 tc) {
	bool doesInt = false;
	LineIntersectPlanePoint(pp, pn, ta, tb, &doesInt);
	if (doesInt) return doesInt;
	LineIntersectPlanePoint(pp, pn, tb, tc, &doesInt);
	if (doesInt) return doesInt;
	LineIntersectPlanePoint(pp, pn, tc, ta, &doesInt);
	if (doesInt) return doesInt;

	return false;
}

Vector3 Vector3::LineIntersectsLine(Vector3 l1p1, Vector3 l1p2, Vector3 l2p1, Vector3 l2p2, bool isInfinite)
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
		if (isInfinite || 
			(
			std::min(l1p1.x, l1p2.x) <= x && x <= std::max(l1p1.x, l1p2.x) &&
			std::min(l1p1.y, l1p2.y) <= y && y <= std::max(l1p1.y, l1p2.y) &&
			std::min(l2p1.x, l2p2.x) <= x && x <= std::max(l2p1.x, l2p2.x) &&
			std::min(l2p1.y, l2p2.y) <= y && y <= std::max(l2p1.y, l2p2.y)
			)
		)
		{
			return { x, y, 0 };
		}
		else {
			//If not intersecting, return infinity
			return { INFINITY, INFINITY , INFINITY };
		}
	}
}

double Vector3::FindCannonAngle(double l, double v, Vector3 P, int dp, int maxTries, bool* passed)
{
	double g = -9.81;
	double theta = 0;
	double result = 0;

	bool up = true;
	double inc = 1;

	int count = 0;

	while (true) {
		count++;
		double t = (P.x - l * cos(theta))/(v*cos(theta));
		result = (l + t * v) * sin(theta) - ((g * t * t) / (2.0));

		if (count >= maxTries) {
			if (passed) *passed = false;
			return theta;
		}
		if (abs(result - P.y) <= 1.0 / pow(10, dp + 2)) {
			if (passed) *passed = true;
			return theta;
		}
		else if (result < P.y) {
			if (!up) {
				up = true;
				inc *= 0.5;
			}
			theta += inc * (M_PI) / 180;
		}
		else if (result > P.y) {
			if (up) {
				up = false;
				inc *= 0.5;
			}
			theta -= inc * (M_PI) / 180;
		}
	}
}

bool Triangle::AnglePoint(Vector3 _p, bool print)
{
	float angleA = Vector3::Angle(a - _p, b - _p);
	float angleB = Vector3::Angle(b - _p, c - _p);
	float angleC = Vector3::Angle(c - _p, a - _p);

	if (print) {
		cprint::Print({ 5,5 }, L"A: " + std::to_wstring(angleA));
		cprint::Print({ 5,6 }, L"B: " + std::to_wstring(angleB));
		cprint::Print({ 5,7 }, L"C: " + std::to_wstring(angleC));
	}
	
	return 360.0 - abs((float)(angleA + angleB + angleC)) <= 0.01;
}

bool Triangle::BaryPoint(Vector3 _p, bool print)
{
	double s1 = c.y - a.y;
	double s2 = c.x - a.x;
	double s3 = b.y - a.y;
	double s4 = _p.y - a.y;

	double w = (a.x * s1 + s4 * s2 - _p.x * s1) / (s3 * s2 - (b.x - a.x) * s1);
	double v = (s4 - w * s3) / s1;
	double u = 1.0f - w - v;

	if (print) {
		cprint::Print({ 5,5 }, L"A: " + std::to_wstring(1.0f - w - v));
		cprint::Print({ 5,6 }, L"B: " + std::to_wstring(w));
		cprint::Print({ 5,7 }, L"C: " + std::to_wstring(v));
	}

	return w >= 0 && v >= 0 && (w + v) <= 1;
}


bool RegPolygon::intersect(RegPolygon* a, RegPolygon* b, std::vector<Vector3>* points)
{
	// loop over the vertices(-> edges -> axis) of the first polygon
	for (int i = 0; i < a->points.size() + 0; ++i) {
		// calculate the normal vector of the current edge
		// this is the axis will we check in this loop
		Vector3 current = a->points[i];
		Vector3 next = a->points[(i + 1) % a->points.size()];
		Vector3 edge = next - current;

		Vector3 axis = Vector3::Zero();
		axis.x = -edge.y;
		axis.y = edge.x;

		// loop over all vertices of both polygons and project them
		// onto the axis. We are only interested in max/min projections
		double aMaxProj = -std::numeric_limits<double>::infinity();
		double aMinProj = std::numeric_limits<double>::infinity();
		double bMaxProj = -std::numeric_limits<double>::infinity();
		double bMinProj = std::numeric_limits<double>::infinity();
		for (Vector3 v : a->points) {
			double proj = Vector3::Dot(axis, v);
			if (proj < aMinProj) aMinProj = proj;
			if (proj > aMaxProj) aMaxProj = proj;
		}

		for (Vector3 v : b->points) {
			double proj = Vector3::Dot(axis, v);
			if (proj < bMinProj) bMinProj = proj;
			if (proj > bMaxProj) bMaxProj = proj;
		}

		if (points) {
			points->push_back(current);
			points->push_back(next);
			points->push_back(edge);
			points->push_back(axis);
		}

		// now check if the intervals the both polygons projected on the
		// axis overlap. If they don't, we have found an axis of separation and
		// the given polygons cannot overlap
		if (aMaxProj < bMinProj || aMinProj > bMaxProj) {
			return true;
		}
	}

	// at this point, we have checked all axis but found no separating axis
	// which means that the polygons must intersect.
	return false;
}