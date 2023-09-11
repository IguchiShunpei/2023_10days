#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::GetInstance()
{
	static Input instance;

	return &instance;
}

void Input::Initialize(WinApp* winApp)
{
	this->winApp = winApp;

	HRESULT result;

	//DirectInputの初期化
	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);  //標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);


	//マウスデバイスのセット
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = mouse->SetDataFormat(&c_dfDIMouse); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = mouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	assert(SUCCEEDED(result));

	//デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	result = mouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	assert(SUCCEEDED(result));
	//マウスカーソルを隠す
	ShowCursor(FALSE);
}

void Input::Update()
{

	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	//キーボード情報の取得開始
	keyboard->Acquire();

	//全キーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(key), key);

	//マウス制御開始
	memcpy(&mouseState_bak, &mouseState, sizeof(mouseState_bak));
	mouse->Acquire();

	HRESULT result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
}

bool Input::PushMouseLeft()
{
	if (mouseState.rgbButtons[0] != 0) {
		return true;
	}
	return false;
}

bool Input::TriggerMouseLeft()
{
	if (mouseState.rgbButtons[0] > 0 && mouseState_bak.rgbButtons[0] == 0) {
		return true;
	}
	return false;
}

bool Input::PushMouseRight()
{
	if (mouseState.rgbButtons[1] > 0) {
		return true;
	}
	return false;
}

bool Input::TriggerMouseRight()
{
	if (mouseState.rgbButtons[1] > 0 && mouseState_bak.rgbButtons[1] == 0) {
		return true;
	}
	return false;
}

Vector3 Input::GetMousePos()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(winApp->GetHwnd(), &p);

	//z成分を0で返す
	return Vector3((float)p.x, (float)p.y, 0.0f);
}

bool Input::PushKey(BYTE keyNumber)
{
	//指定キーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 指定キーを前フレームで押していなく、今のフレームで押していればtrueを返す
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// そうでなければfalseを返す
	return false;
}

bool Input::ReleasedKey(BYTE keyNumber)
{
	// 指定キーを前フレームで押していて、今のフレームで押していなければtrueを返す
	if (keyPre[keyNumber] && !key[keyNumber]) {
		return true;
	}

	// そうでなければfalseを返す
	return false;
}