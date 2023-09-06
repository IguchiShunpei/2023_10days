#include "GamePlayScene.h"

#include "CollisionManager.h"
#include "SphereCollider.h"
#include <fstream>
#include <math.h>

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

	//�G�f�[�^�ǂݍ���
	LoadEnemyPop();

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
		objMeteor = new Meteor;
		objMeteor->MeteorInitialize();
		objMeteor->SetModel(model);

		// ���W
		Vector3 pos;
		//�f�[�^�̒l����
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObject�ɃZ�b�g
		objMeteor->SetPosition(pos);

		// ��]�p
		Vector3 rot;
		//�f�[�^�̒l����
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObject�ɃZ�b�g
		objMeteor->SetRotation(rot);

		// ���W
		Vector3 scale;
		//�f�[�^�̒l����
		scale.x = objectData.scaling.m128_f32[0];
		scale.y = objectData.scaling.m128_f32[1];
		scale.z = objectData.scaling.m128_f32[2];
		//newObject�ɃZ�b�g
		objMeteor->SetScale(scale);

		// �z��ɓo�^
		meteorObjects.push_back(objMeteor);
	}	
}

void GamePlayScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// �Q�[���v���C�V�[���i���V�[���j�𐶐�
		GameSceneManager::GetInstance()->ChangeScene("CLEAR");
	}

	//�f�X�t���O�̗������G���폜
	enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
		{
			return enemy->GetIsDead();
		});
	enemys_.remove_if([](std::unique_ptr <Enemy>& enemy)
		{
			return enemy->GetIsDelete();
		});

	//�V��
	sky->Update();
	viewProjection->UpdateMatrix();
	
	//�G�f�[�^
	UpdateEnemyPop();

	//�ˌ�
	Shot();

	//�G
	for (std::unique_ptr<Enemy>& enemys : enemys_)
	{
		enemys->Update();
		enemys->ColliderUpdate();
	}

	//for (auto& object : meteorObjects)
	//{
	//	object->MeteorUpdate();
	//}

	//�p�[�e�B�N���X�V
	pm_dmg->Update();
}

void GamePlayScene::Draw()
{

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

void GamePlayScene::Shot()
{
	if (input->TriggerMouseLeft()) {
		Vector3 cur = input->GetMousePos();
		for (const std::unique_ptr<Enemy>& enemy : enemys_) {
			Vector3 epos = GetWorldToScreenPos(enemy->GetPosition(), viewProjection);
			if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(30, 2)) {
				enemy->SetIsDead(true);
			}
		}
	}
}

Vector3 GamePlayScene::GetWorldToScreenPos(Vector3 pos_, ViewProjection* viewProjection_)
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

void GamePlayScene::LoadEnemyPop()
{
	enemys_.clear();
	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	enemyPopCommands << file.rdbuf();

	// �t�@�C���ƕ���
	file.close();
}

void GamePlayScene::UpdateEnemyPop()
{
	//�ҋ@����
	if (isWait_ == true)
	{
		waitTimer_--;
		if (waitTimer_ <= 0)
		{
			//�ҋ@����
			isWait_ = false;
		}
		return;
	}

	// �P�s���ǂݍ���
	std::string line;

	while (getline(enemyPopCommands, line))
	{

		// �P�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		// ���p�X�p�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		// enemy��ǂݎ���č��W���Z�b�g����
		if (key == "enemy") {
			//�G�̐���
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			//�G�̏�����
			newEnemy->EnemyInitialize();
			//�R���C�_�[�̒ǉ�
			newEnemy->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// ���W�f�[�^�ɒǉ�
			newEnemy->SetPosition(position);
			newEnemy->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newEnemy->worldTransform_.UpdateMatrix();
			//�o�^
			enemys_.push_back(std::move(newEnemy));
		}
		//�ҋ@���Ԃ�ǂݎ��
		else if (key == "wait")
		{
			std::string word;
			getline(line_stream, word, ' ');

			//�҂�����
			int32_t waitTime = atoi(word.c_str());

			//�ҋ@�J�n
			isWait_ = true;
			waitTimer_ = waitTime;

			//�R�}���h���[�v�𔲂���
			break;
		}
	}
}