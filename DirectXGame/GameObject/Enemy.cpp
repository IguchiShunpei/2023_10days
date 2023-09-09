#include "Enemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"

//デストラクタ
Enemy::~Enemy()
{
	delete enemyModel;
}

//初期化
void Enemy::EnemyInitialize()
{
	Initialize();
	// OBJからモデルデータを読み込む
	switch (enemyNum_)
	{
	case 1:
	enemyModel = Model::LoadFromOBJ("enemy01");
	break;
	case 2:
	enemyModel = Model::LoadFromOBJ("enemy02");
	break;
	}
	// 3Dオブジェクト生成
	Create();
	// オブジェクトにモデルをひも付ける
	SetModel(enemyModel);

	//フラグ
	isDead_ = false;
	isDelete_ = false;

	//タイマー
	turnTimer_ = 0;
	deleteTimer_ = 0;
}

void Enemy::Update()
{
	if (isBack_ == false)
	{
		//デスポーンタイマーが200以上なら
		if (deleteTimer_ >= 400)
		{
			isBack_ = true;
			worldTransform_.rotation_.y = 0.0f;
			turnTimer_ = 0.0f;
		}
		else
		{
			//イージングを使った回転
			worldTransform_.rotation_.y = 360.0f * -MathFunc::easeOutSine(turnTimer_ / 20.0f);
			//デスポーンタイマーの処理
			deleteTimer_++;
			if (turnTimer_ <= 19.0f)
			{
				turnTimer_++;
			}
		}
	}
	//退場演出フラグがtrueになったら
	else
	{
		//イージングを使った回転(登場とは逆)
		worldTransform_.rotation_.y = 360.0f * MathFunc::easeOutSine(turnTimer_ / 20.0f);
		worldTransform_.scale_ -= Vector3({ 0.04f,0.04f,0.04f });
		if (turnTimer_ <= 19.0f)
		{
			turnTimer_++;
		}
		else
		{
			isDelete_ = true;
		}
	}

	// ワールドトランスフォームの行列更新と転送
	worldTransform_.UpdateMatrix();
}

void Enemy::ColliderUpdate()
{
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

void Enemy::OnCollision(const CollisionInfo& info)
{
	//弾に当たったら
	const char* str1 = "class PlayerBullet";

	//相手がplayerBullet
	if (strcmp(toCollisionName, str1) == 0)
	{
		isDead_ = true;
	}
}

Vector3 Enemy::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.position_.x;
	worldPos.y = worldTransform_.position_.y;
	worldPos.z = worldTransform_.position_.z;

	return worldPos;
}
