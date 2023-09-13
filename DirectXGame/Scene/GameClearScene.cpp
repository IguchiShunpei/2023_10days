#include "GameClearScene.h"

using namespace DirectX;

GameClearScene::GameClearScene()
{
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize()
{
	input_ = Input::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	//天球
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//カメラ初期化
	viewProjection = new ViewProjection();
	viewProjection->Initialize();

	//sprite
	cross = new Sprite;
	cross->Initialize(dxCommon);
	cross->LoadTexture(0, L"Resources/cross.png", dxCommon);
	cross->SetScale({ 0.5,0.5 });

	scoreGraph = new Sprite;
	scoreGraph->Initialize(dxCommon);
	scoreGraph->LoadTexture(0, L"Resources/SCORE.png", dxCommon);
	scoreGraph->SetScale({ 5.0f,1.5f });
	scoreGraph->SetPosition({ 400.0f,50.0f ,0.0f});

	//click
	title01 = new Sprite;
	title01->Initialize(dxCommon);
	title01->LoadTexture(0, L"Resources/ToTITLE_01.png", dxCommon);
	title01->SetScale({ 4,1 });
	title02 = new Sprite;
	title02->Initialize(dxCommon);
	title02->LoadTexture(0, L"Resources/ToTITLE_02.png", dxCommon);
	title02->SetScale({ 4,1 });

	retry01 = new Sprite;
	retry01->Initialize(dxCommon);
	retry01->LoadTexture(0, L"Resources/RETRY_01.png", dxCommon);
	retry01->SetScale({ 4,1 });
	retry02 = new Sprite;
	retry02->Initialize(dxCommon);
	retry02->LoadTexture(0, L"Resources/RETRY_02.png", dxCommon);
	retry02->SetScale({ 4,1 });

	//SE
	startSE = new Sound;
	startSE->SoundLoadWave("Resources/Sound/start.wav");
	shotSE = new Sound;
	shotSE->SoundLoadWave("Resources/Sound/shot.wav");

	//number
	const wchar_t* pathname = L"Resources/numbers/";
	const wchar_t* extantion = L".png";
	wchar_t filename[16] = L"";
	numPosX = 340.0f;
	float posY = 260.0f;

	for (int i = 0; i < 10; i++) {
		//1
		onesPlace[i] = new Sprite;
		onesPlace[i]->Initialize(dxCommon);
		int num = i;
		_itow_s(num, filename, 10);
		wchar_t path[128] = L"";
		wcscat_s(path, pathname);
		wcscat_s(path, filename);
		wcscat_s(path, extantion);

		onesPlace[i]->LoadTexture(i + 1, path, dxCommon);
		onesPlace[i]->SetPosition({ numPosX + 520.0f,posY,0.0f });
		onesPlace[i]->SetScale({ 1.0f,1.5f });
		//10
		tensPlace[i] = new Sprite;
		tensPlace[i]->Initialize(dxCommon);
		tensPlace[i]->LoadTexture(i + 1, path, dxCommon);
		tensPlace[i]->SetPosition({ numPosX + 416.0f,posY,0.0f });
		tensPlace[i]->SetScale({ 1.0f,1.5f });
		//100
		hundredPlace[i] = new Sprite;
		hundredPlace[i]->Initialize(dxCommon);
		hundredPlace[i]->LoadTexture(i + 1, path, dxCommon);
		hundredPlace[i]->SetPosition({ numPosX + 312.0f,posY,0.0f });
		hundredPlace[i]->SetScale({ 1.0f,1.5f });
		//1000
		thousandPlace[i] = new Sprite;
		thousandPlace[i]->Initialize(dxCommon);
		thousandPlace[i]->LoadTexture(i + 1, path, dxCommon);
		thousandPlace[i]->SetPosition({ numPosX + 208.0f,posY,0.0f });
		thousandPlace[i]->SetScale({ 1.0f,1.5f });
		//10000
		tenthousandPlace[i] = new Sprite;
		tenthousandPlace[i]->Initialize(dxCommon);
		tenthousandPlace[i]->LoadTexture(i + 1, path, dxCommon);
		tenthousandPlace[i]->SetPosition({ numPosX + 104.0f,posY,0 });
		tenthousandPlace[i]->SetScale({ 1.0f,1.5f });
		//100000
		hundredthousandPlace[i] = new Sprite;
		hundredthousandPlace[i]->Initialize(dxCommon);
		hundredthousandPlace[i]->LoadTexture(i + 1, path, dxCommon);
		hundredthousandPlace[i]->SetPosition({ numPosX,posY,0 });
		hundredthousandPlace[i]->SetScale({ 1.0f,1.5f });
	}
	for (int i = 0; i < 6; i++) {
		scores[i] = 0;
	}
	int score = GamePlayScene::GetScore();
	if (score >= 100000) {
		scores[5] = score / 100000;
		score -= scores[5] * 100000;
		scores[4] = score / 10000;
		score -= scores[4] * 10000;
		scores[3] = score / 1000;
		score -= scores[3] * 1000;
		scores[2] = score / 100;
		score -= scores[2] * 100;
		scores[1] = score / 10;
		score -= scores[1] * 10;
		scores[0] = score;
	}
	else if (score >= 10000) {
		scores[5] = 0;
		scores[4] = score / 10000;
		score -= scores[4] * 10000;
		scores[3] = score / 1000;
		score -= scores[3] * 1000;
		scores[2] = score / 100;
		score -= scores[2] * 100;
		scores[1] = score / 10;
		score -= scores[1] * 10;
		scores[0] = score;
	}
	else if (score >= 1000) {
		scores[5] = 0;
		scores[4] = 0;
		scores[3] = score / 1000;
		score -= scores[3] * 1000;
		scores[2] = score / 100;
		score -= scores[2] * 100;
		scores[1] = score / 10;
		score -= scores[1] * 10;
		scores[0] = score;
	}
	else if (score >= 100) {
		scores[5] = 0;
		scores[4] = 0;
		scores[3] = 0;
		scores[2] = score / 100;
		score -= scores[2] * 100;
		scores[1] = score / 10;
		score -= scores[1] * 10;
		scores[0] = score;
	}
	else if (score >= 10) {
		scores[1] = score / 10;
		score -= scores[1] * 10;
		scores[0] = score;
		scores[2] = 0;
		scores[3] = 0;
		scores[4] = 0;
		scores[5] = 0;
	}
	else {
		scores[5] = 0;
		scores[4] = 0;
		scores[3] = 0;
		scores[2] = 0;
		scores[1] = 0;
		scores[0] = score;
	}
	showTime = 0;
	isNext = false;
	curHit01 = false;
	curHit02 = false;
}

void GameClearScene::Update()
{
	Vector3 cur = Input::GetInstance()->GetMousePos();
	cross->SetPosition({ cur.x - 24,cur.y - 24,0 });
	cross->Update();
	//天球
	sky->Update();
	viewProjection->UpdateMatrix();

	//スコア更新
	showTime++;
	if (showTime == 10) {
		scoreGraph->Update();
		shotSE->SoundPlayWave(false, 0.5f);
	}
	else if (showTime == 60) {
		onesPlace[scores[0]]->Update();
		tensPlace[scores[1]]->Update();
		hundredPlace[scores[2]]->Update();
		thousandPlace[scores[3]]->Update();
		tenthousandPlace[scores[4]]->Update();
		hundredthousandPlace[scores[5]]->Update();
		shotSE->SoundPlayWave(false, 0.5f);
	}
	else if (showTime == 120) {
		shotSE->SoundPlayWave(false, 0.5f);
	}
	else if (showTime > 120) {
		title01->SetPosition({ 700,520,0 });
		title01->Update();
		title02->SetPosition({ 700,520,0 });
		title02->Update();

		retry01->SetPosition({ 200,520,0 });
		retry01->Update();
		retry02->SetPosition({ 200,520,0 });
		retry02->Update();

		// シーンの切り替え
		if (cur.x >= title01->GetPosition().x && cur.x <= title01->GetPosition().x + 400)
		{
			if (cur.y >= title01->GetPosition().y && cur.y <= title01->GetPosition().y + 100)
			{
				curHit01 = true;
				if (Input::GetInstance()->TriggerMouseLeft() == true)
				{
					// ゲームプレイシーン（次シーン）を生成
					GameSceneManager::GetInstance()->ChangeScene("TITLE");
					startSE->SoundPlayWave(false, 0.5f);
				}
			}
			else
			{
				curHit01 = false;
			}
		}
		else
		{
			curHit01 = false;
		}
		// シーンの切り替え
		if (cur.x >= retry01->GetPosition().x && cur.x <= retry01->GetPosition().x + 400)
		{
			if (cur.y >= retry01->GetPosition().y && cur.y <= retry01->GetPosition().y + 100)
			{
				curHit02 = true;
				if (Input::GetInstance()->TriggerMouseLeft() == true)
				{
					// ゲームプレイシーン（次シーン）を生成
					GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
					startSE->SoundPlayWave(false, 0.5f);
				}
			}
			else
			{
				curHit02 = false;
			}
		}
		else
		{
			curHit02 = false;
		}
		if (input_->TriggerMouseLeft() == true)
		{
			shotSE->SoundPlayWave(false, 0.5f);
		}
	}
}

void GameClearScene::Draw()
{
	// 描画前処理
	dxCommon->PreDraw();

	Object3d::PreDraw(dxCommon->GetCommandList());

	sky->Draw(viewProjection);

	Object3d::PostDraw();

	scoreGraph->SetTextureCommands(0, dxCommon);
	scoreGraph->Draw(dxCommon);

	onesPlace[scores[0]]->SetTextureCommands(0, dxCommon);
	onesPlace[scores[0]]->Draw(dxCommon);
	tensPlace[scores[1]]->SetTextureCommands(0, dxCommon);
	tensPlace[scores[1]]->Draw(dxCommon);
	hundredPlace[scores[2]]->SetTextureCommands(0, dxCommon);
	hundredPlace[scores[2]]->Draw(dxCommon);
	thousandPlace[scores[3]]->SetTextureCommands(0, dxCommon);
	thousandPlace[scores[3]]->Draw(dxCommon);
	tenthousandPlace[scores[4]]->SetTextureCommands(0, dxCommon);
	tenthousandPlace[scores[4]]->Draw(dxCommon);
	hundredthousandPlace[scores[5]]->SetTextureCommands(0, dxCommon);
	hundredthousandPlace[scores[5]]->Draw(dxCommon);

	if (curHit01 == true)
	{
		title02->SetTextureCommands(0, dxCommon);
		title02->Draw(dxCommon);
	}
	else
	{
		title01->SetTextureCommands(0, dxCommon);
		title01->Draw(dxCommon);
	}
	if (curHit02 == true)
	{
		retry02->SetTextureCommands(0, dxCommon);
		retry02->Draw(dxCommon);
	}
	else
	{
		retry01->SetTextureCommands(0, dxCommon);
		retry01->Draw(dxCommon);
	}

	cross->SetTextureCommands(0, dxCommon);
	cross->Draw(dxCommon);

	// 描画後処理
	dxCommon->PostDraw();
}

void GameClearScene::Finalize()
{
	GamePlayScene::SetScore(0);
}
