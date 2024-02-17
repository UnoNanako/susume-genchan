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

class Game : public Framework
{
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	DirectXCommon* GetDxCommon() { return dxCommon; }
	ModelCommon* GetModelCommon() { return modelCommon; }
	ParticleCommon* GetParticleCommon() { return particleCommon; }
private:
	WinApiManager* winApiManager;
	Input* input;
	DirectXCommon* dxCommon;
	SpriteCommon* spriteCommon;
	ImGuiManager* imgui;
	GamePlayScene* scene_ = nullptr;
	ModelCommon* modelCommon;
	ParticleCommon* particleCommon;
};

