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
	//namespace省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //メンバ関数
	//初期化
	void Initialize(WinApp* winApp);
	//更新
	void Update();

	//マウス
	bool PushMouseLeft();
	bool TriggerMouseLeft();
	bool PushMouseRight();
	bool TriggerMouseRight();
	//座標
	Vector3 GetMousePos();

	//長押し
	bool PushKey(BYTE keyNumber);

	//押した瞬間
	bool TriggerKey(BYTE keyNumber);

	//離した瞬間
	bool ReleasedKey(BYTE keyNumber);
private: //メンバ変数

	// DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput = nullptr;

	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard = nullptr;

	//マウスのデバイス
	ComPtr<IDirectInputDevice8> mouse;

	//全キーの状態
	BYTE key[256] = {};

	// 前回の全キーの状態
	BYTE keyPre[256] = {};

	//マウスの状態
	DIMOUSESTATE mouseState = {};
	DIMOUSESTATE mouseState_bak = {};

	//WindowsAPI
	WinApp* winApp = nullptr;
};