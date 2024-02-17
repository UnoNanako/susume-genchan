#pragma once
#include "MyMath.h"

//平行光源(Directional Light)
struct DirectionalLight {
	Vector4 color; //ライトの色
	Vector3 direction; //ライトの向き
	float intensity; //輝度
};