#include "GameTitleScene.h"

using namespace DirectX;

GameTitleScene::GameTitleScene()
{
	// サウンドの静的初期化
	Sound::StaticInitialize();
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
	titleLogo = new Sprite;
	titleLogo->Initialize(dxCommon_);
	titleLogo->LoadTexture(0, L"Resources/TITLE.png", dxCommon_);
	titleLogo->SetScale({ 8,5 });
	click = new Sprite;
	click->Initialize(dxCommon_);
	click->LoadTexture(0, L"Resources/CLICKHERE.png", dxCommon_);
	click->SetScale({ 4,1 });

	// サウンドの初期化
	titleBGM = new Sound;
	titleBGM->SoundLoadWave("Resources/Sound/titleBGM.wav");
	titleBGM->SoundPlayWave(true, 1.0f);

	startSE = new Sound;
	startSE->SoundLoadWave("Resources/Sound/start.wav");

	shotSE = new Sound;
	shotSE->SoundLoadWave("Resources/Sound/shot.wav");


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
			//配置
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
	//天球
	sky->Update();
	viewProjection->UpdateMatrix();

	//カーソル
	Vector3 cur = input_->GetMousePos();
	cross->SetPosition({ cur.x - 24,cur.y - 24,0 });
	cross->Update();
	for (int i = 0; i < 120; i++) {
		sceneEffect1[i]->Update();
		sceneEffect2[i]->Update();
	}

	//ロゴやUI
	titleLogo->SetPosition({ 240,10,0 });
	titleLogo->Update();
	click->SetPosition({ 450,520,0 });
	click->Update();

	// シーンの切り替え
	if (input_->TriggerMouseLeft() == true) {
		if (cur.x >= click->GetPosition().x && cur.x <= click->GetPosition().x + 300)
		{
			if (cur.y >= click->GetPosition().y && cur.y <= click->GetPosition().y + 100)
			{
				isNext = true;
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
							// ゲームプレイシーン（次シーン）を生成
							GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
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
	// �`��O����
	dxCommon_->PreDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	sky->Draw(viewProjection);

	Object3d::PostDraw();
	titleLogo->SetTextureCommands(0, dxCommon_);
	titleLogo->Draw(dxCommon_);
	click->SetTextureCommands(0, dxCommon_);
	click->Draw(dxCommon_);
	cross->SetTextureCommands(0, dxCommon_);
	cross->Draw(dxCommon_);

	//シーンチェンジ
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
  
  // �`��㏈��
  dxCommon_->PostDraw();
}

void GameTitleScene::Finalize()
{

}
