#pragma once

#include "Sprite.h"

// 前方宣言
class GameSceneManager;

// シーン基底クラス
class GameBaseScene
{
public: // メンバ関数
	GameBaseScene() = default;
	virtual ~GameBaseScene() = default;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 解放
	/// </summary>
	virtual void Finalize() = 0;

	int GetScore() { return score_; }
	void SetScore(int score) { score_ = score; }

public: // メンバ関数
	virtual void SetSceneManager(GameSceneManager* gSceneManager) { gSceneManager_ = gSceneManager; }

private: // メンバ変数
	// シーンマネージャ（借りてくる）
	GameSceneManager* gSceneManager_ = nullptr;
	//スコア
	int score_;
};

