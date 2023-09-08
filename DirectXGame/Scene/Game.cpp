#include "Game.h"

#include "SceneFactory.h"

void Game::Initialize()
{	
	// ���N���X�̏���������
	SIFrameWork::Initialize();

	// �|�X�g�G�t�F�N�g
	postEffect = new PostEffect();
	postEffect1 = new PostEffect();
	postEffect->Initialize();
	postEffect1->Initialize();

	// �V�[���t�@�N�g���𐶐����A�}�l�[�W���ɃZ�b�g
	sceneFactory_ = new SceneFactory();
	sceneManager_->SetSceneFactory(sceneFactory_);
	// �V�[���}�l�[�W���ɍŏ��̃V�[�����Z�b�g
	sceneManager_->ChangeScene("TITLE");
}

void Game::Finalize()
{
	// ���N���X�̏I������
	SIFrameWork::Finalize();
}

void Game::Update()
{
	// ���N���X�̍X�V����
	SIFrameWork::Update();
}

void Game::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �Q�[���V�[���`��

	// �����_�[�e�N�X�`���̑O����
	postEffect->PreDraw(cmdList);

	//=== �V�[���}�l�[�W���̕`�� ===//
	sceneManager_->Draw();

	// �����_�[�e�N�X�`���̌㏈��
	postEffect->PostDraw(cmdList);

#pragma endregion

#pragma region �|�X�g�G�t�F�N�g�̕`��

	// �����_�[�e�N�X�`��1�̑O����
	postEffect1->PreDraw(cmdList);

	//=== �|�X�g�G�t�F�N�g�̕`�� ===//
	postEffect->Draw(cmdList);

	// �����_�[�e�N�X�`��1�̌㏈��
	postEffect1->PostDraw(cmdList);

#pragma endregion

#pragma region �|�X�g�G�t�F�N�g1�̕`��

	// �`��O����
	dxCommon->PreDraw();

	//=== �|�X�g�G�t�F�N�g1�̕`�� ===//
	postEffect1->Draw(cmdList);

	// �`��㏈��
	dxCommon->PostDraw();

#pragma endregion
}