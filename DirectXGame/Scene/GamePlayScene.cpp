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

	//sprite
	cross = new Sprite;
	cross->Initialize(dxCommon);
	cross->LoadTexture(0, L"Resources/cross.png", dxCommon);
	cross->SetScale({ 0.5,0.5 });

	//number
	const wchar_t* pathname = L"Resources/numbers/";
	const wchar_t* extantion = L".png";
	wchar_t filename[16] = L"";

	for (int i = 0; i < 10;i++) {
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
		onesPlace[i]->SetPosition({1200,620,0});
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

	score_ = 0;
	for (int i = 0; i < 6; i++) {
		scores[i] = 0;
	}
	isWait_ = false;
	waitTimer_ = 0;
}

void GamePlayScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("CLEAR");
	}
	Vector3 cur = input->GetMousePos();
	cross->SetPosition({cur.x - 24,cur.y - 24,0});
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

	//スコア更新
	ScoreCalc();
	onesPlace[scores[0]]->Update();
	tensPlace[scores[1]]->Update();
	hundredPlace[scores[2]]->Update();
	thousandPlace[scores[3]]->Update();
	tenthousandPlace[scores[4]]->Update();
	hundredthousandPlace[scores[5]]->Update();
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

	//エフェクト描画後処理
	ParticleManager::PostDraw();
	cross->SetTextureCommands(0, dxCommon);
	cross->Draw(dxCommon);
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
	if (input->TriggerMouseLeft()) {
		Vector3 cur = input->GetMousePos();
		for (const std::unique_ptr<Enemy>& enemy01 : enemys_01) {
			Vector3 epos = GetWorldToScreenPos(enemy01->GetPosition(), viewProjection);
			if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(50, 2)) {
				enemy01->SetIsDead(true);
				score_++;
			}
		}
		for (const std::unique_ptr<Enemy>& enemy02 : enemys_02) {
			Vector3 epos = GetWorldToScreenPos(enemy02->GetPosition(), viewProjection);
			if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(50, 2)) {
				enemy02->SetIsDead(true);
				score_+=5;
			}
		}
		for (const std::unique_ptr<Enemy>& enemy03 : enemys_03) {
			Vector3 epos = GetWorldToScreenPos(enemy03->GetPosition(), viewProjection);
			if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(50, 2)) {
				enemy03->SetIsDead(true);
				score_ -= 3;
			}
		}
		for (const std::unique_ptr<Enemy>& enemy04 : enemys_04) {
			Vector3 epos = GetWorldToScreenPos(enemy04->GetPosition(), viewProjection);
			if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(50, 2)) {
				enemy04->SetIsDead(true);
				score_ += 10;
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
	}
}

void GamePlayScene::ScoreCalc() {
	int score = score_;
	if (score_ >= 100000) {
		scores[5] = score / 100000;
		score -= scores[5] * 100000;
		scores[4] = score / 10000;
		score -= scores[4]* 10000;
		scores[3] = score / 1000;
		score -= scores[3]* 1000;
		scores[2] = score / 100;
		score -= scores[2]* 100;
		scores[1] = score / 10;
		score -= scores[1]* 10;
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