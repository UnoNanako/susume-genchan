#pragma once
#include "Transform.h"
#include "Camera.h"
#include <wrl.h>
#include <d3d12.h>

class DirectXCommon;
class Player;
class MyMath;
class Input;

class BirdEyeCamera : public Camera{
public:
	BirdEyeCamera();
	void Update(Input* input, Vector3 playerTranslate, bool isTitleScene);
	float GetLon() { return mLon; }
	Matrix4x4 GetViewMatrix() { return mViewMatrix; }
	bool GetIsHit() { return mIsHit; }
	void SetIsHit(bool isHit) { mIsHit = isHit; }

private:
	float mRadius; //球の半径
	float mLat; //緯度
	float mLon; //経度
	Vector3 mTarget; //注視点の座標
	Vector3 mUp; //カメラの上方向の座標
	bool mIsHit = false; 
	float mT; //線形補間
	Vector3 mStartPos; //線形補間開始位置
};

