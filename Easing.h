#pragma once

class MyMath;

template <typename T>
T Leap(const T& start, const T& end, float t) {
	return start + (end - start) * t;
}

namespace Easing {
	float EaseInBack(float t) {
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;
		return c3 * t * t * t - c1 * t * t;
	}
	float EaseOutBack(float t) {
		float c1 = 1.70158f;
		float c3 = c1 + 1.0f;
		return float(1 + c3 * pow(t - 1, 3) + c1 * pow(t - 1, 2));
	}
	float EaseInOutCubic(float t) {
		return t < 0.5f ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2;
	}
}