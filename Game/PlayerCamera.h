#pragma once
#include "Transform.h"
#include "Camera.h"
#include <wrl.h>
#include <d3d12.h>

class DirectXCommon;

class PlayerCamera : public Camera{
public:
	PlayerCamera();
	void Update() override;
};

