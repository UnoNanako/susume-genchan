#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <numbers>
#include <limits>
#include "Math/Quaternion.h"

template <class T>
inline T Max(const T& a, const T& b) {
	return a > b ? a : b;
}
template <class T>
inline T Min(const T& a, const T& b) {
	return a < b ? a : b;
}
template <typename T>
inline T Lerp(const T& a, const T& b, float t) {
	return (1.0f - t) * a + t * b;
}

const float kPi = std::numbers::pi_v<float>;
const float kPiOver2 = kPi / 2.0f; //π/2
const float kInfinity = std::numeric_limits<float>::infinity(); //無限
const float kEpsilon = std::numeric_limits<float>::epsilon();

/// <summary>
/// Vector2
/// </summary>
struct Vector2 {
	float x, y;
	Vector2() {
		x = 0.0f;
		y = 0.0f;
	}
	Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	void Normalize() {
		float length = sqrtf((x * x) + (y * y));
		x /= length;
		y /= length;
	}
	//オーバーロード
	Vector2 operator+(Vector2 v) {
		Vector2 ret;
		ret.x = x + v.x;
		ret.y = y + v.y;
		return ret;
	}
	Vector2 operator-(Vector2 v) {
		Vector2 ret;
		ret.x = x - v.x;
		ret.y = y - v.y;
		return ret;
	}
	Vector2 operator*(float scaler) {
		Vector2 ret;
		ret.x = x * scaler;
		ret.y = y * scaler;
		return ret;
	}
	Vector2 operator/(float scaler) {
		Vector2 ret;
		ret.x = x / scaler;
		ret.y = y / scaler;
		return ret;
	}

	void operator+=(Vector2 v) {
		x += v.x;
		y += v.y;
	}
	void operator-=(Vector2 v) {
		x -= v.x;
		y -= v.y;
	}
	void operator*=(float scaler) {
		x *= scaler;
		y *= scaler;
	}
	void operator/=(float scaler) {
		x /= scaler;
		y /= scaler;
	}
};

inline Vector2 Add(const Vector2& v1, const Vector2& v2) {
	return { v1.x + v2.x, v1.y + v2.y };
}
inline Vector2 Subtract(const Vector2& v1, const Vector2& v2) {
	return { v1.x - v2.x, v1.y - v2.y };
}
inline float Dot(const Vector2& v1, const Vector2& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}
inline float Length(const Vector2& v) {
	return std::sqrt(Dot(v, v));
}

/// <summary>
/// Vector3
/// </summary>
struct Vector3 {
	float x, y, z;
	Vector3() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void Normalize() {
		float length = sqrtf((x * x) + (y * y) + (z * z));
		if (length != 0.0f) {
			x /= length;
			y /= length;
			z /= length;
		}
	}
	static const Vector3 kZero;
	static const Vector3 kOne;
};

inline bool operator==(const Vector3& v1, const Vector3& v2) {
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

inline bool operator!=(const Vector3& v1, const Vector3& v2) {
	return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
}

inline Vector3 operator-(const Vector3& v) {
	return Vector3(-v.x, -v.y, -v.z);
}

inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vector3 operator*(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline Vector3 operator*(const Vector3& v, float a) {
	return Vector3(v.x * a, v.y * a, v.z * a);
}

inline Vector3 operator*(float a, const Vector3& v) {
	return Vector3(a * v.x, a * v.y, a * v.z);
}

inline Vector3 operator/(const Vector3& v, float a) {
	float oneOverA = 1.0f / a;
	return Vector3(v.x * oneOverA, v.y * oneOverA, v.z * oneOverA);
}

inline Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	v1 = v1 + v2;
	return v1;
}

inline Vector3& operator-=(Vector3& v1, const Vector3& v2) {
	v1 = v1 - v2;
	return v1;
}

inline Vector3& operator*=(Vector3& v1, const Vector3& v2) {
	v1 = v1 * v2;
	return v1;
}

inline Vector3& operator*=(Vector3& v, float a) {
	v = v * a;
	return v;
}

inline Vector3& operator/=(Vector3& v, float a) {
	float oneOverA = 1.0f / a;
	v = v * oneOverA;
	return v;
}

inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return Vector3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
}

