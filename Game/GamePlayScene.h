#pragma once
#include "Transform.h"
#include <vector>
#include "Game/Player.h"
#include "Game/Map.h"
#include "Game/Crosshair.h"
#include "Engine/Particle/ParticleList.h"
#include "Game.h"
#include "Game/RotateEnemy.h"
#include "Game/WalkEnemy.h"
#include "Game/Gem.h"
#include "Game/Star.h"
#include "Game/Grass.h"
#include "Game/MoveFloor.h"
#include "Game/SlideFloor.h"
#include "Game/Switch.h"
#include "Game/UpFloor.h"
#include "Game/BirdEyeCamera.h"
#include "Game/PlayerCamera.h"
#include "Game/Skydome.h"
#include "Engine/Light/LightList.h"
	
class DirectXCommon;
class Sprite;
class Model;
class SphereModel;
class Texture;
class Camera;
class Input;

class GamePlayScene
{
public:
	GamePlayScene(Game *game);
	void Initialize(DirectXCommon* dxCommon);
	void Finalize();
	void Update(Input* input);
	void Draw(DirectXCommon* dxCommon);

private:
	Game* mGame;

	std::unique_ptr<PlayerCamera> mPlayerCamera;
	std::unique_ptr<BirdEyeCamera> mBirdEyeCamera;
	std::unique_ptr<LightList> mLightList;
	std::unique_ptr<Player> mPlayer;
	std::unique_ptr<Map> mMap;
	std::unique_ptr<Crosshair> mCrosshair;
	std::unique_ptr<ParticleList> mParticle;
	std::unique_ptr<Skydome> mSkydome; //天球

	//敵
	const int mROTATEENEMY_MAX = 1;
	std::vector<std::unique_ptr<RotateEnemy>> mRotateEnemies;
	const int mWALKENEMY_MAX = 1;
	std::vector<std::unique_ptr<WalkEnemy>> mWalkEnemies;
	//アイテム
	std::vector <std::unique_ptr<Gem>> mGems;
	std::unique_ptr<Star> mStar;
	//草
	const int mGRASS_MAX = 1;
	std::vector<std::unique_ptr<Grass>> mGrasses;
	
	/// <summary>
	/// ギミック
	/// </summary>
	//動く床
	std::unique_ptr<SlideFloor> mSlideFloor;
	//スイッチ
	std::unique_ptr<Switch> mSlideSwitch;
	//スイッチを押すと上下に動く床
	std::unique_ptr<UpFloor> mUpFloor;
	//upスイッチ
	std::unique_ptr<Switch> mUpSwitch;

	//フラグ
	bool mIsDirectionalLight = true;
	bool mIsPlayerCamera = false;
	bool mSlideFloorIsHit = false;
	bool mUpFloorIsHit = false;
	bool mSwitchIsHit = false; //UI(Aボタン)を出すために使う
	bool mIsClear = false;
	bool mIsGameover = false;

	//UI
	std::unique_ptr<Sprite> mAbuttonSprite;
	
	//テキスト
	std::unique_ptr<Sprite> mClearSprite;
	std::unique_ptr<Sprite> mGameoverSprite;
};

