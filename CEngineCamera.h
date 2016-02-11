#include "CEngineCore.h"
#include <math.h>

#pragma once

class CEngineRender;
class CEngineCamera
{
public:
	CEngineCamera();
	void _Update();
	void Reset();
	void SetSpeed(float Value){Speed = Value;}
	void SetPos(float x,float y, float z) {CamPos = XMVectorSet(x,y,z,0);}

	void Scaling(float Value);
	void RotX(float Value);
	void RotY(float Value);
	void InversionX(){InvX = !InvX;}
	void InversionY(){InvY = !InvY;}
	void MoveUp();
	void MoveDown();
	void MoveForward();
	void MoveBack();
	void MoveRight();
	void MoveLeft();
	float GetCoordX(){return XMVectorGetX(CamPos);}
	float GetCoordY(){return XMVectorGetY(CamPos);}
	float GetCoordZ(){return XMVectorGetZ(CamPos);}

	XMMATRIX GetMatScale(){return matScale;}
	XMMATRIX GetMatProj() {return matProj;}
	XMMATRIX GetMatView() {return matView;}
	XMVECTOR GetPos(){return CamPos;}
protected:
	bool InvX;
	bool InvY;
	float Scale;
	float Speed;

	XMVECTOR CamRight;
	XMVECTOR CamLook;
	XMVECTOR CamPos;
	XMVECTOR CamUp;

	XMMATRIX matView;
	XMMATRIX matProj;
	XMMATRIX matScale;
	XMMATRIX matRotation;
};

void CEngineCamera::Reset(){
	CamRight = XMVectorSet(1,0,0,0);
	CamLook = XMVectorSet(0,0,1,0);
	CamPos = XMVectorSet(0,0,-5,0);
	CamUp = XMVectorSet(0,1,0,0);
	Scale = 1;
	return;
}


CEngineCamera::CEngineCamera(){
	InvX = false;
	InvY = false;
	Speed = 0.01f;
	Reset();
	matScale = XMMatrixIdentity();
	matView = XMMatrixLookAtLH(CamPos,CamPos+CamLook,XMVectorSet(0,1,0,0));
	matProj = XMMatrixPerspectiveFovLH(XM_PIDIV4,1.6f,1.0f,1000.0f);
	return;
}


void CEngineCamera::_Update()
{
	matView = XMMatrixLookAtLH(CamPos,CamPos+CamLook,XMVectorSet(0,1,0,0));
	matView = XMMatrixMultiply(matScale,matView);
	return;
}

void CEngineCamera::Scaling(float Value){
	Scale += Value;
	matScale = XMMatrixScaling(Scale,Scale,Scale);
	return;
}


void CEngineCamera::RotX(float Value){
	if(InvX == true) Value = -Value;
	matRotation = XMMatrixRotationY(Value);
	CamLook = XMVector3Transform(CamLook,matRotation);
	CamRight = XMVector3Transform(CamRight,matRotation);
	return;
}

void CEngineCamera::RotY(float Value){
	if(InvY == true) Value = -Value;
	matRotation = XMMatrixRotationAxis(CamRight,Value);
	CamLook = XMVector3Transform(CamLook,matRotation);
	return;
}

void CEngineCamera::MoveUp(){
	CamPos += CamUp*Speed;
	return;}

void CEngineCamera::MoveDown(){
	CamPos -= CamUp*Speed;
	return;}

void CEngineCamera::MoveForward(){
	CamPos += CamLook*Speed;
	return;}

void CEngineCamera::MoveBack(){
	CamPos -= CamLook*Speed;
	return;}

void CEngineCamera::MoveRight(){
	CamPos += CamRight*Speed;
	return;}

void CEngineCamera::MoveLeft(){
	CamPos -= CamRight*Speed;
	return;}

CEngineCamera Camera;