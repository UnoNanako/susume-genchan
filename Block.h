#pragma once
#include "Engine/Transform.h"
#include "Game/Map.h"
#include <string>

class Model;
struct AABB;

struct Block {
	Model* mModel;
	Transform mTransform;
	AABB mAABB;
	std::string mName;
	Map::ModelIndex mModelIndex;
	AABB GetWorldAABB() {
		AABB ret;
		ret.max = mAABB.max + mTransform.translate;
		ret.min = mAABB.min + mTransform.translate;
		return ret;
	}
};