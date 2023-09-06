#pragma once

#include "AbstractSceneFactory.h"
#include "GameTitleScene.h"
#include "GamePlayScene.h"
#include "GameClearScene.h"

/// <summary>
/// ���̃Q�[���p�̃V�[���H��
/// </summary>
class SceneFactory : public AbstractSceneFactory
{
public:
	/// <summary>
	/// �V�[������
	/// </summary>
	/// <param name="sceneName">�V�[����</param>
	/// <returns></returns>
	GameBaseScene* CreateScene(const std::string& sceneName) override;
};