inline float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float Length(const Vector3& v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline float LengthSq(const Vector3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline Vector3 Normalize(const Vector3& v) {
	Vector3 ret = v;
	ret.Normalize();
	return ret;
}

Vector2 ToVector2(const Vector3 v);
Vector3 ToVector3(const Vector2 v);

inline Vector3 Add(const Vector3& v1, const Vector3& v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}
inline Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}
inline Vector3 Multiply(float scalar, const Vector3& v) {
	return { scalar * v.x, scalar * v.y, scalar * v.z };
}

inline Vector3 Negate(const Vector3& v) {
	return { -v.x, -v.y, -v.z };
}

/// <summary>
/// Vector4
/// </summary>
struct Vector4 {
	float x, y, z, w;
};

/// <summary>
/// Matrix3x3
/// </summary>
struct Matrix3x3 {
	float m[3][3];
};

/// <summary>
/// Matrix4x4
/// </summary>
struct Matrix4x4 {
	float m[4][4];
	Matrix4x4() {

	}
	Matrix4x4(float m[4][4]) {
		memcpy(this->m, m, sizeof(float) * 16);
	}
	static const Matrix4x4 kIdentity;
};

//球
struct Sphere {
	Vector3 center; //中心点
	float radius; //半径
};
//平面
struct Plane {
	Vector3 normal; //法線
	float distance; //距離
};
//AABB
struct AABB {
	Vector3 min; //最小
	Vector3 max; //最大
};

//OBB
struct OBB {
	Vector3 center; //中心点
	Vector3 axis[3]; //座標軸。正規化必須
	Vector3 size; //座標軸方向の長さの半分。中心から面までの距離

	OBB()
		:center(Vector3::kZero)
		, size(Vector3::kZero)
	{
		axis[0] = Vector3(1.0f, 0.0f, 0.0f);
		axis[1] = Vector3(0.0f, 1.0f, 0.0f);
		axis[2] = Vector3(0.0f, 0.0f, 1.0f);
	}
	OBB(const Vector3& center, const Vector3& size)
		:center(center)
		, size(size)
	{
		axis[0] = Vector3(1.0f, 0.0f, 0.0f);
		axis[1] = Vector3(0.0f, 1.0f, 0.0f);
		axis[2] = Vector3(0.0f, 0.0f, 1.0f);
	}
	Matrix4x4 CreateInverse() const
	{
		Matrix4x4 ret = Matrix4x4::kIdentity;
		ret.m[0][0] = axis[0].x;
		ret.m[0][1] = axis[1].x;
		ret.m[0][2] = axis[2].x;
		ret.m[1][0] = axis[0].y;
		ret.m[1][1] = axis[1].y;
		ret.m[1][2] = axis[2].y;
		ret.m[2][0] = axis[0].z;
		ret.m[2][1] = axis[1].z;
		ret.m[2][2] = axis[2].z;
		ret.m[3][0] = -Dot(center, axis[0]);
		ret.m[3][1] = -Dot(center, axis[1]);
		ret.m[3][2] = -Dot(center, axis[2]);
		return ret;
	}
};

struct CollisionInfo {
	Vector3 normal;
	float depth;
};

//line
struct Line {
	Vector3 origin; //始点
	Vector3 diff; //終点への差分ベクトル
	static constexpr float kTMin = std::numeric_limits<float>::lowest();
	static constexpr float kTMax = (std::numeric_limits<float>::max)();
};

//Ray
struct Ray {
	Vector3 origin; //始点
	Vector3 diff; //終点の差分ベクトル
	static constexpr float kTMin = 0.0f;
	static constexpr float kTMax = (std::numeric_limits<float>::max)();
};

//Segment
struct Segment {
	Vector3 origin; //始点
	Vector3 diff; //終点への差分ベクトル
	static constexpr float kTMin = 0.0f;
	static constexpr float kTMax = 1.0f;
};

//三角形
struct Triangle {
	Vector3 vertices[3];
};

struct Capsule {
	Segment segment;
	float radius;
};

struct SphericalCoordinate {
	float theta;
	float phi;
	float radius;
};

struct CollisionResult {
	bool isColliding;
	float depth;
	Vector3 normal;
};

// ベクトル射影 v1をv2に射影する
inline Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float v2SqLength = Dot(v2, v2);
	float dot = Dot(v1, v2);
	return Multiply(dot / v2SqLength, v2);
}

inline Vector3 Multiply(const Vector3& v, const Matrix3x3& m) {
	Vector3 ret;
	ret.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	ret.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
	ret.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
	return ret;
}

inline Vector3 Multiply(const Vector3& v, const Matrix4x4& m) {
	Vector3 ret;
	ret.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + 1.0f * m.m[3][0];
	ret.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + 1.0f * m.m[3][1];
	ret.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + 1.0f * m.m[3][2];
	return ret;
}

