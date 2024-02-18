#pragma once

class DirectXCommon;
class Sprite;
class Model;
class SphereModel;
class Texture;
class Camera;
class BirdEyeCamera;
class LightList;
class Input;
class Player;
class Map;
class Crosshair;
class ParticleList;
class Game;

class GamePlayScene
{
public:
	GamePlayScene(Game *game);
	void Initialize(DirectXCommon* dxCommon);
	void Finalize();
	void Update(Input* input);
	void Draw(DirectXCommon* dxCommon);

private:
	Camera* camera;
	BirdEyeCamera* mBirdEyeCamera;
	LightList* lightList;
	Player* mPlayer;
	Map* mMap;
	Crosshair* mCrosshair;
	ParticleList* mParticle;
	Game* mGame;

	//フラグ
	bool mIsDirectionalLight = false;
	bool mIsPlayerCamera = false;
};

