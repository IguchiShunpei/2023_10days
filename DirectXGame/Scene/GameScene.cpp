#include "GameScene.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include <fstream>
#include <math.h>

void GameScene::Initialize()
{
	SIFrameWork::Initialize();

	input = Input::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	//当たり判定
	collisionManager = CollisionManager::GetInstance();

	//カメラ初期化
	viewProjection = new ViewProjection();
	xmViewProjection = new XMViewProjection();

	viewProjection->Initialize();

	//天球
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//パーティクル
	p_dmg = Particle::LoadParticleTexture("effect.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);

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
		meteor = new Meteor;
		meteor->MeteorInitialize();
		meteor->SetModel(model);

		// 座標
		Vector3 pos;
		//データの値を代入
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObjectにセット
		meteor->SetPosition(pos);

		// 回転角
		Vector3 rot;
		//データの値を代入
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObjectにセット
		meteor->SetRotation(rot);

		// 座標
		Vector3 scale;
		//データの値を代入
		scale.x = objectData.scaling.m128_f32[0];
		scale.y = objectData.scaling.m128_f32[1];
		scale.z = objectData.scaling.m128_f32[2];
		//newObjectにセット
		meteor->SetScale(scale);

		// 配列に登録
		meteorObjects.push_back(meteor);
	}	
	//enemy
	for (int i = 0; i < 3; i++) {
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
		newEnemy->EnemyInitialize();
		newEnemy->SetPosition(Vector3(-8.0f + (i * 8.0f),0.0f,0.0f));
		newEnemy->SetScale(Vector3(0.5f, 0.5f, 0.5f));
		enemys_.push_back(std::move(newEnemy));
	}
}

void GameScene::Finalize()
{
	for (Meteor*& object : meteorObjects) {
		delete(object);
	}
	delete p_dmg;
	delete pm_dmg;

	//カメラ解放
	delete viewProjection;
	delete xmViewProjection;

	SIFrameWork::Finalize();
}

void GameScene::Update()
{
	SIFrameWork::Update();

	//天球
	sky->Update();
	viewProjection->UpdateMatrix();
	Shot();

	//for (auto& object : meteorObjects)
	//{
	//	object->MeteorUpdate();
	//}
	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr < Enemy>& enemy_) {
		return enemy_->GetIsDead();
		});
	//敵キャラの更新
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
	}

	//パーティクル更新
	pm_dmg->Update();

}

void GameScene::Draw()
{
	//描画前処理
	dxCommon->PreDraw();

#pragma region 最初のシーンの描画

	Object3d::PreDraw(dxCommon->GetCommandList());

	sky->Draw(viewProjection);

	/*for (auto& object : meteorObjects)
	{
		object->Draw(viewProjection);
	}*/
	//敵キャラの描画
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw(viewProjection);
	}

Object3d::PostDraw();

//エフェクト描画前処理
ParticleManager::PreDraw(dxCommon->GetCommandList());

pm_dmg->Draw();

//エフェクト描画後処理
ParticleManager::PostDraw();

#pragma endregion 最初のシーンの描画

// 描画後処理
dxCommon->PostDraw();
}

void GameScene::Reset()
{
	viewProjection->Initialize();
}

void GameScene::Shot()
{
	if (input->TriggerMouseLeft()) {
		Vector3 cur = input->GetMousePos();
		for (const std::unique_ptr<Enemy>& enemy : enemys_) {
			Vector3 epos = GetWorldToScreenPos(enemy->GetPosition(), viewProjection);
			if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(100,2)) {
				enemy->SetIsDead(true);
			}
		}
	}
}

Vector3 GameScene::GetWorldToScreenPos(Vector3 pos_, ViewProjection* viewProjection_)
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