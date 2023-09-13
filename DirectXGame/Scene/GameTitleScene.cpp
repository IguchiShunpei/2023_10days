#include "GameTitleScene.h"

using namespace DirectX;

GameTitleScene::GameTitleScene()
{
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
	titleBGM->SoundLoadWave("Resources/Sound/TitleBGM.wav");
	titleBGM->SoundPlayWave(true, 1.0f);
}

void GameTitleScene::Update()
{
	// �V�[���̐؂�ւ�
	if (input_->TriggerMouseLeft() == true) {
		// �Q�[���v���C�V�[���i���V�[���j�𐶐�
		GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		titleBGM->StopWave();
	}

	//�V��
	sky->Update();
	viewProjection->UpdateMatrix();

	Vector3 cur = input_->GetMousePos();
	cross->SetPosition({ cur.x - 24,cur.y - 24,0 });
	cross->Update();

}

void GameTitleScene::Draw()
{
	Object3d::PreDraw(dxCommon_->GetCommandList());

	sky->Draw(viewProjection);

	Object3d::PostDraw();

	cross->SetTextureCommands(0, dxCommon_);
	cross->Draw(dxCommon_);
}

void GameTitleScene::Finalize()
{
	// �T�E���h�̉��
	delete titleBGM;
}
