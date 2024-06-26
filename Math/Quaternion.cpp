#include "Quaternion.h"
#include "Math/MyMath.h"

const Quaternion Quaternion::kIdentity(1.0f, 0.0f, 0.0f, 0.0f);

Quaternion::Quaternion() {
	*this = kIdentity;
}

Quaternion::Quaternion(float w, float x, float y, float z)
	:w(w)
	, x(x)
	, y(y)
	, z(z)
{}

Quaternion::Quaternion(const Vector3& axis, float theta) {
	assert(fabs(Length(axis) - 1.0f) <= 0.001f);
	float thetaOver2 = theta * 0.5f;
	float sin = sinf(thetaOver2);
	w = cosf(thetaOver2);
	x = axis.x * sin;
	y = axis.y * sin;
	z = axis.z * sin;
}

void Quaternion::Normalize() {
	float a = w * w + x * x + y * y + z * z;
	if (a > kEpsilon) {

	}
}

Quaternion Conjugate(const Quaternion& q) {
	Quaternion ret;
	ret.w = q.w;
	ret.x = -q.x;
	ret.y = -q.y;
	ret.z = -q.z;
	return ret;
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
	Quaternion ret;
	ret.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	ret.x = q1.w * q2.x + q1.x * q2.w + q1.z * q2.y - q1.y * q2.z;
	ret.y = q1.w * q2.y + q1.y * q2.w * q1.x * q2.z - q1.z * q2.x;
	ret.z = q1.w * q2.z + q1.z * q2.w + q1.y * q2.x - q1.x * q2.y;
	return ret;
}

Vector3 operator*(const Vector3& v, const Quaternion& q) {
	Quaternion ret;
	ret = Conjugate(q) * Quaternion(0.0f, v.x, v.y, v.z) * q;
	return Vector3(ret.x, ret.y, ret.z);
}

Quaternion operator*=(Quaternion& q1, const Quaternion& q2) {
	q1 = q1 * q2;
	return q1;
}

Vector3 operator*=(Vector3& v, const Quaternion& q) {
	v = v * q;
	return v;
}

float Dot(const Quaternion& q1, const Quaternion& q2) {
	return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float mInT) {
	Quaternion ret;
	ret.w = Lerp(q1.w, q2.w, mInT);
	ret.x = Lerp(q1.x, q2.x, mInT);
	ret.y = Lerp(q1.y, q2.y, mInT);
	ret.z = Lerp(q1.z, q2.z, mInT);
	ret.Normalize();
	return ret;
}

Quaternion Pow(const Quaternion& q, float e) {
	if (fabs(q.w) > 0.999f) {
		return q;
	}
	float mAlpha = acosf(q.w);
	float newAlpha = mAlpha * e;
	Quaternion ret;
	ret.w = cosf(newAlpha);
	float a = sinf(newAlpha) / sinf(mAlpha);
	ret.x = q.x * a;
	ret.y = q.y * a;
	ret.z = q.z * a;
	return ret;
}

Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float mInT) {
	if (mInT <= 0.0f) {
		return q1;
	}
	if (mInT >= 1.0f) {
		return q2;
	}
	float cosOmega = Dot(q1, q2);
	float w = q2.w;
	float x = q2.x;
	float y = q2.y;
	float z = q2.z;
	if (cosOmega < 0.0f) {
		w = -q2.w;
		x = -q2.x;
		y = -q2.y;
		z = -q2.z;
		cosOmega = -cosOmega;
	}
	float k0, k1;
	if (cosOmega > 0.999f) {
		k0 = 1.0f - mInT;
		k1 = mInT;
	} else {
		float sinOmega = sqrtf(1.0f - cosOmega * cosOmega);
		float omega = atan2f(sinOmega, cosOmega);
		float oneOverSin = 1.0f / sinOmega;
		k0 = sinf((1.0f - mInT) * omega) * oneOverSin;
		k1 = sinf(mInT * omega) * oneOverSin;
	}
	Quaternion ret;
	ret.w = k0 * q1.w + k1 * w;
	ret.x = k0 * q1.x + k1 * x;
	ret.y = k0 * q1.y + k1 * y;
	ret.z = k0 * q1.z + k1 * z;
	return ret;
}

