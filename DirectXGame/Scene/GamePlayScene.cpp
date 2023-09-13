#include "GamePlayScene.h"

#include "CollisionManager.h"
#include "SphereCollider.h"
#include <fstream>
#include <string.h>
#include <math.h>

using namespace DirectX;
using namespace std;

GamePlayScene::GamePlayScene()
{

}

GamePlayScene::~GamePlayScene()
{

}

void GamePlayScene::Initialize()
{
	input = Input::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	//当たり判定
	collisionManager = CollisionManager::GetInstance();

	//カメラ初期化
	viewProjection = new ViewProjection();
	xmViewProjection = new XMViewProjection();

	viewProjection->Initialize();

	//敵データ読み込み
	LoadEnemyPop();

	//天球
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//パーティクル
	p_dmg = Particle::LoadParticleTexture("effect.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);
	pEffect01 = Particle::LoadParticleTexture("effectstar.png");
	pEffect02 = Particle::LoadParticleTexture("effectstar.png");
	pEffect03 = Particle::LoadParticleTexture("effectstar.png");
	pEffect04 = Particle::LoadParticleTexture("effectstar.png");
	pmEffect01 = ParticleManager::Create();
	pmEffect01->SetParticleModel(pEffect01);
	pmEffect01->SetXMViewProjection(xmViewProjection);
	pmEffect02 = ParticleManager::Create();
	pmEffect02->SetParticleModel(pEffect02);
	pmEffect02->SetXMViewProjection(xmViewProjection);
	pmEffect03 = ParticleManager::Create();
	pmEffect03->SetParticleModel(pEffect03);
	pmEffect03->SetXMViewProjection(xmViewProjection);
	pmEffect04 = ParticleManager::Create();
	pmEffect04->SetParticleModel(pEffect04);
	pmEffect04->SetXMViewProjection(xmViewProjection);

	//sprite
	cross = new Sprite;
	cross->Initialize(dxCommon);
	cross->LoadTexture(0, L"Resources/cross.png", dxCommon);
	cross->SetScale({ 0.5,0.5 });

	//number
	const wchar_t* pathname = L"Resources/numbers/";
	const wchar_t* extantion = L".png";
	wchar_t filename[16] = L"";

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
		onesPlace[i]->SetPosition({ 1200,620,0 });
		onesPlace[i]->SetScale({ 0.5f,1.0f });
		//10
		tensPlace[i] = new Sprite;
		tensPlace[i]->Initialize(dxCommon);
		tensPlace[i]->LoadTexture(i + 1, path, dxCommon);
		tensPlace[i]->SetPosition({ 1148,620,0 });
		tensPlace[i]->SetScale({ 0.5f,1.0f });
		//100
		hundredPlace[i] = new Sprite;
		hundredPlace[i]->Initialize(dxCommon);
		hundredPlace[i]->LoadTexture(i + 1, path, dxCommon);
		hundredPlace[i]->SetPosition({ 1096,620,0 });
		hundredPlace[i]->SetScale({ 0.5f,1.0f });
		//1000
		thousandPlace[i] = new Sprite;
		thousandPlace[i]->Initialize(dxCommon);
		thousandPlace[i]->LoadTexture(i + 1, path, dxCommon);
		thousandPlace[i]->SetPosition({ 1044,620,0 });
		thousandPlace[i]->SetScale({ 0.5f,1.0f });
		//10000
		tenthousandPlace[i] = new Sprite;
		tenthousandPlace[i]->Initialize(dxCommon);
		tenthousandPlace[i]->LoadTexture(i + 1, path, dxCommon);
		tenthousandPlace[i]->SetPosition({ 992,620,0 });
		tenthousandPlace[i]->SetScale({ 0.5f,1.0f });
		//100000
		hundredthousandPlace[i] = new Sprite;
		hundredthousandPlace[i]->Initialize(dxCommon);
		hundredthousandPlace[i]->LoadTexture(i + 1, path, dxCommon);
		hundredthousandPlace[i]->SetPosition({ 940,620,0 });
		hundredthousandPlace[i]->SetScale({ 0.5f,1.0f });
	}
	//得点用画像
	getGold = new Sprite;
	getGold->Initialize(dxCommon);
	getGold->LoadTexture(0, L"Resources/p1000.png", dxCommon);
	getGold->SetScale({ 1.28f * 1.5f,0.48f * 1.5f });
	//10
	for (int i = 0; i < 10; i++) {
		getNormal[i] = new Sprite;
		getNormal[i]->Initialize(dxCommon);
		getNormal[i]->LoadTexture(0, L"Resources/p10.png", dxCommon);
		getNormal[i]->SetScale({ 0.72f * 1.2f,0.48f * 1.2f });
	}
	for (int i = 0; i < 5; i++) {
		//red
		getRed[i] = new Sprite;
		getRed[i]->Initialize(dxCommon);
		getRed[i]->LoadTexture(0, L"Resources/p50.png", dxCommon);
		getRed[i]->SetScale({ 0.72f * 1.3f,0.48f * 1.3f });
		//blue
		getBlue[i] = new Sprite;
		getBlue[i]->Initialize(dxCommon);
		getBlue[i]->LoadTexture(0, L"Resources/m30.png", dxCommon);
		getBlue[i]->SetScale({ 0.72f * 1.3f,0.48f * 1.3f });
	}

	// レベルデータの読み込み
	levelData = LevelLoader::LoadFile("backGround");

	//モデル読み込み
	modelMeteor = Model::LoadFromOBJ("meteor");

	meteorModels.insert(std::make_pair("meteor", modelMeteor));

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(meteorModels)::iterator it = meteorModels.find(objectData.fileName);
		if (it != meteorModels.end()) {
			model = it->second;
		}

		// モデルを指定して3Dオブジェクトを生成
		objMeteor = new Meteor;
		objMeteor->MeteorInitialize();
		objMeteor->SetModel(model);

		// 座標
		Vector3 pos;
		//データの値を代入
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObjectにセット
		objMeteor->SetPosition(pos);

		// 回転角
		Vector3 rot;
		//データの値を代入
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObjectにセット
		objMeteor->SetRotation(rot);

		// 座標
		Vector3 scale;
		//データの値を代入
		scale.x = objectData.scaling.m128_f32[0];
		scale.y = objectData.scaling.m128_f32[1];
		scale.z = objectData.scaling.m128_f32[2];
		//newObjectにセット
		objMeteor->SetScale(scale);

		// 配列に登録
		meteorObjects.push_back(objMeteor);
	}

	// サウンドの初期化
	gameBGM = new Sound;
	gameBGM->SoundLoadWave("Resources/Sound/gameBGM.wav");
	gameBGM->SoundPlayWave(true, 1.0f);
	shotSE = new Sound;
	shotSE->SoundLoadWave("Resources/Sound/shot.wav");
	finishSE = new Sound;
	finishSE->SoundLoadWave("Resources/Sound/finish.wav");
	getSE = new Sound;
	getSE->SoundLoadWave("Resources/Sound/get.wav");
	highGetSE = new Sound;
	highGetSE->SoundLoadWave("Resources/Sound/highGet.wav");
	superHighGetSE = new Sound;
	superHighGetSE->SoundLoadWave("Resources/Sound/superHighGet.wav");
	missSE = new Sound;
	missSE->SoundLoadWave("Resources/Sound/miss.wav");

	//各変数の初期化
	score_ = 0;
	for (int i = 0; i < 6; i++) {
		scores[i] = 0;
		oldScores[i] = 0;
	}
	isWait_ = false;
	isFinish_ = false;
	waitTimer_ = 0;
	finishTimer_ = 0;
	isGetGold = false;
	goldTime = 0;
	for (int i = 0; i < 10; i++) {
		isGetNormal[i] = false;
		normalTime[i] = 0;
	}
	for (int i = 0; i < 5; i++) {
		isGetRed[i] = false;
		redTime[i] = 0;
		isGetBlue[i] = false;
		blueTime[i] = 0;
	}
}

