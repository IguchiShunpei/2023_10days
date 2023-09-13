#pragma once

#include "Input.h"
#include "DirectXCommon.h"
#include "GameBaseScene.h"
#include "Sprite.h"
#include "GameSceneManager.h"
#include "GameTitleScene.h"
#include "SkyDome.h"
#include "Sound.h"

#include <DirectXMath.h>

class GameClearScene : public GameBaseScene
{
public: // メンバ関数
	// コンストラクタ
	GameClearScene();
	// デストラクタ
	~GameClearScene();

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
	DirectXCommon* dxCommon = nullptr;
	Sprite* cross = nullptr;
	SkyDome* sky;
	//カメラ
	ViewProjection* viewProjection = nullptr;
	//score
	Sprite* scoreGraph = nullptr;
	Sprite* onesPlace[10];
	Sprite* tensPlace[10];
	Sprite* hundredPlace[10];
	Sprite* thousandPlace[10];
	Sprite* tenthousandPlace[10];
	Sprite* hundredthousandPlace[10];
	int scores[6];
	float numPosX;
	int showTime;
	//click
	Sprite* click01 = nullptr;
	Sprite* click02 = nullptr;
	bool isNext;
	bool curHit;
	//SE
	Sound* startSE = nullptr;
	Sound* shotSE = nullptr;
};
