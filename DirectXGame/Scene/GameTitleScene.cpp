#include "GameTitleScene.h"

using namespace DirectX;

GameTitleScene::GameTitleScene()
{
	// �T�E���h�̐ÓI������
	Sound::StaticInitialize();
}

GameTitleScene::~GameTitleScene()
{
}

void GameTitleScene::Initialize()
{
	input_ = Input::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	//�J����������
	viewProjection = new ViewProjection();
	viewProjection->Initialize();

	//�V��
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//sprite
	cross = new Sprite;
	cross->Initialize(dxCommon_);
	cross->LoadTexture(0, L"Resources/cross.png", dxCommon_);
	cross->SetScale({ 0.5,0.5 });

	// �T�E���h�̏�����
	titleBGM = new Sound;
	titleBGM->SoundLoadWave("Resources/Sound/titleBGM.wav");
	titleBGM->SoundPlayWave(true, 1.0f);

	for (int i = 0; i < 120; i++) {
		sceneEffect1[i] = new Sprite;
		sceneEffect2[i] = new Sprite;

		sceneEffect1[i]->Initialize(dxCommon_);
		sceneEffect2[i]->Initialize(dxCommon_);

		sceneEffect1[i]->LoadTexture(0, L"Resources/black.png", dxCommon_);
		sceneEffect2[i]->LoadTexture(0, L"Resources/black.png", dxCommon_);

		sceneEffect1[i]->SetScale({ 0.64f,0.6f });
		sceneEffect2[i]->SetScale({ 0.64f,0.6f });
		isShow1[i] = false;
		isShow2[i] = false;

	}
	int num = 0;
	for (int j = 0; j < 12; j++) {
		for (int n = 0; n < 10; n++) {
			float posx = 0.0f;
			float posx2 = 64.0f;
			if (j % 2 == 1) {
				posx = 64.0f;
				posx2 = 0.0f;
			}
			float posy = j * 60.0f;
			//�z�u
			sceneEffect1[num]->SetPosition({ posx + (n * 128.0f),posy ,0.0f });
			sceneEffect2[num]->SetPosition({ posx2 + (n * 128.0f),posy ,0.0f });
			num++;
		}
	}
	effectSwitch = false;
	isNext = false;
}

void GameTitleScene::Update()
{
	// �V�[���̐؂�ւ�
	if (input_->TriggerMouseLeft() == true) {
		// �Q�[���v���C�V�[���i���V�[���j�𐶐�
		GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		startSE->SoundPlayWave(false, 1.0f);
		titleBGM->StopWave();
	}

	//�V��
	sky->Update();
	viewProjection->UpdateMatrix();

	Vector3 cur = input_->GetMousePos();
	cross->SetPosition({ cur.x - 24,cur.y - 24,0 });
	cross->Update();
	for (int i = 0; i < 120; i++) {
		sceneEffect1[i]->Update();
		sceneEffect2[i]->Update();
	}



	if (isNext == true) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 12; j++) {
				if (effectSwitch == false) {
					if (isShow1[i * 12 + j] == false) {
						isShow1[i * 12 + j] = true;
						if (i == 9 && j == 11) {
							effectSwitch = true;
						}
						break;
					}
				}
				if (effectSwitch == true) {
					if (isShow2[i * 12 + j] == false) {
						isShow2[i * 12 + j] = true;
						if (i == 9 && j == 11) {
							// �Q�[���v���C�V�[���i���V�[���j�𐶐�
							GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
							titleBGM->StopWave();
						}
						break;
					}
				}
			}
		}
	}
}

void GameTitleScene::Draw()
{
	Object3d::PreDraw(dxCommon_->GetCommandList());

	sky->Draw(viewProjection);

	Object3d::PostDraw();

	cross->SetTextureCommands(0, dxCommon_);
	cross->Draw(dxCommon_);
	//�V�[���`�F���W
	for (int i = 0; i < 120; i++) {
		sceneEffect1[i]->SetTextureCommands(0, dxCommon_);
		sceneEffect2[i]->SetTextureCommands(0, dxCommon_);
		if (isShow1[i] == true) {
			sceneEffect1[i]->Draw(dxCommon_);
		}
		if (isShow2[i] == true) {
			sceneEffect2[i]->Draw(dxCommon_);
		}
	}
}

void GameTitleScene::Finalize()
{
	
}
