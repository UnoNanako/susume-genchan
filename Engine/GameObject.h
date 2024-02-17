#pragma once
#include "Transform.h"
#include <d3d12.h>

using namespace std;

class DirectXCommon;
class Model;
class Texture;
class Camera;
class VertexData;

class GameObject
{
public:
	
protected:
	Model* mModel;
	Texture* mTexture;
	Transform mTransform;
	DirectXCommon* mDxCommon;
};

