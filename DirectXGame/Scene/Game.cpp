#include "Game.h"

#include "SceneFactory.h"

void Game::Initialize()
{	
	// 基底クラスの初期化処理
	SIFrameWork::Initialize();

	// ポストエフェクト
	postEffect = new PostEffect();
	postEffect1 = new PostEffect();
	postEffect->Initialize();
	postEffect1->Initialize();

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

#pragma region ゲームシーン描画

	// レンダーテクスチャの前処理
	postEffect->PreDraw(cmdList);

	//=== シーンマネージャの描画 ===//
	sceneManager_->Draw();

	// レンダーテクスチャの後処理
	postEffect->PostDraw(cmdList);

#pragma endregion

#pragma region ポストエフェクトの描画

	// レンダーテクスチャ1の前処理
	postEffect1->PreDraw(cmdList);

	//=== ポストエフェクトの描画 ===//
	postEffect->Draw(cmdList);

	// レンダーテクスチャ1の後処理
	postEffect1->PostDraw(cmdList);

#pragma endregion

#pragma region ポストエフェクト1の描画

	// 描画前処理
	dxCommon->PreDraw();

	//=== ポストエフェクト1の描画 ===//
	postEffect1->Draw(cmdList);

	// 描画後処理
	dxCommon->PostDraw();

#pragma endregion
}