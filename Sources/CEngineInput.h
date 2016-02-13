#include <dinput.h>

#pragma once

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#define DIM_RBUTTON 1
#define DIM_LBUTTON 2
#define DIM_BUTTON  3
#define DIM_GETX	4
#define DIM_GETY	5
#define DIM_GETZ	6

class CEngineInput
{
protected:
	IDirectInput* pDIInterface;
	LPDIRECTINPUTDEVICE pKeyboard;
	LPDIRECTINPUTDEVICE pMouse;
	DIMOUSESTATE MouseBuffer;
	char KeyBuffer[256];
public:
	void _Init(HINSTANCE hInstance);
	void _Acquire();
	bool Keyboard(short);
	LONG Mouse(short);
	~CEngineInput();
};

void CEngineInput::_Init(HINSTANCE hInstance)
{
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
                   IID_IDirectInput8, (void**)&pDIInterface, NULL);
	/*if(pDIInterface == NULL){
		Core.SetError(ERROR_CODE_DINPUT);
		return;}*/
	pDIInterface->CreateDevice(GUID_SysKeyboard, &pKeyboard, NULL);
	/*if(pKeyboard == NULL){
		Core.SetError(ERROR_CODE_DIKEYBOARD);
		return;}*/
	pDIInterface->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	/*if(pMouse == NULL){
		Core.SetError(ERROR_CODE_DIMOUSE);
		return;}*/
	pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	pKeyboard->SetCooperativeLevel(Core.hMainWnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	pMouse->SetDataFormat(&c_dfDIMouse);
	pMouse->SetCooperativeLevel(Core.hMainWnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	pKeyboard->Acquire();
	pMouse->Acquire();
	return;
}

void CEngineInput::_Acquire(){
	if(FAILED(pKeyboard->GetDeviceState(sizeof(KeyBuffer),KeyBuffer)))
		pKeyboard->Acquire();
	if(FAILED(pMouse->GetDeviceState(sizeof(DIMOUSESTATE),&MouseBuffer)))
		pMouse->Acquire();
}

bool CEngineInput::Keyboard(short Key)
{
	if(KeyBuffer[Key])
		return true;
	return false;
}

LONG CEngineInput::Mouse(short Type)
{
	if(Type == DIM_LBUTTON)
		if(MouseBuffer.rgbButtons[0] & 0x80)
			return 1;
		else
			return 0;
	if(Type == DIM_RBUTTON)
		if(MouseBuffer.rgbButtons[1] & 0x80)
			return 1;
		else
			return 0;
	if(Type == DIM_BUTTON)
		if(MouseBuffer.rgbButtons[2] & 0x80)
			return 1;
		else
			return 0;
	if(Type == DIM_GETX)
		return MouseBuffer.lX;
	if(Type == DIM_GETY)
		return MouseBuffer.lY;
	if(Type == DIM_GETZ)
		return MouseBuffer.lZ;
	return 0;
}

CEngineInput::~CEngineInput()
{
	pMouse->Release();
	pKeyboard->Release();
	pDIInterface->Release();
	return;
}

CEngineInput Input;