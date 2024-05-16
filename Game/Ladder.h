#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <d3d12.h>

struct AABB;

class Ladder : public GameObject{
public:
	enum Direction {
		FRONT,
		BACK,
		LEFT,
		RIGHT
	};
	~Ladder();
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, Camera* camera);
	AABB CalcurateAABB(const Vector3& translate);

	/// <summary>
	/// アクセッサ
	/// </summary>
	AABB GetAABB() { return mAABB; }
	void SetModel(Model* model) { mModel = model; }
	void SetTexture(Texture* texture) { mTexture = texture; }
	void SetIsHit(bool isHit) { mIsHit = isHit; }
	void SetVec(Vector3 vec) { mVec = vec; }
	bool GetIsHit() { return mIsHit; }
	Vector3 GetVec() { return mVec; }
	void SetDirection(Direction direction) { mDirection = direction; }
	Direction GetDirection() { return mDirection; }
	void SetHeight(float height) { mHeight = height; }
	float GetHeight() { return mHeight; }
private:
	AABB mAABB;
	bool mIsHit = false;
	Vector3 mVec;
	Direction mDirection;
	float mHeight;
};

