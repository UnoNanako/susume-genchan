#pragma once
#include "MyMath.h"
#include <string>

//マテリアル
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float shininess;
};

//マテリアルデータ
struct MaterialData {
	std::string textureFilePath;
};