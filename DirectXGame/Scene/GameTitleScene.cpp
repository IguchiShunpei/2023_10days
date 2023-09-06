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
}

void GameTitleScene::Update()
{
	// シーンの切り替え
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// ゲームプレイシーン（次シーン）を生成
		GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

}

void GameTitleScene::Draw()
{
	
}

void GameTitleScene::Finalize()
{
	
}