inline Matrix3x3 Transpose(const Matrix3x3& mMatrix) {
	Matrix3x3 ret;
	ret.m[0][0] = mMatrix.m[0][0];
	ret.m[0][1] = mMatrix.m[1][0];
	ret.m[0][2] = mMatrix.m[2][0];
	ret.m[1][0] = mMatrix.m[0][1];
	ret.m[1][1] = mMatrix.m[1][1];
	ret.m[1][2] = mMatrix.m[2][1];
	ret.m[2][0] = mMatrix.m[0][2];
	ret.m[2][1] = mMatrix.m[1][2];
	ret.m[2][2] = mMatrix.m[2][2];
	return ret;
}

inline Matrix3x3 Inverse(const Matrix3x3& mMatrix) {
	Matrix3x3 ret;
	float determinant = mMatrix.m[0][0] * mMatrix.m[1][1] * mMatrix.m[2][2] +
		mMatrix.m[0][1] * mMatrix.m[1][2] * mMatrix.m[2][0] +
		mMatrix.m[0][2] * mMatrix.m[1][0] * mMatrix.m[2][1] -
		mMatrix.m[0][2] * mMatrix.m[1][1] * mMatrix.m[2][0] -
		mMatrix.m[0][1] * mMatrix.m[1][0] * mMatrix.m[2][2] -
		mMatrix.m[0][0] * mMatrix.m[1][2] * mMatrix.m[2][1];
	assert(determinant != 0.0f);
	float determinantRecp = 1.0f / determinant;

	ret.m[0][0] =
		(mMatrix.m[1][1] * mMatrix.m[2][2] - mMatrix.m[1][2] * mMatrix.m[2][1]) * determinantRecp;
	ret.m[0][1] =
		-(mMatrix.m[0][1] * mMatrix.m[2][2] - mMatrix.m[0][2] * mMatrix.m[2][1]) * determinantRecp;
	ret.m[0][2] =
		(mMatrix.m[0][1] * mMatrix.m[1][2] - mMatrix.m[0][2] * mMatrix.m[1][1]) * determinantRecp;

	ret.m[1][0] =
		-(mMatrix.m[1][0] * mMatrix.m[2][2] - mMatrix.m[1][2] * mMatrix.m[2][0]) * determinantRecp;
	ret.m[1][1] =
		(mMatrix.m[0][0] * mMatrix.m[2][2] - mMatrix.m[0][2] * mMatrix.m[2][0]) * determinantRecp;
	ret.m[1][2] =
		-(mMatrix.m[0][0] * mMatrix.m[1][2] - mMatrix.m[0][2] * mMatrix.m[1][0]) * determinantRecp;

	ret.m[2][0] =
		(mMatrix.m[1][0] * mMatrix.m[2][1] - mMatrix.m[1][1] * mMatrix.m[2][0]) * determinantRecp;
	ret.m[2][1] =
		-(mMatrix.m[0][0] * mMatrix.m[2][1] - mMatrix.m[0][1] * mMatrix.m[2][0]) * determinantRecp;
	ret.m[2][2] =
		(mMatrix.m[0][0] * mMatrix.m[1][1] - mMatrix.m[0][1] * mMatrix.m[1][0]) * determinantRecp;

	return ret;
}

inline Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 ret;
	ret.m[0][0] = m1.m[0][0] + m2.m[0][0];
	ret.m[0][1] = m1.m[0][1] + m2.m[0][1];
	ret.m[0][2] = m1.m[0][2] + m2.m[0][2];
	ret.m[0][3] = m1.m[0][3] + m2.m[0][3];

	ret.m[1][0] = m1.m[1][0] + m2.m[1][0];
	ret.m[1][1] = m1.m[1][1] + m2.m[1][1];
	ret.m[1][2] = m1.m[1][2] + m2.m[1][2];
	ret.m[1][3] = m1.m[1][3] + m2.m[1][3];

	ret.m[2][0] = m1.m[2][0] + m2.m[2][0];
	ret.m[2][1] = m1.m[2][1] + m2.m[2][1];
	ret.m[2][2] = m1.m[2][2] + m2.m[2][2];
	ret.m[2][3] = m1.m[2][3] + m2.m[2][3];

	ret.m[3][0] = m1.m[3][0] + m2.m[3][0];
	ret.m[3][1] = m1.m[3][1] + m2.m[3][1];
	ret.m[3][2] = m1.m[3][2] + m2.m[3][2];
	ret.m[3][3] = m1.m[3][3] + m2.m[3][3];
	return ret;
}

