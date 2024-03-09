#include "Transform.h"

void Transform::UpdateMatrix()
{
	//スケール、回転、平行移動を合成して行列を計算する
	mMatWorld = MakeAffineMatrix(scale, rotate, translate);
	//親があれば親のワールド行列を掛ける
	if (mParent) {
		mMatWorld = Multiply(mMatWorld, mParent->mMatWorld);
	}
}
