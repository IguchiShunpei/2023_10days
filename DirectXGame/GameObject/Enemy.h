#pragma once
#include "Object3d.h"
#include "WinApp.h"
#include "Vector3.h"
#include "Model.h"
#include "MathFunc.h"

#include <memory>
#include <list>

//GameScene�̑O���錾
class GamePlayScene;

class Enemy : public Object3d
{
public:
	//�f�X�g���N�^
	~Enemy();
	//������
	void EnemyInitialize();

	//�X�V
	void Update();

	//�����蔻��X�V
	void ColliderUpdate();
	
	//�����蔻��R�[���o�b�N
	void OnCollision(const CollisionInfo& info) override;

	//���[���h���W���擾
	Vector3 GetPosition();
	Vector3 GetRotation() { return worldTransform_.rotation_; }
	bool GetIsDead() { return isDead_; }
	bool GetIsDelete() { return isDelete_; }
	int GetEnemyNum() { return enemyNum_; }

	void SetRotation(Vector3 rot) { worldTransform_.rotation_ = rot; }
	void SetScale(Vector3 scale) { worldTransform_.scale_ = scale; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }
	void SetEnemyNum(int enemyNum) { enemyNum_ = enemyNum; }

private:
	//�Q�[���V�[��
	GamePlayScene* gameScene_ = nullptr;

	//�G�ԍ�
	int enemyNum_;

	//�f�X�t���O
	//�|���ꂽ��
	bool isDead_ = false;
	//�f�X�|�[��������
	bool isDelete_ = false;
	//�ޏꉉ�o�p�t���O
	bool isBack_ = false;

	// ���f��
	Model* enemyModel = nullptr;

	//�^�C�}�[
	//�f�X�|�[��
	int deleteTimer_;
	//�o�ޏ�̉�]�^�C�}�[
	float turnTimer_;
};