float GetTheta(const Quaternion& q) {
	return acosf(q.w) * 2.0f;
}

Vector3 GetAxis(const Quaternion& q){
	float sinSq = 1.0f - q.w * q.w;
	if (sinSq <= 0.0f) {
		return Vector3(1.0f, 0.0f, 0.0f);
	}
	float oneOverSin = 1.0f / sqrtf(sinSq);
	return Vector3(q.x * oneOverSin, q.y * oneOverSin, q.z * oneOverSin);
}

Vector3 ToEuler(const Quaternion& q){
	Vector3 euler;
	float sxcy = 2.0f * (q.w * q.x + q.y * q.z);
	float cxcy = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	euler.x = atan2f(sxcy, cxcy);
	float sy = 2.0f * (q.w * q.y - q.z * q.x);
	if (fabsf(sy) >= 1.0f) {
		euler.y = kPiOver2;
		if (sy < 0.0f) {
			euler.y = -euler.y;
		}
	} else {
		euler.y = asinf(sy);
	}
	float szcy = 2.0f * (q.w * q.z + q.x * q.y);
	float czcy = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	euler.z = atan2f(szcy, czcy);
	return euler;
}

Vector3 ToEuler(const Matrix4x4& m){
	Vector3 euler;
	euler.x = atan2f(m.m[2][1], m.m[2][2]);
	float sy = -m.m[2][0];
	float cy = sqrtf(m.m[0][0] * m.m[0][0] + m.m[1][0] * m.m[1][0]);
	euler.y = atan2f(sy, cy);
	float sx = sinf(euler.x);
	float cx = cosf(euler.x);
	euler.z = atan2f(m.m[1][0] * cx - m.m[0][0] * sx, m.m[0][1] * cx - m.m[1][1] * sx);
	return euler;
}

Quaternion ToQuaternion(const Vector3& e){
	float cx = cosf(e.x * 0.5f);
	float sx = sinf(e.x * 0.5f);
	float cy = cosf(e.y * 0.5f);
	float sy = sinf(e.y * 0.5f);
	float cz = cosf(e.z * 0.5f);
	float sz = sinf(e.z * 0.5f);
	Quaternion ret;
	ret.w = cz * cy * cx + sz * sy * sx;
	ret.x = cz * cy * sx - sz * sy * cx;
	ret.y = sz * cy * sx + cz * sy * cx;
	ret.z = sz * cy * cx - cz * sy * sx;
	ret.Normalize();
	return ret;
}

Quaternion ToQuaternion(const Matrix4x4& m){
	auto mInT = Transpose(m);
	auto a = mInT.m;
	Quaternion ret;
	float trace = a[0][0] + a[1][1] + a[2][2];
	if (trace > 0) {
		float s = 0.5f / sqrtf(trace + 1.0f);
		ret.w = 0.25f / s;
		ret.x = (a[2][1] - a[1][2]) * s;
		ret.y = (a[0][2] - a[2][0]) * s;
		ret.z = (a[1][0] - a[0][1]) * s;
	} else {
		if (a[0][0] > a[1][1] && a[0][0] > a[2][2]) {
			float s = 2.0f * sqrtf(1.0f + a[0][0] - a[1][1] - a[2][2]);
			ret.w = (a[2][1] - a[1][2]) / s;
			ret.x = 0.25f * s;
			ret.y = (a[0][1] + a[1][0]) / s;
			ret.z = (a[0][2] + a[2][0]) / s;
		} else if (a[1][1] > a[2][2]) {
			float s = 2.0f * sqrtf(1.0f + a[1][1] - a[0][0] - a[2][2]);
			ret.w = (a[0][2] - a[2][0]) / s;
			ret.x = (a[0][1] + a[1][0]) / s;
			ret.y = 0.25f * s;
			ret.z = (a[1][2] + a[2][1]) / s;
		} else {
			float s = 2.0f * sqrtf(1.0f + a[2][2] - a[0][0] - a[1][1]);
			ret.w = (a[1][0] - a[0][1]) / s;
			ret.x = (a[0][2] + a[2][0]) / s;
			ret.y = (a[1][2] + a[2][1]) / s;
			ret.z = 0.25f * s;
		}
	}
	return ret;
}
