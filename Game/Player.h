#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <d3d12.h>

class Input;
class Texture;
class LightList;
struct AABB;
class MovingFloor;

class Player : public GameObject
{
public:
	Player();
	void Initialize(DirectXCommon* dxCommon);
	void Update(Input* input, float theta);
	void Draw(ID3D12GraphicsCommandList* commandList,Camera* camera);
	
	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABBtranslate; }
	Vector3 GetVelocity() { return mVelocity; }
	Vector3 GetTranslate() { return mTransform.translate; }
	Transform GetTransform() { return mTransform; }
	void SetTranslate(Vector3 translate) { mTransform.translate = translate; }
	void SetRotate(float rotateY) { mTransform.rotate.y = rotateY; }
	void SetVelocity(Vector3 velocity) { mVelocity = velocity; }
	void SetIsHit(bool isHit) { mIsHit = isHit; }
	void SetLightList(LightList* lightList) { mLightList = lightList; }
	void SetMovingFloor(MovingFloor* movingFloor) { mMovingFloor = movingFloor; }

private:
	Vector3 mVelocity;
	Texture* mTexture;
	LightList* mLightList;

	AABB mAABBtranslate;
	bool mIsHit;
	float mSpeed;
	float mRotateSpeed;
	float mGravity; //重力

	Matrix4x4 mTransposeViewMatrix;

	MovingFloor* mMovingFloor;
};

