#pragma once
#include "Transform.h"
#include <vector>

class DirectXCommon;
class Sprite;
class Model;
class SphereModel;
class Texture;
class Camera;
class PlayerCamera;
class BirdEyeCamera;
class LightList;
class Input;
class Player;
class Map;
class Crosshair;
class ParticleList;
class Game;
class RotateEnemy;
class Gem;
class Star;
class MoveFloor;
class SlideFloor;
class Switch;
class UpFloor;

class GamePlayScene
{
public:
	GamePlayScene(Game *game);
	void Initialize(DirectXCommon* dxCommon);
	void Finalize();
	void Update(Input* input);
	void Draw(DirectXCommon* dxCommon);

private:
	PlayerCamera* mPlayerCamera;
	BirdEyeCamera* mBirdEyeCamera;
	LightList* lightList;
	Player* mPlayer;
	Map* mMap;
	Crosshair* mCrosshair;
	ParticleList* mParticle;
	Game* mGame;

	//敵
	const int mROTATEENEMY_MAX = 1;
	std::vector<RotateEnemy*> mRotateEnemies;
	//アイテム
	std::vector<Gem*> mGems;
	Star* mStar;
	//動く床
	SlideFloor* mSlideFloor;
	std::unique_ptr<SlideFloor> mSlideFloor;
	//スイッチ
	std::unique_ptr<Switch> mSlideSwitch;
	//スイッチを押すと上下に動く床
	std::unique_ptr<UpFloor> mUpFloor;
	//upスイッチ
	std::unique_ptr<Switch> mUpSwitch;
	//upスイッチモデル
	std::unique_ptr<Model> mUpSwitchModel;

	//フラグ
	bool mIsDirectionalLight = true;
	bool mIsPlayerCamera = false;
	bool mIsHit = false;
};

