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
	titleLogo = new Sprite;
	titleLogo->Initialize(dxCommon_);
	titleLogo->LoadTexture(0, L"Resources/TITLE.png", dxCommon_);
	titleLogo->SetScale({ 8,5 });
	click = new Sprite;
	click->Initialize(dxCommon_);
	click->LoadTexture(0, L"Resources/CLICKHERE.png", dxCommon_);
	click->SetScale({ 4,1 });

	// �T�E���h�̏�����
	titleBGM = new Sound;
	titleBGM->SoundLoadWave("Resources/Sound/titleBGM.wav");
	titleBGM->SoundPlayWave(true, 1.0f);
	startSE = new Sound;
	startSE->SoundLoadWave("Resources/Sound/start.wav");
	shotSE = new Sound;
	shotSE->SoundLoadWave("Resources/Sound/shot.wav");
}

void GameTitleScene::Update()
{
	//�V��
	sky->Update();
	viewProjection->UpdateMatrix();

	//�J�[�\��
	Vector3 cur = input_->GetMousePos();
	cross->SetPosition({ cur.x - 24,cur.y - 24,0 });
	cross->Update();

	//���S��UI
	titleLogo->SetPosition({ 240,10,0 });
	titleLogo->Update();
	click->SetPosition({ 450,520,0 });
	click->Update();

	// �V�[���̐؂�ւ�
	if (input_->TriggerMouseLeft() == true) {
		if (cur.x >= click->GetPosition().x && cur.x <= click->GetPosition().x + 300)
		{
			if (cur.y >= click->GetPosition().y && cur.y <= click->GetPosition().y + 100)
			{
				// �Q�[���v���C�V�[���i���V�[���j�𐶐�
				GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
				startSE->SoundPlayWave(false, 1.0f);
				titleBGM->StopWave();
			}
			else
			{
				shotSE->SoundPlayWave(false, 1.0f);
			}
		}
		else
		{
			shotSE->SoundPlayWave(false, 1.0f);
		}
	}
}

void GameTitleScene::Draw()
{
	Object3d::PreDraw(dxCommon_->GetCommandList());

	sky->Draw(viewProjection);

	Object3d::PostDraw();

	titleLogo->SetTextureCommands(0, dxCommon_);
	titleLogo->Draw(dxCommon_);
	click->SetTextureCommands(0, dxCommon_);
	click->Draw(dxCommon_);
	cross->SetTextureCommands(0, dxCommon_);
	cross->Draw(dxCommon_);
}

void GameTitleScene::Finalize()
{

}
