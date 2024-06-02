#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <d3d12.h>

class Input;
class Texture;
class LightList;
struct AABB;
class MovingFloor;


class Player : public GameObject{
public:
	Player();
	~Player();
	void Initialize(DirectXCommon* dxCommon);
	void Update(Input* input, float theta);
	void Draw(ID3D12GraphicsCommandList* commandList,Camera* camera);
	AABB CalcurateAABB(const Vector3& translate);
	
	/// <summary>
	/// アクセッサ
	/// </summary>
	int GetHp() { return mHp; }
	bool GetIsEnemyHit() { return mIsEnemyHit; }
	bool GetIsAttack() { return mIsAttack; }
	AABB GetAABB() { return mAABBtranslate; }
	Vector3 GetVelocity() { return mVelocity; }
	Vector3 GetWorldPosition() { return Vector3{ mTransform.mMatWorld.m[3][0],mTransform.mMatWorld.m[3][1],mTransform.mMatWorld.m[3][2] }; }
	Matrix4x4 GetWorldMatrix() { return mTransform.mMatWorld; }
	Transform* GetParent() { return mTransform.mParent; }
	void SetHp(uint32_t hp) { mHp = hp; }
	void SetIsEnemyHit(bool isEnemyHit) { mIsEnemyHit = isEnemyHit; }
	void SetIsOperatable(bool isOperatable) { mIsOperatable = isOperatable; }
	void SetVelocity(Vector3 velocity) { mVelocity = velocity; }
	void SetIsHit(bool isHit) { mIsHit = isHit; }
	void SetLightList(LightList* lightList) { mLightList = lightList; }
	void SetMovingFloor(MovingFloor* movingFloor) { mMovingFloor = movingFloor; }
	//親となるトランスフォームをセット
	void SetParent(Transform* parent) { mTransform.mParent = parent; }
	void SetVelocityY(float y) { mVelocity.y = y; }
	void SetGravity(float gravity) { mGravity = gravity; }

private:
	uint32_t mHp; //最大2
	uint32_t mInvincibleTime; //無敵時間(2秒)
	Vector3 mVelocity;
	Texture* mTexture;
	LightList* mLightList;

	AABB mAABBtranslate; //当たり判定用
	bool mIsHit; //壁や床に当たったかどうかを管理するフラグ
	bool mIsEnemyHit; //敵と当たったかどうかを管理するフラグ
	bool mIsAttack; //攻撃しているかどうかを管理するフラグ
	bool mIsOperatable; //操作できるかどうかを管理するフラグ
	float mSpeed; 
	float mRotateSpeed; //回転スピード
	float mGravity; //重力
	int32_t mProgressTimer; //前進タイマー
	int32_t mBackTimer; //後退タイマー

	Matrix4x4 mTransposeViewMatrix;
	MovingFloor* mMovingFloor; //動く床
};

