#pragma once
#include "MyMath.h"

//ポイントライト
struct PointLight {
	Vector4 color; //ライトの色
	Vector3 position; //ライトの位置
	float intensity; //輝度
	float radius; //ライトの届く最大距離
	float decay; //減衰率
};