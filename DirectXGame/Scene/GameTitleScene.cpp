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

	//カメラ初期化
	viewProjection = new ViewProjection();
	viewProjection->Initialize();

	//天球
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//sprite
	cross = new Sprite;
	cross->Initialize(dxCommon_);
	cross->LoadTexture(0, L"Resources/cross.png", dxCommon_);
	cross->SetScale({ 0.5,0.5 });

	// サウンドの初期化
	titleBGM = new Sound;
	titleBGM->SoundLoadWave("Resources/Sound/TitleBGM.wav");
	titleBGM->SoundPlayWave(true, 1.0f);
}

void GameTitleScene::Update()
{
	// シーンの切り替え
	if (input_->TriggerMouseLeft() == true) {
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		titleBGM->StopWave();
	}

	//天球
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
	// サウンドの解放
	delete titleBGM;
}
