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
class MovingFloor;

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
	MovingFloor* mMovingFloor;

	//フラグ
	bool mIsDirectionalLight = true;
	bool mIsPlayerCamera = false;

	//BirdEyeCameraの回転角度と距離
	float angle = 0.0f;
	float distance = 5.0f;
};

