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
	dxCommon = DirectXCommon::GetInstance();
}

void GameClearScene::Update()
{
	// 十字キーの右を押したら
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameClearScene::Draw()
{
	// 描画前処理
	dxCommon->PreDraw();
	
	// 描画後処理
	dxCommon->PostDraw();
}

void GameClearScene::Finalize()
{
}
