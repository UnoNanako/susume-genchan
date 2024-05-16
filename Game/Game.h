#pragma once
#include "Framework.h"

class WinApiManager;
class DirectXCommon;
class ImGuiManager;
class Input;
class Sprite;
class SpriteCommon;
class Model;
class Texture;
class SphereModel;
class GamePlayScene;
class ModelCommon;
class ParticleCommon;

class Game : public Framework{
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	DirectXCommon* GetDxCommon() { return mDxCommon; }
	ModelCommon* GetModelCommon() { return mModelCommon; }
	ParticleCommon* GetParticleCommon() { return mParticleCommon; }
private:
	WinApiManager* mWinApiManager;
	Input* mInput;
	DirectXCommon* mDxCommon;
	SpriteCommon* mSpriteCommon;
	ImGuiManager* mImgui;
	GamePlayScene* mScene = nullptr;
	ModelCommon* mModelCommon;
	ParticleCommon* mParticleCommon;
};

