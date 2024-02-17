#pragma once
#include "MyMath.h"
struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};