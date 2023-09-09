#include "Game.h"

#include "SceneFactory.h"

void Game::Initialize()
{	
	// 基底クラスの初期化処理
	SIFrameWork::Initialize();

	// ポストエフェクトの初期化
	postEffect = new PostEffect();
	postEffect->Initialize();

	// シーンファクトリを生成し、マネージャにセット
	sceneFactory_ = new SceneFactory();
	sceneManager_->SetSceneFactory(sceneFactory_);
	// シーンマネージャに最初のシーンをセット
	sceneManager_->ChangeScene("TITLE");
}

void Game::Finalize()
{
	// 基底クラスの終了処理
	SIFrameWork::Finalize();
}

void Game::Update()
{
	// 基底クラスの更新処理
	SIFrameWork::Update();
}

void Game::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	postEffect->PreDraw(cmdList);

	//=== シーンマネージャの描画 ===//
	sceneManager_->Draw();

	postEffect->PostDraw(cmdList);


	// 描画前処理
	dxCommon->PreDraw();

	postEffect->Draw(cmdList);

	// 描画後処理
	dxCommon->PostDraw();
}
