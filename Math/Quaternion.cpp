#include "Quaternion.h"

const Quaternion Quaternion::kIdentity(1.0f, 0.0f, 0.0f, 0.0f);

Quaternion::Quaternion(){
	*this = kIdentity;
}

Quaternion::Quaternion(float w, float x, float y, float z)
	:w(w)
	,x(x)
	,y(y)
	,z(z)
{}

Quaternion::Quaternion(const Vector3& axis, float theta){
	assert(fabs(Length(axis) - 1.0f) <= 0.001f);
	float thetaOver2 = theta * 0.5f;
	float sin = sinf(thetaOver2);
	w = cosf(thetaOver2);
	x = axis.x * sin;
	y = axis.y * sin;
	z = axis.z * sin;
}

void Quaternion::Normalize(){
	float a = w * w + x * x + y * y + z * z;
	if (a > kEpsilon) {

	}
}