// 2. 行列の減法
inline Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 ret;
	ret.m[0][0] = m1.m[0][0] - m2.m[0][0];
	ret.m[0][1] = m1.m[0][1] - m2.m[0][1];
	ret.m[0][2] = m1.m[0][2] - m2.m[0][2];
	ret.m[0][3] = m1.m[0][3] - m2.m[0][3];

	ret.m[1][0] = m1.m[1][0] - m2.m[1][0];
	ret.m[1][1] = m1.m[1][1] - m2.m[1][1];
	ret.m[1][2] = m1.m[1][2] - m2.m[1][2];
	ret.m[1][3] = m1.m[1][3] - m2.m[1][3];

	ret.m[2][0] = m1.m[2][0] - m2.m[2][0];
	ret.m[2][1] = m1.m[2][1] - m2.m[2][1];
	ret.m[2][2] = m1.m[2][2] - m2.m[2][2];
	ret.m[2][3] = m1.m[2][3] - m2.m[2][3];

	ret.m[3][0] = m1.m[3][0] - m2.m[3][0];
	ret.m[3][1] = m1.m[3][1] - m2.m[3][1];
	ret.m[3][2] = m1.m[3][2] - m2.m[3][2];
	ret.m[3][3] = m1.m[3][3] - m2.m[3][3];
	return ret;
}

inline Matrix4x4 Inverse(const Matrix4x4& m) {
	// clang-format off
	float determinant = +m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]
		+ m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]
		+ m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]

		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]
		- m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]
		- m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]

		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]
		- m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]
		- m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]

		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]
		+ m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]
		+ m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]

		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]
		+ m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]

		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]
		- m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]
		- m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]

		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]
		- m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]
		- m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]

		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]
		+ m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]
		+ m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	Matrix4x4 ret;
	float recpDeterminant = 1.0f / determinant;
	ret.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) * recpDeterminant;
	ret.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) * recpDeterminant;
	ret.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) * recpDeterminant;
	ret.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) * recpDeterminant;

	ret.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
		m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) * recpDeterminant;
	ret.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
		m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
		m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) * recpDeterminant;
	ret.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
		m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
		m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) * recpDeterminant;
	ret.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) * recpDeterminant;

	ret.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) * recpDeterminant;
	ret.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
		m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
		m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) * recpDeterminant;
	ret.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
		m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
		m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) * recpDeterminant;
	ret.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) * recpDeterminant;

	ret.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
		m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) * recpDeterminant;
	ret.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
		m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) * recpDeterminant;
	ret.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
		m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
		m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) * recpDeterminant;
	ret.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) * recpDeterminant;

	return ret;
	// clang-format on
}

inline Matrix4x4 InverseAffine(const Matrix4x4& m) {
	Matrix3x3 minor;
	minor.m[0][0] = m.m[0][0];
	minor.m[0][1] = m.m[0][1];
	minor.m[0][2] = m.m[0][2];

	minor.m[1][0] = m.m[1][0];
	minor.m[1][1] = m.m[1][1];
	minor.m[1][2] = m.m[1][2];

	minor.m[2][0] = m.m[2][0];
	minor.m[2][1] = m.m[2][1];
	minor.m[2][2] = m.m[2][2];
	Matrix3x3 minorInverse = Inverse(minor);
	Vector3 translateNegate{ -m.m[3][0], -m.m[3][1], -m.m[3][2] };
	Vector3 translateInverse = Multiply(translateNegate, minorInverse);
	float tmp[4][4] = {
		{minorInverse.m[0][0], minorInverse.m[0][1], minorInverse.m[0][2], 0.0f},
	   {minorInverse.m[1][0], minorInverse.m[1][1], minorInverse.m[1][2], 0.0f},
	   {minorInverse.m[2][0], minorInverse.m[2][1], minorInverse.m[2][2], 0.0f},
	   {translateInverse.x, translateInverse.y, translateInverse.z, 1.0f}
	};
	return Matrix4x4(tmp);
}

inline Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 ret;
	ret.m[0][0] = m.m[0][0];
	ret.m[0][1] = m.m[1][0];
	ret.m[0][2] = m.m[2][0];
	ret.m[0][3] = m.m[3][0];

	ret.m[1][0] = m.m[0][1];
	ret.m[1][1] = m.m[1][1];
	ret.m[1][2] = m.m[2][1];
	ret.m[1][3] = m.m[3][1];

	ret.m[2][0] = m.m[0][2];
	ret.m[2][1] = m.m[1][2];
	ret.m[2][2] = m.m[2][2];
	ret.m[2][3] = m.m[3][2];

	ret.m[3][0] = m.m[0][3];
	ret.m[3][1] = m.m[1][3];
	ret.m[3][2] = m.m[2][3];
	ret.m[3][3] = m.m[3][3];

	return ret;
}

