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
	enemyModel = Model::LoadFromOBJ("enemy");
	// 3D�I�u�W�F�N�g����
	Create();
	// �I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	SetModel(enemyModel);
	
	//�t���O
	isDead_ = false;
	isDelete_ = false;

	//�^�C�}�[
	deleteTimer_ = 90;
}

void Enemy::Update()
{
	deleteTimer_--;
	if (deleteTimer_ <= 0)
	{
		isDelete_ = true;
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
