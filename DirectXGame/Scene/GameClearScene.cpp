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
	
}

void GameClearScene::Update()
{
	// �\���L�[�̉E����������
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// �Q�[���v���C�V�[���i���V�[���j�𐶐�
		GameSceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameClearScene::Draw()
{
	
}

void GameClearScene::Finalize()
{
}