inline Matrix4x4 MakeIdentity4x4() {
	// clang-format off
	Matrix4x4 identity;
	identity.m[0][0] = 1.0f;
	identity.m[0][1] = 0.0f;
	identity.m[0][2] = 0.0f;
	identity.m[0][3] = 0.0f;
	identity.m[1][0] = 0.0f;
	identity.m[1][1] = 1.0f;
	identity.m[1][2] = 0.0f;
	identity.m[1][3] = 0.0f;
	identity.m[2][0] = 0.0f;
	identity.m[2][1] = 0.0f;
	identity.m[2][2] = 1.0f;
	identity.m[2][3] = 0.0f;
	identity.m[3][0] = 0.0f;
	identity.m[3][1] = 0.0f;
	identity.m[3][2] = 0.0f;
	identity.m[3][3] = 1.0f;
	return identity;
	// clang-format on
}

inline Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	float tmp[4][4] = {
		{1.0f,0.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f,0.0f},
		{0.0f,0.0f,1.0f,0.0f},
		{translate.x,translate.y,translate.z,1.0f}
	};
	return Matrix4x4(tmp);
}

inline Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	float tmp[4][4] = {
		{scale.x,0.0f,0.0f,0.0f},
		{0.0f,scale.y,0.0f,0.0f},
		{0.0f,0.0f,scale.z,0.0f},
		{0.0f,0.0f,0.0f,1.0f}
	};
	return Matrix4x4(tmp);
}

inline Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 ret;
	ret.m[0][0] = 1.0f;
	ret.m[0][1] = 0.0f;
	ret.m[0][2] = 0.0f;
	ret.m[0][3] = 0.0f;
	ret.m[1][0] = 0.0f;
	ret.m[1][1] = std::cos(radian);
	ret.m[1][2] = std::sin(radian);
	ret.m[1][3] = 0.0f;
	ret.m[2][0] = 0.0f;
	ret.m[2][1] = -std::sin(radian);
	ret.m[2][2] = std::cos(radian);
	ret.m[2][3] = 0.0f;
	ret.m[3][0] = 0.0f;
	ret.m[3][1] = 0.0f;
	ret.m[3][2] = 0.0f;
	ret.m[3][3] = 1.0f;
	return ret;
}

inline Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 ret;
	ret.m[0][0] = std::cos(radian);
	ret.m[0][1] = 0.0f;
	ret.m[0][2] = -std::sin(radian);
	ret.m[0][3] = 0.0f;
	ret.m[1][0] = 0.0f;
	ret.m[1][1] = 1.0f;
	ret.m[1][2] = 0.0f;
	ret.m[1][3] = 0.0f;
	ret.m[2][0] = std::sin(radian);
	ret.m[2][1] = 0.0f;
	ret.m[2][2] = std::cos(radian);
	ret.m[2][3] = 0.0f;
	ret.m[3][0] = 0.0f;
	ret.m[3][1] = 0.0f;
	ret.m[3][2] = 0.0f;
	ret.m[3][3] = 1.0f;
	return ret;
}

inline Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 ret;
	ret.m[0][0] = std::cos(radian);
	ret.m[0][1] = std::sin(radian);
	ret.m[0][2] = 0.0f;
	ret.m[0][3] = 0.0f;
	ret.m[1][0] = -std::sin(radian);
	ret.m[1][1] = std::cos(radian);
	ret.m[1][2] = 0.0f;
	ret.m[1][3] = 0.0f;
	ret.m[2][0] = 0.0f;
	ret.m[2][1] = 0.0f;
	ret.m[2][2] = 1.0f;
	ret.m[2][3] = 0.0f;
	ret.m[3][0] = 0.0f;
	ret.m[3][1] = 0.0f;
	ret.m[3][2] = 0.0f;
	ret.m[3][3] = 1.0f;
	return ret;
}

inline Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 ret;
	ret.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
		m1.m[0][3] * m2.m[3][0];
	ret.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
		m1.m[0][3] * m2.m[3][1];
	ret.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
		m1.m[0][3] * m2.m[3][2];
	ret.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
		m1.m[0][3] * m2.m[3][3];

	ret.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
		m1.m[1][3] * m2.m[3][0];
	ret.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
		m1.m[1][3] * m2.m[3][1];
	ret.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
		m1.m[1][3] * m2.m[3][2];
	ret.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
		m1.m[1][3] * m2.m[3][3];

	ret.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
		m1.m[2][3] * m2.m[3][0];
	ret.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
		m1.m[2][3] * m2.m[3][1];
	ret.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
		m1.m[2][3] * m2.m[3][2];
	ret.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
		m1.m[2][3] * m2.m[3][3];

	ret.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
		m1.m[3][3] * m2.m[3][0];
	ret.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
		m1.m[3][3] * m2.m[3][1];
	ret.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
		m1.m[3][3] * m2.m[3][2];
	ret.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
		m1.m[3][3] * m2.m[3][3];

	return ret;
}

