#pragma once
#include "Transform.h"
#include "Camera.h"
#include <wrl.h>
#include <d3d12.h>

class DirectXCommon;
class Player;
class MyMath;
class Input;

class BirdEyeCamera : public Camera
{
public:
	BirdEyeCamera();
	void Update() override;
	void UpdateCameraPosition(const Vector3& translate,float distance,Input* input);
private:
	//カメラの位置(親クラス)
	//ビュー行列(親クラス)
	Vector3 mTarget; //注視点
	Vector3 mUp; //カメラの上方向
	float mAngleY; //横角度
	float mAngleX; //縦方向
};

