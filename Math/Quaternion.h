#pragma once
#include "MyMath.h"

//	クォータニオン
class Quaternion{
public:
	float w;
	float x;
	float y;
	float z;

	Quaternion();
	Quaternion(float w, float x, float y, float z);
	Quaternion(const Vector3& axis, float theta);

	void Normalize();

	static const Quaternion kIdentity;
};

