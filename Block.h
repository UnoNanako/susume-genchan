#pragma once
#include "Engine/Transform.h"

class Model;
struct AABB;

struct Block {
	Model* mModel;
	Transform mTransform;
	AABB mAABB;
};