inline Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 ret = Multiply(
		Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)),
		MakeRotateZMatrix(rotate.z));
	ret.m[0][0] *= scale.x;
	ret.m[0][1] *= scale.x;
	ret.m[0][2] *= scale.x;

	ret.m[1][0] *= scale.y;
	ret.m[1][1] *= scale.y;
	ret.m[1][2] *= scale.y;

	ret.m[2][0] *= scale.z;
	ret.m[2][1] *= scale.z;
	ret.m[2][2] *= scale.z;

	ret.m[3][0] = translate.x;
	ret.m[3][1] = translate.y;
	ret.m[3][2] = translate.z;
	return ret;
}

inline Matrix4x4 CreateRotate(const Quaternion& q) {
	float ww = 2.0f * q.w;
	float xx = 2.0f * q.x;
	float yy = 2.0f * q.y;
	float zz = 2.0f * q.z;
	Matrix4x4 ret = MakeIdentity4x4();
	ret.m[0][0] = 1.0f - yy * q.y - zz * q.z;
	ret.m[0][1] = xx * q.y + ww * q.z;
	ret.m[0][2] = xx * q.z - ww * q.y;
	ret.m[1][0] = xx * q.y - ww * q.z;
	ret.m[1][1] = 1.0f - xx * q.x - zz * q.z;
	ret.m[1][2] = yy * q.z + ww * q.x;
	ret.m[2][0] = xx * q.z + ww * q.y;
	ret.m[2][1] = yy * q.z - ww * q.x;
	ret.m[2][2] = 1.0f - xx * q.x - yy * q.y;
	return ret;
}

inline Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	Matrix4x4 ret = CreateRotate(rotate);
	ret.m[0][0] *= scale.x;
	ret.m[0][1] *= scale.x;
	ret.m[0][2] *= scale.x;
	ret.m[1][0] *= scale.y;
	ret.m[1][1] *= scale.y;
	ret.m[1][2] *= scale.y;
	ret.m[2][0] *= scale.z;
	ret.m[2][1] *= scale.z;
	ret.m[2][2] *= scale.z;
	ret.m[3][0] = translate.x;
	ret.m[3][1] = translate.y;
	ret.m[3][2] = translate.z;
	return ret;
}

//透視投影行列
inline Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 ret;
	float dot = farClip * nearClip;
	float tan = std::tan(fovY / 2.0f);
	ret.m[0][0] = 1.0f / (aspectRatio * tan);
	ret.m[0][1] = 0.0f;
	ret.m[0][2] = 0.0f;
	ret.m[0][3] = 0.0f;
	ret.m[1][0] = 0.0f;
	ret.m[1][1] = 1.0f / tan;
	ret.m[1][2] = 0.0f;
	ret.m[1][3] = 0.0f;
	ret.m[2][0] = 0.0f;
	ret.m[2][1] = 0.0f;
	ret.m[2][2] = farClip / (farClip - nearClip);
	ret.m[2][3] = 1.0f;
	ret.m[3][0] = 0.0f;
	ret.m[3][1] = 0.0f;
	ret.m[3][2] = -dot / (farClip - nearClip);
	ret.m[3][3] = 0.0f;
	return ret;
}

//正射影行列(平行投影行列)
inline Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 ret;
	ret.m[0][0] = 2.0f / (right - 1.0f);
	ret.m[0][1] = 0.0f;
	ret.m[0][2] = 0.0f;
	ret.m[0][3] = 0.0f;
	ret.m[1][0] = 0.0f;
	ret.m[1][1] = 2.0f / (top - bottom);
	ret.m[1][2] = 0.0f;
	ret.m[1][3] = 0.0f;
	ret.m[2][0] = 0.0f;
	ret.m[2][1] = 0.0f;
	ret.m[2][2] = 1.0f / (farClip - nearClip);
	ret.m[2][3] = 0.0f;
	ret.m[3][0] = (left + right) / (left - right);
	ret.m[3][1] = (top + bottom) / (bottom - top);
	ret.m[3][2] = nearClip / (nearClip - farClip);
	ret.m[3][3] = 1.0f;
	return ret;
}

//ビューポート変換行列
inline Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 ret;
	ret.m[0][0] = width / 2.0f;
	ret.m[0][1] = 0.0f;
	ret.m[0][2] = 0.0f;
	ret.m[0][3] = 0.0f;
	ret.m[1][0] = 0.0f;
	ret.m[1][1] = -(height / 2.0f);
	ret.m[1][2] = 0.0f;
	ret.m[1][3] = 0.0f;
	ret.m[2][0] = 0.0f;
	ret.m[2][1] = 0.0f;
	ret.m[2][2] = maxDepth - minDepth;
	ret.m[2][3] = 0.0f;
	ret.m[3][0] = left + width / 2.0f;
	ret.m[3][1] = top + height / 2.0f;
	ret.m[3][2] = minDepth;
	ret.m[3][3] = 1.0f;
	return ret;
}

