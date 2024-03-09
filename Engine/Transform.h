#pragma once
#include "MyMath.h"
struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;

	//ローカル→ワールド変換行列
	Matrix4x4 mMatWorld;
	//親となるワールド変換へのポインタ
	Transform* mParent = nullptr;

	//行列を更新する
	void UpdateMatrix();
};