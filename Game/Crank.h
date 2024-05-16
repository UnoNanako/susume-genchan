#pragma once
#include "Engine/GameObject.h"
#include "Transform.h"
#include <d3d12.h>
#include <vector>

class Input;
class AABB;
class Model;

class Crank : public GameObject{
public:
	Crank();
	~Crank();
	void Initialize(DirectXCommon* dxCommon);
	void Update(Input* input);
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	//minとmaxを求める関数
	AABB CalculateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }
	bool GetIsHit() { return mIsHit; }
	void SetIsHit(bool isHit) { mIsHit = isHit; }
	bool GetIsPushA() { return mIsPushA; }
	void SetIsPushA(bool isPushA) { mIsPushA = isPushA; }
	
private:
	AABB mAABB; 
	Model* mFoundationModel; //土台
	Transform mFoundationTransform; //土台
	float mCalculateAngle; //前の角度
	float mCurrentAngle; //現在の角度
	bool mIsHit = false; //プレイヤーと当たっているか
	bool mIsPushA = false; //Aボタンが押されたか
};

