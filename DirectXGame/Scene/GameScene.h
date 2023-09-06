#pragma once
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

class GameScene : public SIFrameWork
{
public:
	//������
	void Initialize() override;
	//�I��
	void Finalize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;
	//���Z�b�g
	void Reset();
	//�G���X�g
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }

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
	Enemy* enemy;
	//�G
	std::list<std::unique_ptr<Enemy>> enemys_;

	//���W
	WorldTransform* worldTransform = nullptr;
	//�����蔻��
	CollisionManager* collisionManager = nullptr;

	//�p�[�e�B�N��
	Particle* p_dmg = nullptr;
	ParticleManager* pm_dmg = nullptr;

	//�w�i�I�u�W�F�N�g
	Meteor* objMeteor = nullptr;
	Model* modelMeteor = nullptr;

	//���x���f�[�^
	LevelData* levelData = nullptr;
	//���f��
	std::map<std::string, Model*> meteorModels;
	//�I�u�W�F�N�g
	std::vector<Meteor*> meteorObjects;

	//�t���O
	//�G�𔭐�������Ƃ��̑ҋ@�t���O
	bool isWait_;

	//�^�C�}�[
	//�G�𔭐�������Ƃ��̑ҋ@����
	int waitTimer_;
};
