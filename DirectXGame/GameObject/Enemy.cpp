#include "Enemy.h"
#include "SphereCollider.h"
#include "GamePlayScene.h"
#include "string.h"

//�f�X�g���N�^
Enemy::~Enemy()
{
	delete enemyModel;
}

//������
void Enemy::EnemyInitialize()
{
	Initialize();
	// OBJ���烂�f���f�[�^��ǂݍ���
	switch (enemyNum_)
	{
	case 1:
	enemyModel = Model::LoadFromOBJ("enemy01");
	break;
	case 2:
	enemyModel = Model::LoadFromOBJ("enemy02");
	break;
	}
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(enemyModel);

	//�t���O
	isDead_ = false;
	isDelete_ = false;

	//�^�C�}�[
	turnTimer_ = 0;
	deleteTimer_ = 0;
}

void Enemy::Update()
{
	if (isBack_ == false)
	{
		//�f�X�|�[���^�C�}�[��200�ȏ�Ȃ�
		if (deleteTimer_ >= 400)
		{
			isBack_ = true;
			worldTransform_.rotation_.y = 0.0f;
			turnTimer_ = 0.0f;
		}
		else
		{
			//�C�[�W���O���g������]
			worldTransform_.rotation_.y = 360.0f * -MathFunc::easeOutSine(turnTimer_ / 20.0f);
			//�f�X�|�[���^�C�}�[�̏���
			deleteTimer_++;
			if (turnTimer_ <= 19.0f)
			{
				turnTimer_++;
			}
		}
	}
	//�ޏꉉ�o�t���O��true�ɂȂ�����
	else
	{
		//�C�[�W���O���g������](�o��Ƃ͋t)
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

	// ���[���h�g�����X�t�H�[���̍s��X�V�Ɠ]��
	worldTransform_.UpdateMatrix();
}

void Enemy::ColliderUpdate()
{
	//�����蔻��X�V
	if (collider)
	{
		collider->Update();
	}
}

void Enemy::OnCollision(const CollisionInfo& info)
{
	//�e�ɓ���������
	const char* str1 = "class PlayerBullet";

	//���肪playerBullet
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
