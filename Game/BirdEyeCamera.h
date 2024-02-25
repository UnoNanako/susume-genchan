#pragma once
#include "Transform.h"
#include "Camera.h"
#include <wrl.h>
#include <d3d12.h>

class DirectXCommon;
class Player;
class MyMath;

class BirdEyeCamera : public Camera
{
public:
	BirdEyeCamera();
	void Update() override;
	void UpdateCameraPosition(const Vector3& translate,float angle,float distance);
private:
	
};

