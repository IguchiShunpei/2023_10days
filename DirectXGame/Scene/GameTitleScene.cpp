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
	// �V�[���̐؂�ւ�
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// �Q�[���v���C�V�[���i���V�[���j�𐶐�
		GameSceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

}

void GameTitleScene::Draw()
{
	
}

void GameTitleScene::Finalize()
{
	
}
