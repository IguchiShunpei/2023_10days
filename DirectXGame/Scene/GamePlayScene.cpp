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
	enemys_01.remove_if([](std::unique_ptr <Enemy>& enemy01)
		{
			return enemy01->GetIsDead();
		});
	enemys_01.remove_if([](std::unique_ptr <Enemy>& enemy01)
		{
			return enemy01->GetIsDelete();
		});
	enemys_02.remove_if([](std::unique_ptr <Enemy>& enemy02)
		{
			return enemy02->GetIsDead();
		});
	enemys_02.remove_if([](std::unique_ptr <Enemy>& enemy02)
		{
			return enemy02->GetIsDelete();
		});
	enemys_03.remove_if([](std::unique_ptr <Enemy>& enemy03)
		{
			return enemy03->GetIsDead();
		});
	enemys_03.remove_if([](std::unique_ptr <Enemy>& enemy03)
		{
			return enemy03->GetIsDelete();
		});

	//�V��
	sky->Update();
	viewProjection->UpdateMatrix();
	
	//�G�f�[�^
	UpdateEnemyPop();

	//�ˌ�
	Shot();

	//�G
	for (std::unique_ptr<Enemy>& enemys01 : enemys_01)
	{
		enemys01->Update();
		enemys01->ColliderUpdate();
	}
	for (std::unique_ptr<Enemy>& enemys02 : enemys_02)
	{
		enemys02->Update();
		enemys02->ColliderUpdate();
	}
	for (std::unique_ptr<Enemy>& enemys03 : enemys_03)
	{
		enemys03->Update();
		enemys03->ColliderUpdate();
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
	for (const std::unique_ptr<Enemy>& enemy01 : enemys_01) {
		enemy01->Draw(viewProjection);
	}
	for (const std::unique_ptr<Enemy>& enemy02 : enemys_02) {
		enemy02->Draw(viewProjection);
	}
	for (const std::unique_ptr<Enemy>& enemy03 : enemys_03) {
		enemy03->Draw(viewProjection);
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
		for (const std::unique_ptr<Enemy>& enemy01 : enemys_01) {
			Vector3 epos = GetWorldToScreenPos(enemy01->GetPosition(), viewProjection);
			if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(50, 2)) {
				enemy01->SetIsDead(true);
			}
		}
		for (const std::unique_ptr<Enemy>& enemy02 : enemys_02) {
			Vector3 epos = GetWorldToScreenPos(enemy02->GetPosition(), viewProjection);
			if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(50, 2)) {
				enemy02->SetIsDead(true);
			}
		}
		for (const std::unique_ptr<Enemy>& enemy03 : enemys_03) {
			Vector3 epos = GetWorldToScreenPos(enemy03->GetPosition(), viewProjection);
			if (pow((epos.x - cur.x), 2) + pow((epos.y - cur.y), 2) < pow(50, 2)) {
				enemy03->SetIsDead(true);
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
	enemys_01.clear();
	enemys_02.clear();
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

		// enemy01��ǂݎ���č��W���Z�b�g����
		if (key == "enemy01") {
			//�G�̐���
			std::unique_ptr<Enemy> newEnemy01 = std::make_unique<Enemy>();
			//�G�̏�����
			newEnemy01->SetEnemyNum(1);
			newEnemy01->EnemyInitialize();
			//�R���C�_�[�̒ǉ�
			newEnemy01->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// ���W�f�[�^�ɒǉ�
			newEnemy01->SetPosition(position);
			newEnemy01->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newEnemy01->worldTransform_.UpdateMatrix();
			//�o�^
			enemys_01.push_back(std::move(newEnemy01));
		}
		//enemy02
		else if (key == "enemy02") {
			//�G�̐���
			std::unique_ptr<Enemy> newEnemy02 = std::make_unique<Enemy>();
			//�G�̏�����
			newEnemy02->SetEnemyNum(2);
			newEnemy02->EnemyInitialize();
			//�R���C�_�[�̒ǉ�
			newEnemy02->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// ���W�f�[�^�ɒǉ�
			newEnemy02->SetPosition(position);
			newEnemy02->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newEnemy02->worldTransform_.UpdateMatrix();
			//�o�^
			enemys_02.push_back(std::move(newEnemy02));
		}
		//enemy03
		else if (key == "enemy03") {
			//�G�̐���
			std::unique_ptr<Enemy> newEnemy03 = std::make_unique<Enemy>();
			//�G�̏�����
			newEnemy03->SetEnemyNum(3);
			newEnemy03->EnemyInitialize();
			//�R���C�_�[�̒ǉ�
			newEnemy03->SetCollider(new SphereCollider(Vector3(0, 0, 0), 1.5f));
			// X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// ���W�f�[�^�ɒǉ�
			newEnemy03->SetPosition(position);
			newEnemy03->SetScale(Vector3(0.8f, 0.8f, 0.8f));
			newEnemy03->worldTransform_.UpdateMatrix();
			//�o�^
			enemys_03.push_back(std::move(newEnemy03));
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