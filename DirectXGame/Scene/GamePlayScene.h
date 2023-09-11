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
	Sprite* cross = nullptr;
	Sprite* numbers[10];

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

	//フラグ
	//敵を発生させるときの待機フラグ
	bool isWait_;

	//タイマー
	//敵を発生させるときの待機時間
	int waitTimer_;
};
