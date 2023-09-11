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
/// �Q�[���v���C�V�[��
/// </summary>
class GamePlayScene : public GameBaseScene
{
public:
	GamePlayScene();
	~GamePlayScene();

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dXCommon"></param>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ���
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �Q�[���̃��Z�b�g
	/// </summary>
	void GameReset();

/// <summary>
///   �ˌ�
/// </summary>
	void Shot();

/// <summary>
/// ���W�ϊ�
/// </summary>
	Vector3 GetWorldToScreenPos(Vector3 pos_, ViewProjection* viewProjection_);

	//�G���X�g
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_01; }

	//�G�f�[�^�ǂݍ���
	void LoadEnemyPop();
	void UpdateEnemyPop();

private://�����o�ϐ�
	//�w�i
	SkyDome* sky;
	//����
	Input* input = nullptr;
	//DxCommon
	DirectXCommon* dxCommon = nullptr;

	//�J����
	ViewProjection* viewProjection = nullptr;
	XMViewProjection* xmViewProjection = nullptr;

	//�G�����R�}���h
	std::stringstream enemyPopCommands;
	//�G
	Enemy* enemy_01;
	Enemy* enemy_02;
	Enemy* enemy_03;
	Enemy* enemy_04;

	//�G
	std::list<std::unique_ptr<Enemy>> enemys_01;
	std::list<std::unique_ptr<Enemy>> enemys_02;
	std::list<std::unique_ptr<Enemy>> enemys_03;
	std::list<std::unique_ptr<Enemy>> enemys_04;

	//���W
	WorldTransform* worldTransform = nullptr;
	//�����蔻��
	CollisionManager* collisionManager = nullptr;

	//�p�[�e�B�N��
	Particle* p_dmg = nullptr;
	ParticleManager* pm_dmg = nullptr;
	Sprite* cross = nullptr;
	Sprite* numbers[10];

	//�w�i�I�u�W�F�N�g
	Meteor* objMeteor = nullptr;
	Model* modelMeteor = nullptr;

	//���x���f�[�^
	LevelData* levelData = nullptr;
	//���f��
	std::map<std::string, Model*> meteorModels;
	//�I�u�W�F�N�g
	std::vector<Meteor*> meteorObjects;

	//�X�R�A
	int score_;

	//�t���O
	//�G�𔭐�������Ƃ��̑ҋ@�t���O
	bool isWait_;

	//�^�C�}�[
	//�G�𔭐�������Ƃ��̑ҋ@����
	int waitTimer_;
};