//球と球の衝突判定
inline bool IsCollision(const Sphere& s1, const Sphere& s2) {
	Vector3 distanceVector = Subtract(s1.center, s2.center);
	float distanceSq = Dot(distanceVector, distanceVector);
	float sumRadius = s1.radius + s2.radius;
	if (distanceSq <= (sumRadius * sumRadius)) {
		return true;
	} else {
		return false;
	}
}

//球と平面の衝突判定
inline bool IsCollision(const Sphere& sphere, const Plane& plane) {
	float distance = std::abs(Dot(sphere.center, plane.normal) - plane.distance);
	if (distance <= sphere.radius) {
		return true;
	} else {
		return false;
	}
}

//BoxとBoxの衝突判定
inline bool IsCollision(const AABB& aabb1, const AABB& aabb2, CollisionResult& result) {
	if (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x &&
		aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y &&
		aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z) {
		float x = Min(aabb1.max.x - aabb2.min.x, aabb2.max.x - aabb1.min.x);
		float y = Min(aabb1.max.y - aabb2.min.y, aabb2.max.y - aabb1.min.y);
		float z = Min(aabb1.max.z - aabb2.min.z, aabb2.max.z - aabb1.min.z);
		float minOverlap = kInfinity;
		if (x < minOverlap) {
			minOverlap = x;
			if (aabb1.max.x - aabb2.min.x < aabb2.max.x - aabb1.min.x) {
				result.normal = Vector3(-1.0f, 0.0f, 0.0f);
			} else {
				result.normal = Vector3(1.0f, 0.0f, 0.0f);
			}
		}
		if (y < minOverlap) {
			minOverlap = y;
			if (aabb1.max.y - aabb2.min.y < aabb2.max.y - aabb1.min.y) {
				result.normal = Vector3(0.0f, -1.0f, 0.0f);
			} else {
				result.normal = Vector3(0.0f, 1.0f, 0.0f);
			}
		}
		if (z < minOverlap) {
			minOverlap = z;
			if (aabb1.max.z - aabb2.min.z < aabb2.max.z - aabb1.min.z) {
				result.normal = Vector3(0.0f, 0.0f, -1.0f);
			} else {
				result.normal = Vector3(0.0f, 0.0f, 1.0f);
			}
		}
		result.depth = minOverlap;
		return true;
	}
	return false;
}

//三角形とラインの衝突判定
template<typename tLine> bool IsCollision(const Triangle& triangle, const tLine& line) {
	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 normal = Normalize(Cross(v01, v12));
	Plane plane{ .normal = normal, .distance = Dot(triangle.vertices[0],normal) };
	float dot = Dot(plane.normal, line.diff);
	if (dot == 0.0f) {
		return false;
	}
	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
	if ((t < tLine::kTMin) || (tLine::kTMax < t)) {
		return false;
	}
	Vector3 intersect = Add(line.origin, Multiply(t, line.diff));
	Vector3 v1p = Subtract(intersect, triangle.vertices[1]);
	if (Dot(Cross(v01, v1p), normal) < 0.0f) {
		return false;
	}
	Vector3 v2p = Subtract(intersect, triangle.vertices[2]);
	if (Dot(Cross(v12, v2p), normal) < 0.0f) {
		return false;
	}
	Vector3 v0p = Subtract(intersect, triangle.vertices[0]);
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
	if (Dot(Cross(v20, v0p), normal) < 0.0f) {
		return false;
	}
	return true;
}

//ボックスと球体の衝突判定
inline bool IsCollision(const AABB& aabb, const Sphere& sphere) {
	//最近接点を求める
	Vector3 closestPoint{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z) };
	//最近接点と球の中心との距離を求める
	float distance = Length(Subtract(closestPoint, sphere.center));
	//距離が半径よりも小さければ衝突
	return distance <= sphere.radius;
}

