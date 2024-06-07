#include "Random.h"
#include "Math/MyMath.h"

std::mt19937 Random::sEngine;

void Random::Initialize()
{
	std::random_device rd;
	sEngine.seed(rd());
}

int Random::Rand(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(sEngine);
}

float Random::Rand(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(sEngine);
}

Vector2 Random::Rand(const Vector2& min, const Vector2& max)
{
	Vector2 ret = Vector2(
		min.x + (max.x - min.x) * Rand(0.0f, 1.0f),
		min.y + (max.y - min.y) * Rand(0.0f, 1.0f));
	return ret;
}

Vector3 Random::Rand(const Vector3& min, const Vector3& max)
{
	Vector3 ret = Vector3(
		min.x + (max.x - min.x) * Rand(0.0f, 1.0f),
		min.y + (max.y - min.y) * Rand(0.0f, 1.0f),
		min.z + (max.z - min.z) * Rand(0.0f, 1.0f));
	return ret;
}
