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

	//�����蔻��
	collisionManager = CollisionManager::GetInstance();

	//�J����������
	viewProjection = new ViewProjection();
	xmViewProjection = new XMViewProjection();

	viewProjection->Initialize();

	//�V��
	sky = new SkyDome;
	sky->SkyDomeInitialize();

	//�p�[�e�B�N��
	p_dmg = Particle::LoadParticleTexture("effect.png");
	pm_dmg = ParticleManager::Create();
	pm_dmg->SetParticleModel(p_dmg);
	pm_dmg->SetXMViewProjection(xmViewProjection);

	// ���x���f�[�^�̓ǂݍ���
	levelData = LevelLoader::LoadFile("backGround");

	//���f���ǂݍ���
	modelMeteor = Model::LoadFromOBJ("meteor");

	meteorModels.insert(std::make_pair("meteor", modelMeteor));

	// ���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData->objects) {
		// �t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		decltype(meteorModels)::iterator it = meteorModels.find(objectData.fileName);
		if (it != meteorModels.end()) {
			model = it->second;
		}

		// ���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		meteor = new Meteor;
		meteor->MeteorInitialize();
		meteor->SetModel(model);

		// ���W
		Vector3 pos;
		//�f�[�^�̒l����
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObject�ɃZ�b�g
		meteor->SetPosition(pos);

		// ��]�p
		Vector3 rot;
		//�f�[�^�̒l����
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObject�ɃZ�b�g
		meteor->SetRotation(rot);

		// ���W
		Vector3 scale;
		//�f�[�^�̒l����
		scale.x = objectData.scaling.m128_f32[0];
		scale.y = objectData.scaling.m128_f32[1];
		scale.z = objectData.scaling.m128_f32[2];
		//newObject�ɃZ�b�g
		meteor->SetScale(scale);

		// �z��ɓo�^
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

	//�J�������
	delete viewProjection;
	delete xmViewProjection;

	SIFrameWork::Finalize();
}

void GameScene::Update()
{
	SIFrameWork::Update();

	//�V��
	sky->Update();
	viewProjection->UpdateMatrix();
	Shot();

	//for (auto& object : meteorObjects)
	//{
	//	object->MeteorUpdate();
	//}
	//�f�X�t���O�̗������G���폜
	enemys_.remove_if([](std::unique_ptr < Enemy>& enemy_) {
		return enemy_->GetIsDead();
		});
	//�G�L�����̍X�V
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
	}

	//�p�[�e�B�N���X�V
	pm_dmg->Update();

}

void GameScene::Draw()
{
	//�`��O����
	dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��

	Object3d::PreDraw(dxCommon->GetCommandList());

	sky->Draw(viewProjection);

	/*for (auto& object : meteorObjects)
	{
		object->Draw(viewProjection);
	}*/
	//�G�L�����̕`��
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw(viewProjection);
	}

Object3d::PostDraw();

//�G�t�F�N�g�`��O����
ParticleManager::PreDraw(dxCommon->GetCommandList());

pm_dmg->Draw();

//�G�t�F�N�g�`��㏈��
ParticleManager::PostDraw();

#pragma endregion �ŏ��̃V�[���̕`��

// �`��㏈��
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

	//�r���[�s��//
	Matrix4 view = viewProjection_->matView;
	//�v���W�F�N�V�����s��//
	float fovAngleY = 45.0f * (3.141592f / 180.0f);;
	float aspectRatio = (float)WinApp::window_width / WinApp::window_height;
	//�v���W�F�N�V�����s�񐶐�
	Matrix4 projection = projection.ProjectionMat(fovAngleY, aspectRatio, 0.1f, 200.0f);
	//�r���[�|�[�g�s�񐶐�
	Matrix4 viewPort = viewPort.ViewPortMat(WinApp::window_width, WinApp::window_height, Vector2(0.0f, 0.0f));

	Matrix4 matVPV = view * projection * viewPort;

	Matrix4 mat;
	Vector3 posScreen = pos_;
	posScreen = mat.transform(posScreen, matVPV);
	posScreen.z = 0;

	return posScreen;
}