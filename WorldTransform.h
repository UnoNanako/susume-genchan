#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Math/MyMath.h"

class WorldTransform
{
public:
	//ローカルスケール
	Vector3 mScale = { 1.0f,1.0f,1.0f };
	//X,Y,Z軸回りのローカル回転角
	Vector3 mRotation = { 0.0f,0.0f,0.0f };
	//ローカル座標
	Vector3 mTranslation = { 0.0f,0.0f,0.0f };
	//ローカル→ワールド変換行列
	Matrix4x4 mMatWorld;
	//親となるワールド変換へのポインタ
	WorldTransform* mParent = nullptr;
	

};

