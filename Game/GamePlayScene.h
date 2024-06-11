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
#include "Game/Ghost.h"
#include "Game/Gem.h"
#include "Game/Star.h"
#include "Game/Grass.h"
#include "Game/MoveFloor.h"
#include "Game/SlideFloor.h"
#include "Game/Switch.h"
#include "Game/Crank.h"
#include "Game/RotateFloor.h"
#include "Game/UpFloor.h"
#include "Game/BirdEyeCamera.h"
#include "Game/PlayerCamera.h"
#include "Game/Skydome.h"
#include "Game/Ladder.h"
#include "Engine/Light/LightList.h"
#include "Game/CollisionWall.h"
#include "Game/TitleScene.h"
#include "Game/MiniBridge.h"
#include "Game/Fence.h"

class DirectXCommon;
class Sprite;
class Model;
class SphereModel;
class Texture;
class Camera;
class Input;

class GamePlayScene{
public:
	GamePlayScene(Game* game);
	void Initialize(DirectXCommon* dxCommon);
	void Finalize();
	void Update(Input* input);
	void Draw(DirectXCommon* dxCommon);

private:
	void LadderInitialize(DirectXCommon* dxCommon);
	void ObjectUpdate(Input* input);
	void Collision(Input* input);

	Game* mGame;
	std::unique_ptr<PlayerCamera> mPlayerCamera;
	std::unique_ptr<BirdEyeCamera> mBirdEyeCamera; //俯瞰カメラ
	std::unique_ptr<LightList> mLightList; //ライト

	/// <summary>
	/// プレイヤー
	/// </summary>
	std::unique_ptr<Player> mPlayer;

	/// <summary>
	/// 敵
	/// </summary>
	//回転するだけの敵
	const uint32_t mROTATEENEMY_MAX = 1;
	std::vector<std::unique_ptr<RotateEnemy>> mRotateEnemies;
	//歩く敵
	const uint32_t mWALKENEMY_MAX = 3;
	std::vector<std::unique_ptr<WalkEnemy>> mWalkEnemies;
	//幽霊(テレサ)
	const uint32_t mGHOST_MAX = 1;
	std::vector<std::unique_ptr<Ghost>> mGhosts;

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
	//クランク
	std::unique_ptr<Crank> mCrank;
	//クランクを回すと回る床
	std::unique_ptr<RotateFloor> mRotateFloor;

	/// <summary>
	/// アイテム
	/// </summary>
	std::vector <std::unique_ptr<Gem>> mGems;
	std::unique_ptr<Star> mStar;

	/// <summary>
	/// 当たり判定の壁(透明)
	/// <summary>
	const uint32_t mCOLLISIONWALL_MAX = 8;
	std::vector<std::unique_ptr<CollisionWall>> mCollisionWalls;

	/// <summary>
	/// オブジェクト
	/// </summary>
	std::unique_ptr<Map> mMap; //マップ
	std::unique_ptr<Skydome> mSkydome; //天球
	//はしご
	const uint32_t mLADDER_MAX = 5;
	std::vector<std::unique_ptr<Ladder>> mLadders;
	//はしご用のモデル、テクスチャ
	std::unique_ptr<Model> mLadderModel0; //離島にあるはしご
	std::unique_ptr<Model> mLadderModel_height15_01; //縦15(3マス分)のはしご
	std::unique_ptr<Model> mLadderModel_height15_02; //縦15のはしご2つ目
	std::unique_ptr<Model> mLadderModel_height15_03; //離島1つめのはしご
	std::unique_ptr<Model> mLadderModel_height05; //離島高さ1のはしご
	//草
	const uint32_t mGRASS_MAX = 1;
	std::vector<std::unique_ptr<Grass>> mGrasses;
	//小さい橋
	const uint32_t mMINIBRIDGE_MAX = 3;
	std::vector<std::unique_ptr<MiniBridge>> mMiniBridges;
	//フェンス
	const uint32_t mFENCE_MAX = 13;
	std::vector<std::unique_ptr<Fence>> mFences;

	/// <summary>
	/// スプライト
	/// <summary>
	std::unique_ptr<Crosshair> mCrosshair; //クロスヘア
	std::unique_ptr<Sprite> mClearSprite; //クリアスプライト
	std::unique_ptr<Sprite> mGameoverSprite; //ゲームオーバースプライト

	/// <summary>
	/// シーン
	/// <summary>
	std::unique_ptr<TitleScene> mTitleScene;

	/// <summary>
	/// フラグ
	/// </summary>
	bool mIsDirectionalLight = true;
	bool mIsPlayerCamera = false;
	bool mSlideFloorIsHit = false;
	bool mUpFloorIsHit = false;
	bool mSwitchIsHit = false; //UI(Aボタン)を出すために使う
	bool mIsClear = false;
	bool mIsGameover = false;
	bool mLadderIsHit = false; //はしごに当たっているか
	bool mIsTitleScene = true; //タイトルシーンか、そうでないか
};

