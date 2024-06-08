#pragma once
#include "MyMath.h"

//Particle構造体
struct Particle {
	Transform mTransform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

struct ParticleForGPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};

struct Emitter {
	Transform transform; //エミッタのTransform
	uint32_t count; //発生数
	float frequency; //発生頻度
	float frequencyTime; //頻度用時刻
};