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
private:
	float mRadius; //球の半径
	float mPolarAngle; //方位角
	float mAzimuthalAngle; //仰角
	Vector3 mTarget; //注視点の座標
	Vector3 mUp; //カメラの上方向の座標
};

