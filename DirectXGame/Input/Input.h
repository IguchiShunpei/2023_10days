#pragma once
#include<windows.h>
#include<wrl.h>
#include"WinApp.h"
#include<dinput.h>
#include "Vector3.h"

class Input
{
public:
	static Input* GetInstance();

public:
	//namespace�ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //�����o�֐�
	//������
	void Initialize(WinApp* winApp);
	//�X�V
	void Update();

	//�}�E�X
	bool PushMouseLeft();
	bool TriggerMouseLeft();
	bool PushMouseRight();
	bool TriggerMouseRight();
	//���W
	Vector3 GetMousePos();

	//������
	bool PushKey(BYTE keyNumber);

	//�������u��
	bool TriggerKey(BYTE keyNumber);

	//�������u��
	bool ReleasedKey(BYTE keyNumber);
private: //�����o�ϐ�

	// DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> directInput = nullptr;

	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> keyboard = nullptr;

	//�}�E�X�̃f�o�C�X
	ComPtr<IDirectInputDevice8> mouse;

	//�S�L�[�̏��
	BYTE key[256] = {};

	// �O��̑S�L�[�̏��
	BYTE keyPre[256] = {};

	//�}�E�X�̏��
	DIMOUSESTATE mouseState = {};
	DIMOUSESTATE mouseState_bak = {};

	//WindowsAPI
	WinApp* winApp = nullptr;
};