//OBBとOBBの衝突判定
inline bool IsCollision(const OBB& a, const OBB& b, CollisionResult& result) {
	//分離軸
	Vector3 axes[15];
	axes[0] = a.axis[0];
	axes[1] = a.axis[1];
	axes[2] = a.axis[2];
	axes[3] = b.axis[0];
	axes[4] = b.axis[1];
	axes[5] = b.axis[2];
	axes[6] = Normalize(Cross(a.axis[0], b.axis[0]));
	axes[7] = Normalize(Cross(a.axis[0], b.axis[1]));
	axes[8] = Normalize(Cross(a.axis[0], b.axis[2]));
	axes[9] = Normalize(Cross(a.axis[1], b.axis[0]));
	axes[10] = Normalize(Cross(a.axis[1], b.axis[1]));
	axes[11] = Normalize(Cross(a.axis[1], b.axis[2]));
	axes[12] = Normalize(Cross(a.axis[2], b.axis[0]));
	axes[13] = Normalize(Cross(a.axis[2], b.axis[1]));
	axes[14] = Normalize(Cross(a.axis[2], b.axis[2]));
	Vector3 d1[3] = {
		a.axis[0] * a.size.x,
		a.axis[1] * a.size.y,
		a.axis[2] * a.size.z
	};
	Vector3 d2[3] = {
		b.axis[0] * b.size.x,
		b.axis[1] * b.size.y,
		b.axis[2] * b.size.z
	};
	Vector3 c1[8] = {
		a.center + d1[0] + d1[1] + d1[2],
		a.center + d1[0] + d1[1] - d1[2],
		a.center + d1[0] - d1[1] + d1[2],
		a.center + d1[0] - d1[1] - d1[2],
		a.center - d1[0] + d1[1] + d1[2],
		a.center - d1[0] + d1[1] - d1[2],
		a.center - d1[0] - d1[1] + d1[2],
		a.center - d1[0] - d1[1] - d1[2]
	};
	Vector3 c2[8] = {
		b.center + d2[0] + d2[1] + d2[2],
		b.center + d2[0] + d2[1] - d2[2],
		b.center + d2[0] - d2[1] + d2[2],
		b.center + d2[0] - d2[1] - d2[2],
		b.center - d2[0] + d2[1] + d2[2],
		b.center - d2[0] + d2[1] - d2[2],
		b.center - d2[0] - d2[1] + d2[2],
		b.center - d2[0] - d2[1] - d2[2]
	};
	float minOverlap = kInfinity;
	Vector3 minAxis;
	for (const auto& axis : axes) {
		if (axis.x > -kEpsilon && axis.x < kEpsilon &&
			axis.y > -kEpsilon && axis.y < kEpsilon &&
			axis.z > -kEpsilon && axis.z < kEpsilon) {
			continue;
		}
		float min1 = (std::numeric_limits<float>::max)();
		float max1 = std::numeric_limits<float>::lowest();
		float min2 = min1;
		float max2 = max1;
		for (uint32_t i = 0; i < 8; ++i) {
			float dist1 = Dot(c1[i], axis);
			min1 = Min(dist1, min1);
			max1 = Max(dist1, max1);
			float dist2 = Dot(c2[i], axis);
			min2 = Min(dist2, min2);
			max2 = Max(dist2, max2);
		}
		float overlap = (max1 - min1) + (max2 - min2) - (Max(max1, max2) - Min(min1, min2));
		if (overlap < 0.0f) {
			return false;
		}
		if (overlap < minOverlap) {
			minOverlap = overlap;
			minAxis = axis;
		}
	}
	result.normal = Normalize(minAxis);
	if (Dot(a.center - b.center, result.normal) < 0.0f) {
		result.normal = -result.normal;
	}
	result.depth = minOverlap;
	return true;
}

//OBBとAABBの衝突判定
inline bool IsCollision(const AABB& a, const OBB& b, CollisionResult& result) {
	Vector3 size = (a.max - a.min) / 2.0f;
	OBB obb;
	obb.center = a.min + size;
	obb.size = size;
	if (IsCollision(obb, b, result)) {
		return true;
	}
	return false;
}

//カメラのビュー行列を求める関数
inline Matrix4x4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
	Vector3 axisZ = Normalize(Subtract(target, eye));
	Vector3 axisX = Normalize(Cross(up, axisZ));
	Vector3 axisY = Normalize(Cross(axisZ, axisX));
	Vector3 trans = Vector3(-Dot(axisX, eye), -Dot(axisY, eye), -Dot(axisZ, eye));
	Matrix4x4 ret;
	ret.m[0][0] = axisX.x;
	ret.m[0][1] = axisY.x;
	ret.m[0][2] = axisZ.x;
	ret.m[0][3] = 0.0f;
	ret.m[1][0] = axisX.y;
	ret.m[1][1] = axisY.y;
	ret.m[1][2] = axisZ.y;
	ret.m[1][3] = 0.0f;
	ret.m[2][0] = axisX.z;
	ret.m[2][1] = axisY.z;
	ret.m[2][2] = axisZ.z;
	ret.m[2][3] = 0.0f;
	ret.m[3][0] = trans.x;
	ret.m[3][1] = trans.y;
	ret.m[3][2] = trans.z;
	ret.m[3][3] = 1.0f;
	return ret;
}