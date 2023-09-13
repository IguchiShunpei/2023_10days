#pragma once

#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "GameClearScene.h"
#include "SIFrameWork.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include "SkyDome.h"
#include "LevelLoader.h"
#include "Meteor.h"
#include "Enemy.h"
#include "Sound.h"
#include "PostEffect.h"

#include<cassert>
#include<vector>
#include<string>
#include<d3dcompiler.h>
#include <sstream>
#include <map>

#include<DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

struct LevelData;

class CollisionManager;

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene : public GameBaseScene
{
public:
	GamePlayScene();
	~GamePlayScene();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dXCommon"></param>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 解放
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ゲームのリセット
	/// </summary>
	void GameReset();

/// <summary>
///   射撃
/// </summary>
	void Shot();
	
	void ScoreCalc();

/// <summary>
/// 座標変換
/// </summary>
	Vector3 GetWorldToScreenPos(Vector3 pos_, ViewProjection* viewProjection_);

	//敵リスト
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_01; }

	//敵データ読み込み
	void LoadEnemyPop();
	void UpdateEnemyPop();

private://メンバ変数
	//背景
	SkyDome* sky;
	//入力
	Input* input = nullptr;
	//DxCommon
	DirectXCommon* dxCommon = nullptr;

	//カメラ
	ViewProjection* viewProjection = nullptr;
	XMViewProjection* xmViewProjection = nullptr;

	//敵発生コマンド
	std::stringstream enemyPopCommands;
	//敵
	Enemy* enemy_01;
	Enemy* enemy_02;
	Enemy* enemy_03;
	Enemy* enemy_04;

	//敵
	std::list<std::unique_ptr<Enemy>> enemys_01;
	std::list<std::unique_ptr<Enemy>> enemys_02;
	std::list<std::unique_ptr<Enemy>> enemys_03;
	std::list<std::unique_ptr<Enemy>> enemys_04;

	//座標
	WorldTransform* worldTransform = nullptr;
	//当たり判定
	CollisionManager* collisionManager = nullptr;

	//パーティクル
	Particle* p_dmg = nullptr;
	ParticleManager* pm_dmg = nullptr;
	Particle* pEffect01 = nullptr;
	Particle* pEffect02 = nullptr;
	Particle* pEffect03 = nullptr;
	Particle* pEffect04 = nullptr;
	ParticleManager* pmEffect01 = nullptr;
	ParticleManager* pmEffect02 = nullptr;
	ParticleManager* pmEffect03 = nullptr;
	ParticleManager* pmEffect04 = nullptr;
	Sprite* cross = nullptr;
	Sprite* onesPlace[10];
	Sprite* tensPlace[10];
	Sprite* hundredPlace[10];
	Sprite* thousandPlace[10];
	Sprite* tenthousandPlace[10];
	Sprite* hundredthousandPlace[10];
	Sprite* getGold = nullptr;
	Sprite* getNormal[10];
	Sprite* getRed[5];
	Sprite* getBlue[5];
	//ロゴやUI
	Sprite* titleLogo = nullptr;

	//Sound
	Sound* gameBGM = nullptr;
	Sound* finishSE = nullptr;
	Sound* shotSE = nullptr;
	Sound* getSE = nullptr;
	Sound* highGetSE = nullptr;
	Sound* superHighGetSE = nullptr;
	Sound* missSE = nullptr;

	// ポストエフェクト
	PostEffect* postEffect = nullptr;

	//得点演出管理
	bool isGetGold;
	bool isGetNormal[10];
	bool isGetBlue[5];
	bool isGetRed[5];
	int goldTime;
	int normalTime[10];
	int redTime[5];
	int blueTime[5];

	//シーン切り替え
	Sprite* sceneEffect1[120];
	Sprite* sceneEffect2[120];
	int effectTime;
	bool isShow1[120];
	bool isShow2[120];
	bool effectSwitch;
	bool isPlayable;


	//背景オブジェクト
	Meteor* objMeteor = nullptr;
	Model* modelMeteor = nullptr;

	//レベルデータ
	LevelData* levelData = nullptr;
	//モデル
	std::map<std::string, Model*> meteorModels;
	//オブジェクト
	std::vector<Meteor*> meteorObjects;

	//スコア
	int score_;
	int scores[6];
	int oldScores[6];

	//フラグ
	//敵を発生させるときの待機フラグ
	bool isWait_;
	//ゲーム終了
	bool isFinish_;

	//タイマー
	//敵を発生させるときの待機時間
	int waitTimer_;
	int finishTimer_;

	// エフェクト
	bool isBlur = false;
	int blurTimer = 0;
};
