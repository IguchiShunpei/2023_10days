#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include <memory>
#include <list>

//GameSceneの前方宣言
class GamePlayScene;

class Enemy : public Object3d
{
public:
	//デストラクタ
	~Enemy();
	//初期化
	void EnemyInitialize();

	//更新
	void Update();

	//当たり判定更新
	void ColliderUpdate();
	
	//当たり判定コールバック
	void OnCollision(const CollisionInfo& info) override;

	//ワールド座標を取得
	Vector3 GetPosition();
	Vector3 GetRotation() { return worldTransform_.rotation_; }
	bool GetIsDead() { return isDead_; }
	bool GetIsDelete() { return isDelete_; }

	void SetRotation(Vector3 rot) { worldTransform_.rotation_ = rot; }
	void SetScale(Vector3 scale) { worldTransform_.scale_ = scale; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }

private:
	//ゲームシーン
	GamePlayScene* gameScene_ = nullptr;

	//デスフラグ
	//倒されたか
	bool isDead_ = false;
	//デスポーンしたか
	bool isDelete_ = false;

	// モデル
	Model* enemyModel = nullptr;

	//タイマー
	int deleteTimer_;
};
