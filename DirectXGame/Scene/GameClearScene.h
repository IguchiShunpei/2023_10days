#pragma once

#include "Input.h"
#include "DirectXCommon.h"
#include "GameBaseScene.h"
#include "Sprite.h"
#include "GameSceneManager.h"
#include "GameTitleScene.h"
#include "SkyDome.h"
#include "Sound.h"

#include <DirectXMath.h>

class GameClearScene : public GameBaseScene
{
public: // �����o�֐�
	// �R���X�g���N�^
	GameClearScene();
	// �f�X�g���N�^
	~GameClearScene();

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
	DirectXCommon* dxCommon = nullptr;
	Sprite* cross = nullptr;
	SkyDome* sky;
	//�J����
	ViewProjection* viewProjection = nullptr;
	//score
	Sprite* scoreGraph = nullptr;
	Sprite* onesPlace[10];
	Sprite* tensPlace[10];
	Sprite* hundredPlace[10];
	Sprite* thousandPlace[10];
	Sprite* tenthousandPlace[10];
	Sprite* hundredthousandPlace[10];
	int scores[6];
	float numPosX;
	int showTime;
	//click
	Sprite* click01 = nullptr;
	Sprite* click02 = nullptr;
	bool isNext;
	bool curHit;
	//SE
	Sound* startSE = nullptr;
	Sound* shotSE = nullptr;
};
