#pragma once

class MyMath;

template <typename T>
T Leap(const T& start, const T& end, float mInT) {
	return start + (end - start) * mInT;
}

namespace Easing {
	float EaseInBack(float mInT) {
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;
		return c3 * mInT * mInT * mInT - c1 * mInT * mInT;
	}
	float EaseOutBack(float mInT) {
		float c1 = 1.70158f;
		float c3 = c1 + 1.0f;
		return float(1 + c3 * pow(mInT - 1, 3) + c1 * pow(mInT - 1, 2));
	}
	float EaseInOutCubic(float mInT) {
		return mInT < 0.5f ? 4 * mInT * mInT * mInT : 1 - pow(-2 * mInT + 2, 3) / 2;
	}
}