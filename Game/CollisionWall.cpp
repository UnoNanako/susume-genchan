#include "CollisionWall.h"
#include "Math/MyMath.h"

void CollisionWall::Initialize()
{
	mTransform = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
}

void CollisionWall::Update()
{
	mAABB = CalcurateAABB(mTransform.translate, mTransform.scale);
	mTransform.UpdateMatrix();
}

AABB CollisionWall::CalcurateAABB(const Vector3& translate, const Vector3& scale)
{
	AABB ret;
	ret.min = {
		{translate.x - (scale.x / 2.0f)},
		{translate.y - (scale.y / 2.0f)},
		{translate.z - (scale.z / 2.0f)}
	};
	ret.max = {
		{translate.x + (scale.x / 2.0f)},
		{translate.y + (scale.y / 2.0f)},
		{translate.z + (scale.z / 2.0f)}
	};
	return ret;
}
