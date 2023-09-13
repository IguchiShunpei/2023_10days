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
#include "Sound.h"
#include "PostEffect.h"

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
	
	void ScoreCalc();

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
	Particle* pEffect01 = nullptr;
	Particle* pEffect02 = nullptr;
	Particle* pEffect03 = nullptr;
	Particle* pEffect04 = nullptr;
	ParticleManager* pmEffect01 = nullptr;
	ParticleManager* pmEffect02 = nullptr;
	ParticleManager* pmEffect03 = nullptr;
	ParticleManager* pmEffect04 = nullptr;
	Sprite* cross = nullptr;
	Sprite* onesPlace[10];
	Sprite* tensPlace[10];
	Sprite* hundredPlace[10];
	Sprite* thousandPlace[10];
	Sprite* tenthousandPlace[10];
	Sprite* hundredthousandPlace[10];
	Sprite* getGold = nullptr;
	Sprite* getNormal[10];
	Sprite* getRed[5];
	Sprite* getBlue[5];
	//���S��UI
	Sprite* titleLogo = nullptr;

	//Sound
	Sound* gameBGM = nullptr;
	Sound* finishSE = nullptr;
	Sound* shotSE = nullptr;
	Sound* getSE = nullptr;
	Sound* highGetSE = nullptr;
	Sound* superHighGetSE = nullptr;
	Sound* missSE = nullptr;

	// �|�X�g�G�t�F�N�g
	PostEffect* postEffect = nullptr;

	//���_���o�Ǘ�
	bool isGetGold;
	bool isGetNormal[10];
	bool isGetBlue[5];
	bool isGetRed[5];
	int goldTime;
	int normalTime[10];
	int redTime[5];
	int blueTime[5];

	//�V�[���؂�ւ�
	Sprite* sceneEffect1[120];
	Sprite* sceneEffect2[120];
	int effectTime;
	bool isShow1[120];
	bool isShow2[120];
	bool effectSwitch;
	bool isPlayable;


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
	int scores[6];
	int oldScores[6];

	//�t���O
	//�G�𔭐�������Ƃ��̑ҋ@�t���O
	bool isWait_;
	//�Q�[���I��
	bool isFinish_;

	//�^�C�}�[
	//�G�𔭐�������Ƃ��̑ҋ@����
	int waitTimer_;
	int finishTimer_;

	// �G�t�F�N�g
	bool isBlur = false;
	int blurTimer = 0;
};
