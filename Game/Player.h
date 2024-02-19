#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <d3d12.h>

using namespace std;

class Input;
class Texture;
class LightList;
struct AABB;

class Player : public GameObject
{
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update(Input* input);
	void Draw(ID3D12GraphicsCommandList* commandList,Camera* camera);
	void SetLightList(LightList* lightList) { mLightList = lightList; }
	AABB GetAABB() { return mAABBtranslate; }
	Vector3 GetVelocity() { return mVelocity; }
	Vector3 GetTranslate() { return mTransform.translate; }
	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }
	void SetVelocity(Vector3 velocity) { mVelocity = velocity; }
	void SetIsHit(bool isHit) { mIsHit = isHit; }
private:
	Vector3 mVelocity;
	Texture* mTexture;
	LightList* mLightList;

	AABB mAABBtranslate;
	bool mIsHit;
	bool mIsPlayerCamera;
	float mSpeed;
	float mRotateSpeed;
};

