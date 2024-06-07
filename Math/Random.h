#pragma once
#include <random>

struct Vector2;
struct Vector3;

class Random
{
public:
	static void Initialize();

	static int Rand(int min, int max);
	static float Rand(float min, float max);
	static Vector2 Rand(const Vector2& min, const Vector2& max);
	static Vector3 Rand(const Vector3& min, const Vector3& max);
private:
	//メルセンヌ・ツイスタ
	static std::mt19937 sEngine;
};