void GamePlayScene::Update()
{
	if (isFinish_ == true)
	{
		finishTimer_++;
		if (finishTimer_ >= 180)
		{
			// ゲームプレイシーン（次シーン）を生成
			GameSceneManager::GetInstance()->ChangeScene("CLEAR");
			gameBGM->StopWave();
			finishTimer_ = 0;
		}
	}
	Vector3 cur = input->GetMousePos();
	cross->SetPosition({ cur.x - 24,cur.y - 24,0 });
	cross->Update();

	//デスフラグの立った敵を削除
	enemys_01.remove_if([](std::unique_ptr <Enemy>& enemy01)
		{
			return enemy01->GetIsDead();
		});
	enemys_01.remove_if([](std::unique_ptr <Enemy>& enemy01)
		{
			return enemy01->GetIsDelete();
		});
	enemys_02.remove_if([](std::unique_ptr <Enemy>& enemy02)
		{
			return enemy02->GetIsDead();
		});
	enemys_02.remove_if([](std::unique_ptr <Enemy>& enemy02)
		{
			return enemy02->GetIsDelete();
		});
	enemys_03.remove_if([](std::unique_ptr <Enemy>& enemy03)
		{
			return enemy03->GetIsDead();
		});
	enemys_03.remove_if([](std::unique_ptr <Enemy>& enemy03)
		{
			return enemy03->GetIsDelete();
		});
	enemys_04.remove_if([](std::unique_ptr <Enemy>& enemy04)
		{
			return enemy04->GetIsDead();
		});
	enemys_04.remove_if([](std::unique_ptr <Enemy>& enemy04)
		{
			return enemy04->GetIsDelete();
		});

	//天球
	sky->Update();
	viewProjection->UpdateMatrix();

	//敵データ
	UpdateEnemyPop();

	//射撃
	Shot();


	//敵
	for (std::unique_ptr<Enemy>& enemys01 : enemys_01)
	{
		enemys01->Update();
		enemys01->ColliderUpdate();
	}
	for (std::unique_ptr<Enemy>& enemys02 : enemys_02)
	{
		enemys02->Update();
		enemys02->ColliderUpdate();
	}
	for (std::unique_ptr<Enemy>& enemys03 : enemys_03)
	{
		enemys03->Update();
		enemys03->ColliderUpdate();
	}
	for (std::unique_ptr<Enemy>& enemys04 : enemys_04)
	{
		enemys04->Update();
		enemys04->ColliderUpdate();
	}

	//for (auto& object : meteorObjects)
	//{
	//	object->MeteorUpdate();
	//}

	//パーティクル更新
	pm_dmg->Update();
	pmEffect01->Update();
	pmEffect02->Update();
	pmEffect03->Update();
	pmEffect04->Update();

	if (isGetGold == true) {
		XMFLOAT3 g = getGold->GetPosition();
		g.y -= 0.5f;
		getGold->SetPosition(g);
		getGold->Update();
		goldTime++;
		if (goldTime == 50) {
			isGetGold = false;
			goldTime = 0;
		}
	}
	for (int i = 0; i < 10; i++) {
		if (isGetNormal[i] == true) {
			XMFLOAT3 n = getNormal[i]->GetPosition();
			n.y -= 0.5f;
			getNormal[i]->SetPosition(n);
			getNormal[i]->Update();
			normalTime[i]++;
			if (normalTime[i] == 50) {
				isGetNormal[i] = false;
				normalTime[i] = 0;
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		if (isGetRed[i] == true) {
			XMFLOAT3 r = getRed[i]->GetPosition();
			r.y -= 0.5f;
			getRed[i]->SetPosition(r);
			getRed[i]->Update();
			redTime[i]++;
			if (redTime[i] == 50) {
				isGetRed[i] = false;
				redTime[i] = 0;
			}
		}
		if (isGetBlue[i] == true) {
			XMFLOAT3 b = getBlue[i]->GetPosition();
			b.y -= 0.5f;
			getBlue[i]->SetPosition(b);
			getBlue[i]->Update();
			blueTime[i]++;
			if (blueTime[i] == 50) {
				isGetBlue[i] = false;
				blueTime[i] = 0;
			}
		}
	}

	//スコア更新
	XMFLOAT3 one = onesPlace[scores[0]]->GetPosition();
	if (one.y < 620) {
		one.y += 2;
		onesPlace[scores[0]]->SetPosition(one);
	}
	onesPlace[scores[0]]->Update();
	XMFLOAT3 ten = tensPlace[scores[1]]->GetPosition();
	if (ten.y < 620) {
		ten.y += 2;
		tensPlace[scores[1]]->SetPosition(ten);
	}
	tensPlace[scores[1]]->Update();
	XMFLOAT3 hund = hundredPlace[scores[2]]->GetPosition();
	if (hund.y < 620) {
		hund.y += 2;
		hundredPlace[scores[2]]->SetPosition(hund);
	}
	hundredPlace[scores[2]]->Update();
	XMFLOAT3 thou = thousandPlace[scores[3]]->GetPosition();
	if (thou.y < 620) {
		thou.y += 2;
		thousandPlace[scores[3]]->SetPosition(thou);
	}
	thousandPlace[scores[3]]->Update();
	XMFLOAT3 tThou = tenthousandPlace[scores[4]]->GetPosition();
	if (tThou.y < 620) {
		tThou.y += 2;
		tenthousandPlace[scores[4]]->SetPosition(tThou);
	}
	tenthousandPlace[scores[4]]->Update();
	XMFLOAT3 hThou = hundredthousandPlace[scores[5]]->GetPosition();
	if (hThou.y < 620) {
		hThou.y += 2;
		hundredthousandPlace[scores[5]]->SetPosition(hThou);
	}
	hundredthousandPlace[scores[5]]->Update();
	for (int i = 0; i < 6; i++) {
		oldScores[i] = scores[i];
	}
}

void GamePlayScene::Draw()
{

#pragma region 最初のシーンの描画

	Object3d::PreDraw(dxCommon->GetCommandList());

	sky->Draw(viewProjection);

	/*for (auto& object : meteorObjects)
	{
		object->Draw(viewProjection);
	}*/
	//敵キャラの描画
	for (const std::unique_ptr<Enemy>& enemy01 : enemys_01) {
		enemy01->Draw(viewProjection);
	}
	for (const std::unique_ptr<Enemy>& enemy02 : enemys_02) {
		enemy02->Draw(viewProjection);
	}
	for (const std::unique_ptr<Enemy>& enemy03 : enemys_03) {
		enemy03->Draw(viewProjection);
	}
	for (const std::unique_ptr<Enemy>& enemy04 : enemys_04) {
		enemy04->Draw(viewProjection);
	}

	Object3d::PostDraw();

	//エフェクト描画前処理
	ParticleManager::PreDraw(dxCommon->GetCommandList());

	pm_dmg->Draw();
	pmEffect01->Draw();
	pmEffect02->Draw();
	pmEffect03->Draw();
	pmEffect04->Draw();

	//エフェクト描画後処理
	ParticleManager::PostDraw();
	cross->SetTextureCommands(0, dxCommon);
	cross->Draw(dxCommon);
	getGold->SetTextureCommands(0, dxCommon);
	if (isGetGold == true) {
		getGold->Draw(dxCommon);
	}
	for (int i = 0; i < 10; i++) {
		getNormal[i]->SetTextureCommands(i, dxCommon);
		if (isGetNormal[i] == true) {
			getNormal[i]->Draw(dxCommon);
		}
	}
	for (int i = 0; i < 5; i++) {
		getRed[i]->SetTextureCommands(i, dxCommon);
		getBlue[i]->SetTextureCommands(i, dxCommon);
		if (isGetRed[i] == true) {
			getRed[i]->Draw(dxCommon);
		}
		if (isGetBlue[i] == true) {
			getBlue[i]->Draw(dxCommon);
		}
	}
	//スコア
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

#pragma endregion 最初のシーンの描画

}

void GamePlayScene::Finalize()
{
	for (Meteor*& object : meteorObjects) {
		delete(object);
	}
	delete p_dmg;
	delete pm_dmg;

	//カメラ解放
	delete viewProjection;
	delete xmViewProjection;
}

void GamePlayScene::GameReset()
{
	viewProjection->Initialize();
}

void GamePlayScene::Shot()
{
	bool isHit = false;
	if (input->TriggerMouseLeft()) {
		shotSE->SoundPlayWave(false, 1.0f);
		Vector3 cur = input->GetMousePos();
		for (const std::unique_ptr<Enemy>& enemy01 : enemys_01) {
			Vector3 epos = GetWorldToScreenPos(enemy01->GetPosition(), viewProjection);
			if (enemy01->GetIsBack() == false)
			{
				if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(50, 2)) {
					enemy01->SetIsDead(true);
					getSE->SoundPlayWave(false, 1.0f);
					score_ += 10;
					for (int i = 0; i < 10; i++) {
						if (isGetNormal[i] == false) {
							isGetNormal[i] = true;
							getNormal[i]->SetPosition({ epos.x - 36,epos.y - 24,0 });
							break;
						}
					}
					isHit = true;
				}
			}
			// 敵撃破時のエフェクト
			if (enemy01->GetIsDead() == true) {
				XMFLOAT3 ePos01 = { enemy01->GetPosition().x * 2.5f, enemy01->GetPosition().y * 2.5f,enemy01->GetPosition().z };
				pmEffect01->Fire(pEffect01, 20, ePos01, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0, 30, { 0.0f, 7.0f });
			}
		}
		for (const std::unique_ptr<Enemy>& enemy02 : enemys_02) {
			Vector3 epos = GetWorldToScreenPos(enemy02->GetPosition(), viewProjection);
			if (enemy02->GetIsBack() == false)
			{
				if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(50, 2)) {
					enemy02->SetIsDead(true);
					highGetSE->SoundPlayWave(false, 1.0f);
					score_ += 50;
					for (int i = 0; i < 5; i++) {
						if (isGetRed[i] == false) {
							isGetRed[i] = true;
							getRed[i]->SetPosition({ epos.x - 36,epos.y - 24,0 });
							break;
						}
					}
					isHit = true;
				}
			}
			// 敵撃破時のエフェクト
			if (enemy02->GetIsDead() == true) {
				XMFLOAT3 ePos02 = { enemy02->GetPosition().x * 2.5f, enemy02->GetPosition().y * 2.5f, enemy02->GetPosition().z };
				pmEffect02->Fire(pEffect02, 20, ePos02, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0, 30, { 0.0f, 7.0f });
			}
		}
		for (const std::unique_ptr<Enemy>& enemy03 : enemys_03) {
			Vector3 epos = GetWorldToScreenPos(enemy03->GetPosition(), viewProjection);
			if (enemy03->GetIsBack() == false)
			{
				if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(50, 2)) {
					enemy03->SetIsDead(true);
					missSE->SoundPlayWave(false, 1.0f);
					score_ -= 30;
					for (int i = 0; i < 5; i++) {
						if (isGetBlue[i] == false) {
							isGetBlue[i] = true;
							getBlue[i]->SetPosition({ epos.x - 36,epos.y - 24,0 });
							break;
						}
					}
					isHit = true;
				}
			}
			// 敵撃破時のエフェクト
			if (enemy03->GetIsDead() == true) {
				XMFLOAT3 ePos03 = { enemy03->GetPosition().x * 2.5f, enemy03->GetPosition().y * 2.5f,enemy03->GetPosition().z };
				pmEffect03->Fire(pEffect03, 20, ePos03, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0, 30, { 0.0f, 7.0f });
			}
		}
		for (const std::unique_ptr<Enemy>& enemy04 : enemys_04) {
			Vector3 epos = GetWorldToScreenPos(enemy04->GetPosition(), viewProjection);
			if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(70, 2)) {
				enemy04->SetIsDead(true);
				superHighGetSE->SoundPlayWave(false, 1.0f);
				score_ += 1000;
				if (isGetGold == false) {
					isGetGold = true;
					getGold->SetPosition({ epos.x - 64,epos.y - 24,0 });
				}
				isHit = true;
			}
			// 敵撃破時のエフェクト
			if (enemy04->GetIsDead() == true) {
				XMFLOAT3 ePos04 = { enemy04->GetPosition().x * 2.5f, enemy04->GetPosition().y * 2.5f, enemy04->GetPosition().z };
				pmEffect04->Fire(pEffect04, 20, ePos04, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0, 30, { 0.0f, 7.0f });
			}
		}
	}
	ScoreCalc();
	if (isHit == true) {
		for (int i = 0; i < 6; i++) {
			if ((oldScores[i] - scores[i]) != 0) {
				if (i == 0) {
					onesPlace[scores[0]]->SetPosition({ 1200,596,0 });
				}
				else if (i == 1) {
					tensPlace[scores[1]]->SetPosition({ 1148,596,0 });
				}
				else if (i == 2) {
					hundredPlace[scores[2]]->SetPosition({ 1096,596,0 });
				}
				else if (i == 3) {
					thousandPlace[scores[3]]->SetPosition({ 1044,596,0 });
				}
				else if (i == 4) {
					tenthousandPlace[scores[4]]->SetPosition({ 992,596,0 });
				}
				else if (i == 5) {
					hundredthousandPlace[scores[5]]->SetPosition({ 940,596,0 });
				}
				else {}
			}
		}
	}
}

