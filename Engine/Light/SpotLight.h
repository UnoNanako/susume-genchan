#pragma once
#include "MyMath.h"

struct SpotLight {
	Vector4 color; //ライトの色
	Vector3 position; //ライトの位置
	float intensity; //輝度
	Vector3 direction; //スポットライトの方向
	float distance; //ライトの届く最大距離
	float decay; //減衰率
	float cosAngle; //スポットライトの余弦
	float cosFalloffStart;
	float padding;//段ボールの隙間に入ってる紙みたいなもの。隙間を埋める空の4バイト
};