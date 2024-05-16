#pragma once
#include "Transform.h"
#include <d3d12.h>

class DirectXCommon;
class Model;
class Texture;
class Camera;
struct VertexData;

class GameObject{
public:
	virtual void Initialize(DirectXCommon* dxCommon);
	virtual void Update();
	virtual void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);

	/// <summary>
	/// アクセッサ
	/// </summary>
	Transform GetTransform() { return mTransform; }
	Vector3 GetScale() { return mTransform.scale; }
	Vector3 GetRotate() { return mTransform.rotate; }
	Vector3 GetTranslate() { return mTransform.translate; }
	void SetTransform(Transform transform) { mTransform = transform; }
	void SetScale(Vector3 scale) { mTransform.scale = scale; }
	void SetRotate(Vector3 rotate) { mTransform.rotate = rotate; }
	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }

protected:
	Model* mModel;
	Texture* mTexture;
	Transform mTransform;
	DirectXCommon* mDxCommon;
};