Vector3 GamePlayScene::GetWorldToScreenPos(Vector3 pos_, ViewProjection* viewProjection_)
{
	if (viewProjection_ == nullptr) {
		return Vector3(0, 0, 0);
	}

	//ビュー行列//
	Matrix4 view = viewProjection_->matView;
	//プロジェクション行列//
	float fovAngleY = 45.0f * (3.141592f / 180.0f);;
	float aspectRatio = (float)WinApp::window_width / WinApp::window_height;
	//プロジェクション行列生成
	Matrix4 projection = projection.ProjectionMat(fovAngleY, aspectRatio, 0.1f, 200.0f);
	//ビューポート行列生成
	Matrix4 viewPort = viewPort.ViewPortMat(WinApp::window_width, WinApp::window_height, Vector2(0.0f, 0.0f));

	Matrix4 matVPV = view * projection * viewPort;

	Matrix4 mat;
	Vector3 posScreen = pos_;
	posScreen = mat.transform(posScreen, matVPV);
	posScreen.z = 0;

	return posScreen;
}

void GamePlayScene::LoadEnemyPop()
{
	enemys_01.clear();
	enemys_02.clear();
	enemys_03.clear();
	enemys_04.clear();
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルと閉じる
	file.close();
}

void GamePlayScene::UpdateEnemyPop()
{
	//待機処理
	if (isWait_ == true)
	{
		waitTimer_--;
		if (waitTimer_ <= 0)
		{
			//待機完了
			isWait_ = false;
		}
		return;
	}

	// １行ずつ読み込む
	std::string line;

	while (getline(enemyPopCommands, line))
	{

		// １行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スパース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		// enemy01を読み取って座標をセットする
		if (key == "enemy01") {
			//敵の生成
			std::unique_ptr<Enemy> newEnemy01 = std::make_unique<Enemy>();
			//敵の初期化
			newEnemy01->SetEnemyNum(1);
			newEnemy01->EnemyInitialize();
			//コライダーの追加
			newEnemy01->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			newEnemy01->SetPosition(position);
			newEnemy01->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newEnemy01->worldTransform_.UpdateMatrix();
			//登録
			enemys_01.push_back(std::move(newEnemy01));
		}
		//enemy02
		else if (key == "enemy02") {
			//敵の生成
			std::unique_ptr<Enemy> newEnemy02 = std::make_unique<Enemy>();
			//敵の初期化
			newEnemy02->SetEnemyNum(2);
			newEnemy02->EnemyInitialize();
			//コライダーの追加
			newEnemy02->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			newEnemy02->SetPosition(position);
			newEnemy02->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newEnemy02->worldTransform_.UpdateMatrix();
			//登録
			enemys_02.push_back(std::move(newEnemy02));
		}
		//enemy03
		else if (key == "enemy03") {
			//敵の生成
			std::unique_ptr<Enemy> newEnemy03 = std::make_unique<Enemy>();
			//敵の初期化
			newEnemy03->SetEnemyNum(3);
			newEnemy03->EnemyInitialize();
			//コライダーの追加
			newEnemy03->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			newEnemy03->SetPosition(position);
			newEnemy03->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newEnemy03->worldTransform_.UpdateMatrix();
			//登録
			enemys_03.push_back(std::move(newEnemy03));
		}
		//enemy04
		else if (key == "enemy04") {
			std::string word;
			getline(line_stream, word, ' ');
			//敵の生成
			std::unique_ptr<Enemy> newEnemy04 = std::make_unique<Enemy>();
			//敵の初期化
			newEnemy04->SetEnemyNum(4);
			newEnemy04->EnemyInitialize();
			//コライダーの追加
			newEnemy04->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			//移動向きを読み取ってセットする
			if (word.find("DOWN") == 0)
			{
				newEnemy04->SetDirectionNum(1);
			}
			else if (word.find("LEFT") == 0)
			{
				newEnemy04->SetDirectionNum(2);
			}
			else if (word.find("RIGHT") == 0)
			{
				newEnemy04->SetDirectionNum(3);
			}
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			newEnemy04->SetPosition(position);
			newEnemy04->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newEnemy04->worldTransform_.UpdateMatrix();
			//登録
			enemys_04.push_back(std::move(newEnemy04));
		}
		//待機時間を読み取る
		else if (key == "wait")
		{
			std::string word;
			getline(line_stream, word, ' ');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait_ = true;
			waitTimer_ = waitTime;

			//コマンドループを抜ける
			break;
		}

		else if (key == "FINISH")
		{
			finishSE->SoundPlayWave(false, 1.0f);
			isFinish_ = true;
		}
	}
}

void GamePlayScene::ScoreCalc() {
	int score = score_;
	if (score_ >= 100000) {
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
	else if (score_ >= 10000) {
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
	else if (score_ >= 1000) {
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
	else if (score_ >= 100) {
		scores[5] = 0;
		scores[4] = 0;
		scores[3] = 0;
		scores[2] = score / 100;
		score -= scores[2] * 100;
		scores[1] = score / 10;
		score -= scores[1] * 10;
		scores[0] = score;
	}
	else if (score_ >= 10) {
		if (score_ == 11) {
			int a = 0;
		}
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
		scores[0] = score_;
	}
}