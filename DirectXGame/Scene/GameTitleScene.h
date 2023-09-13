#pragma once

#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "GamePlayScene.h"
#include "SkyDome.h"
#include "viewProjection.h"
#include "Sound.h"

#include <DirectXMath.h>

class GameTitleScene : public GameBaseScene
{
public: // メンバ関数
	// コンストラクタ
	GameTitleScene();

	// デストラクタ
	~GameTitleScene();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="dXCommon"></param>
	void Draw() override;

	/// <summary>
	/// 解放
	/// </summary>
	void Finalize() override;

private: // メンバ変数
	// 入力
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	
	//背景
	SkyDome* sky;

	//カメラ
	ViewProjection* viewProjection = nullptr;

	//カーソル
	Sprite* cross = nullptr;
	//ロゴやUI
	Sprite* titleLogo = nullptr;
	Sprite* click01 = nullptr;
	Sprite* click02 = nullptr;
	//シーン切り替え
	Sprite* sceneEffect1[120];
	Sprite* sceneEffect2[120];

	Sprite* loading = nullptr;

	int effectTime;
	bool isShow1[120];
	bool isShow2[120];
	bool effectSwitch;
	bool isNext;
	bool curHit;
	bool isUp;
	float logoTime;
	float logoY;

	//Sound
	Sound* titleBGM = nullptr;
	Sound* startSE = nullptr;
	Sound* shotSE = nullptr;
};
