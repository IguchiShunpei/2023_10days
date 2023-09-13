#pragma once

#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "GamePlayScene.h"
#include "SkyDome.h"
#include "viewProjection.h"
#include "Sound.h"

#include <DirectXMath.h>

class GameTitleScene : public GameBaseScene
{
public: // �����o�֐�
	// �R���X�g���N�^
	GameTitleScene();

	// �f�X�g���N�^
	~GameTitleScene();

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="dXCommon"></param>
	void Draw() override;

	/// <summary>
	/// ���
	/// </summary>
	void Finalize() override;

private: // �����o�ϐ�
	// ����
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	
	//�w�i
	SkyDome* sky;

	//�J����
	ViewProjection* viewProjection = nullptr;

	//�J�[�\��
	Sprite* cross = nullptr;
	//�V�[���؂�ւ�
	Sprite* sceneEffect1[120];
	Sprite* sceneEffect2[120];
	int effectTime;
	bool isShow1[120];
	bool isShow2[120];
	bool effectSwitch;
	bool isNext;

	//Sound
	Sound* titleBGM = nullptr;
};
