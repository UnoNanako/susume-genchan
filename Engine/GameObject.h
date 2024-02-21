#pragma once
#include "Transform.h"
#include <d3d12.h>

class DirectXCommon;
class Model;
class Texture;
class Camera;
struct VertexData;

class GameObject
{
public:
	virtual void Initialize(DirectXCommon *dxCommon);
	virtual void Update();
	virtual void Draw(ID3D12GraphicsCommandList* commandList,Camera* camera);
protected:
	Model* mModel;
	Texture* mTexture;
	Transform mTransform;
	DirectXCommon* mDxCommon;
};

