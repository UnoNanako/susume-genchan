#include "Math/MyMath.h"

const Vector3 Vector3::kZero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::kOne(1.0f, 1.0f, 1.0f);

static float sIdentity4[4][4] = {
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f
};
const Matrix4x4 Matrix4x4::kIdentity(sIdentity4);