#include "GamePlayScene.h"

#include "CollisionManager.h"
#include "SphereCollider.h"
#include <fstream>

using namespace DirectX;
using namespace std;

GamePlayScene::GamePlayScene()
{
}

GamePlayScene::~GamePlayScene()
{

}

void GamePlayScene::Initialize()
{
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
}

void GamePlayScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// �Q�[���v���C�V�[���i���V�[���j�𐶐�
		GameSceneManager::GetInstance()->ChangeScene("CLEAR");
	}

	//�V��
	sky->Update();
	viewProjection->UpdateMatrix();

	for (auto& object : meteorObjects)
	{
		object->MeteorUpdate();
	}

	//�p�[�e�B�N���X�V
	pm_dmg->Update();
}

void GamePlayScene::Draw()
{

#pragma region �ŏ��̃V�[���̕`��

	Object3d::PreDraw(dxCommon->GetCommandList());

	sky->Draw(viewProjection);

	for (auto& object : meteorObjects)
	{
		object->Draw(viewProjection);
	}

	Object3d::PostDraw();

	//�G�t�F�N�g�`��O����
	ParticleManager::PreDraw(dxCommon->GetCommandList());

	pm_dmg->Draw();

	//�G�t�F�N�g�`��㏈��
	ParticleManager::PostDraw();

#pragma endregion �ŏ��̃V�[���̕`��

}

void GamePlayScene::Finalize()
{
	for (Meteor*& object : meteorObjects) {
		delete(object);
	}
	delete p_dmg;
	delete pm_dmg;

	//�J�������
	delete viewProjection;
	delete xmViewProjection;
}

void GamePlayScene::GameReset()
{
	viewProjection->Initialize();
}
