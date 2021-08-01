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
