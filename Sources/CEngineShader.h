//#include <d3dx11effect.h>
#include <string>
#include <map>

#include "CEngineCore.h"

#pragma once

#define VERTEX 0
#define PIXEL 1

struct CBDefVertex
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProj;
};

struct CBDefPixel
{
	XMVECTOR CamPos;
	XMFLOAT4 GlobalProp;
	XMFLOAT4 LightPosition[15];
	XMFLOAT4 LightDirection[10];
	XMFLOAT4 LightProp[15];
	XMFLOAT4 LightColor[15];
};

CBDefVertex CBDefVConst;
CBDefPixel CBDefPConst;

class CEngineRender;
class CEngineShader
{
protected:
	ID3D11InputLayout* pVertexLayout;
	std::wstring CurrentVShader;
	std::wstring CurrentPShader;
	ID3DBlob* pErrorBuffer;
	ID3DBlob* pShaderBuffer;
	//ID3DX11Effect* pEffect;
	//ID3DX11EffectTechnique* pEffectTechnique;
	std::map <std::wstring,ID3DBlob*> pVShaderBuffer;
	std::map <std::wstring,ID3D11VertexShader*> pVShader;
	std::map <std::wstring,ID3D11PixelShader*> pPShader;
	std::map <std::wstring,ID3D11Buffer*> pConstBuffer;
	std::map <std::wstring,void*> pConstStruct;
public:
	ID3D11Buffer* cbPerObjectBuffer;
	~CEngineShader();
	void _Init();
	void AddShader(std::wstring ShaderName, short ShaderType, UINT ConstSize, void* pConst);
	void AddShader(std::wstring ShaderName, std::wstring ShaderFile,
					std::string EntryPoint, short ShaderType, UINT ConstSize, void* pConst);
	void UseShader(std::wstring ShaderName, short ShaderType);
	void UpdateConst(std::wstring ShaderName);
	friend CEngineRender;
	void UseDefaultShader();
};

void CEngineShader::_Init(){
	CBDefVConst.mWorld = XMMatrixIdentity();
	AddShader(DefVSName,DefVSFile,DefVSEntryPoint,VERTEX,sizeof(CBDefVConst),&CBDefVConst);
	AddShader(DefPSName,DefPSFile,DefPSEntryPoint,PIXEL,sizeof(CBDefPConst),&CBDefPConst);
	UseShader(DefVSName,VERTEX);
	UseShader(DefPSName,PIXEL);
}

void CEngineShader::UseDefaultShader(){
	UseShader(DefVSName,VERTEX);
	UseShader(DefPSName,PIXEL);
}

void CEngineShader::AddShader(std::wstring ShaderName, short ShaderType, UINT ConstSize, void* pConst){
	if(pConst){
		pConstStruct[ShaderName] = pConst;
		D3D11_BUFFER_DESC CBufferDesc;	
		ZeroMemory(&CBufferDesc,sizeof(D3D11_BUFFER_DESC));
		CBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		CBufferDesc.ByteWidth = ConstSize;
		CBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		CBufferDesc.CPUAccessFlags = 0;
		CBufferDesc.MiscFlags = 0;
		Core.pD3DDevice->CreateBuffer(&CBufferDesc,NULL,&pConstBuffer[ShaderName]);
	}
	if(ShaderType == VERTEX){
		D3DX11CompileFromFile(&ShaderName[0],0,0,"main","vs_4_0",0,0,0,&pShaderBuffer,&pErrorBuffer,0);
//		char* err = (char*)pErrorBuffer->GetBufferPointer();
		Core.pD3DDevice->CreateVertexShader(pShaderBuffer->GetBufferPointer(),pShaderBuffer->GetBufferSize(),
									NULL,&pVShader[ShaderName]);
		pVShaderBuffer[ShaderName] = pShaderBuffer;
	}
	if(ShaderType == PIXEL){
		D3DX11CompileFromFile(&ShaderName[0],0,0,"main","ps_4_0",0,0,0,&pShaderBuffer,&pErrorBuffer,0);
		if(pErrorBuffer) char* err = (char*)pErrorBuffer->GetBufferPointer();
		Core.pD3DDevice->CreatePixelShader(pShaderBuffer->GetBufferPointer(),pShaderBuffer->GetBufferSize(),
									NULL,&pPShader[ShaderName]);
	}
}

