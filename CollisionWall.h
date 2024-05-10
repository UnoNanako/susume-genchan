#pragma once
#include "Engine//GameObject.h"
#include "Transform.h"

struct AABB;

class CollisionWall
{
public:
	void Initialize();
	void Update();

	//translateとscaleを渡すとmin,maxを求めてくれる関数
	AABB CalcurateAABB(const Vector3& translate, const Vector3& scale);
	/// <summary>
	/// アクセッサ
	/// </summary>
	Transform GetTransform() { return mTransform; }
	Vector3 GetTranslate() { return mTransform.translate; }
	
private:
	AABB mAABB;
	Transform mTransform;
};