void CEngineShader::AddShader(std::wstring ShaderName, std::wstring ShaderFile,
			   std::string EntryPoint, short ShaderType, UINT ConstSize, void* pConst){
	if(pConst){
		pConstStruct[ShaderName] = pConst;
		D3D11_BUFFER_DESC CBufferDesc;	
		ZeroMemory(&CBufferDesc,sizeof(D3D11_BUFFER_DESC));
		CBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		CBufferDesc.ByteWidth = ConstSize;
		CBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		CBufferDesc.CPUAccessFlags = 0;
		CBufferDesc.MiscFlags = 0;
		Core.pD3DDevice->CreateBuffer(&CBufferDesc,NULL,&pConstBuffer[ShaderName]);
	}
	if(ShaderType == VERTEX){
		D3DX11CompileFromFile(&ShaderFile[0],0,0,&EntryPoint[0],"vs_4_0",0,0,0,&pShaderBuffer,&pErrorBuffer,0);
//		char* err = (char*)pErrorBuffer->GetBufferPointer();
		Core.pD3DDevice->CreateVertexShader(pShaderBuffer->GetBufferPointer(),pShaderBuffer->GetBufferSize(),
									NULL,&pVShader[ShaderName]);
		Core.pD3DContext->VSSetConstantBuffers(0,1,&pConstBuffer[ShaderName]);
		pVShaderBuffer[ShaderName] = pShaderBuffer;
	}
	if(ShaderType == PIXEL){
		D3DX11CompileFromFile(&ShaderFile[0],0,0,&EntryPoint[0],"ps_4_0",0,0,0,&pShaderBuffer,&pErrorBuffer,0);
//		char* err = (char*)pErrorBuffer->GetBufferPointer();
		Core.pD3DDevice->CreatePixelShader(pShaderBuffer->GetBufferPointer(),pShaderBuffer->GetBufferSize(),
									NULL,&pPShader[ShaderName]);
		Core.pD3DContext->PSSetConstantBuffers(0,1,&pConstBuffer[ShaderName]);
	}
}

void CEngineShader::UseShader(std::wstring ShaderName, short ShaderType){
	if(ShaderType == VERTEX && CurrentVShader != ShaderName){
		Core.pD3DContext->VSSetConstantBuffers(0,1,&pConstBuffer[ShaderName]);
		Core.pD3DContext->VSSetShader(pVShader[ShaderName],0,0);
		Core.pD3DDevice->CreateInputLayout(D3DLayout,ARRAYSIZE(D3DLayout),pVShaderBuffer[ShaderName]->GetBufferPointer(), 
							pVShaderBuffer[ShaderName]->GetBufferSize(),&pVertexLayout);
		Core.pD3DContext->IASetInputLayout(pVertexLayout);
		CurrentVShader = ShaderName;
	}
	if(ShaderType == PIXEL && CurrentPShader != ShaderName){
		Core.pD3DContext->PSSetConstantBuffers(0,1,&pConstBuffer[ShaderName]);
		Core.pD3DContext->PSSetShader(pPShader[ShaderName],0,0);
		CurrentPShader = ShaderName;
	}
}
/*
void CEngineShader::AddEffect(std::wstring EffectName, std::wstring EffectFile,
			   std::string EntryPoint, short ShaderType, UINT ConstSize, void* pConst){
	if(pConst){
		pConstStruct[ShaderName] = pConst;
		D3D11_BUFFER_DESC CBufferDesc;	
		ZeroMemory(&CBufferDesc,sizeof(D3D11_BUFFER_DESC));
		CBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		CBufferDesc.ByteWidth = ConstSize;
		CBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		CBufferDesc.CPUAccessFlags = 0;
		CBufferDesc.MiscFlags = 0;
		Core.pD3DDevice->CreateBuffer(&CBufferDesc,NULL,&pConstBuffer[ShaderName]);
	}
	D3DX10CreateEffectFromFile(&EffectFile[0],NULL, NULL,"fx_4_0",D3D10_SHADER_ENABLE_STRICTNESS,0, 
						Core.pD3DDevice,NULL, NULL,&pEffect,&pErrorBuffer,NULL);
//	char* err = (char*)pErrorBuffer->GetBufferPointer();
	Core.pD3DDevice->CreateVertexShader(pShaderBuffer->GetBufferPointer(),pShaderBuffer->GetBufferSize(),
									NULL,&pVShader[ShaderName]);
	Core.pD3DContext->VSSetConstantBuffers(0,1,&pConstBuffer[ShaderName]);
	pVShaderBuffer[ShaderName] = pShaderBuffer;
}
*/
void CEngineShader::UpdateConst(std::wstring ShaderName){
	Core.pD3DContext->UpdateSubresource(pConstBuffer[ShaderName],0,NULL,pConstStruct[ShaderName],0,0);
}

CEngineShader::~CEngineShader(){
	pVertexLayout->Release();
	if(pErrorBuffer) pErrorBuffer->Release();
	for(std::map <std::wstring,ID3DBlob*>::iterator i = pVShaderBuffer.begin();i != pVShaderBuffer.end();i++)
		(*i).second->Release();
	for(std::map <std::wstring,ID3D11VertexShader*>::iterator i = pVShader.begin();i != pVShader.end();i++)
		(*i).second->Release();
	for(std::map <std::wstring,ID3D11PixelShader*>::iterator i = pPShader.begin();i != pPShader.end();i++)
		(*i).second->Release();
	for(std::map <std::wstring,ID3D11Buffer*>::iterator i = pConstBuffer.begin();i != pConstBuffer.end();i++)
		if((*i).second) (*i).second->Release();
}

CEngineShader Shader;