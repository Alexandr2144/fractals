#include <string>
#include "CEngineCore.h"

#pragma once

class CEngineMaterial
{
public:
//protected:
	std::map <std::wstring,ID3D11ShaderResourceView*> pTextureRV;
	std::map <std::wstring,ID3D11SamplerState*> pTextureSampler;
//public:
	void AddMaterialFromMemory(std::wstring MaterialName,void* srcData,size_t srcWidth,size_t scrHeight);
	void AddMaterial(std::wstring FileName);
	void UseMaterial(std::wstring FileName);
	~CEngineMaterial();
};


void CEngineMaterial::AddMaterialFromMemory(std::wstring MaterialName,void* srcData,size_t srcWidth,size_t scrHeight){
	//if(pTextureRV.count(MaterialName)) return;
	D3D11_TEXTURE2D_DESC TexDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC ViewDesc;
	D3D11_SUBRESOURCE_DATA SubData;
	ZeroMemory(&TexDesc, sizeof(TexDesc));
	SubData.pSysMem = srcData;
	SubData.SysMemPitch = srcWidth;
	SubData.SysMemSlicePitch = srcWidth*scrHeight;
	TexDesc.Width = srcWidth;
	TexDesc.Height = scrHeight;
	TexDesc.ArraySize = 1;
	TexDesc.MipLevels = 1;
	ViewDesc.Texture2D.MipLevels = 1;
	TexDesc.Format  = DXGI_FORMAT_R8G8B8A8_UNORM;
	ViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TexDesc.SampleDesc.Count = 1;
	TexDesc.Usage = D3D11_USAGE_STAGING;
	TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TexDesc.MiscFlags = 0;
	ViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ViewDesc.Texture2D.MostDetailedMip = 0;

	ID3D11Texture2D* pTexture = NULL;
	Core.pD3DDevice->CreateTexture2D(&TexDesc,&SubData,&pTexture);
	Core.pD3DDevice->CreateShaderResourceView(pTexture,&ViewDesc,&pTextureRV[MaterialName]);
	D3D11_SAMPLER_DESC SampDesc;
	ZeroMemory(&SampDesc,sizeof(SampDesc));
	SampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    SampDesc.MinLOD = 0;
    SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	Core.pD3DDevice->CreateSamplerState(&SampDesc,&pTextureSampler[MaterialName]);
}


void CEngineMaterial::AddMaterial(std::wstring FileName){
	if(pTextureRV.count(FileName)) return;
	D3DX11CreateShaderResourceViewFromFile(Core.pD3DDevice,&FileName[0],NULL,NULL,&pTextureRV[FileName],NULL);
	D3D11_SAMPLER_DESC SampDesc;
	ZeroMemory(&SampDesc,sizeof(SampDesc));
	SampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    SampDesc.MinLOD = 0;
    SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	Core.pD3DDevice->CreateSamplerState(&SampDesc,&pTextureSampler[FileName]);

	ID3D11Resource* Resource;
	D3D11_MAPPED_SUBRESOURCE SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	pTextureRV[FileName]->GetResource(&Resource);
	HRESULT hr = Core.pD3DContext->Map(Resource,0,D3D11_MAP_WRITE_DISCARD,0,&SubResource);
}

void CEngineMaterial::UseMaterial(std::wstring FileName){
	Core.pD3DContext->PSSetShaderResources(0,1,&pTextureRV[FileName]);
	Core.pD3DContext->PSSetSamplers(0,1,&pTextureSampler[FileName]);
}

CEngineMaterial::~CEngineMaterial(){
	for(std::map <std::wstring,ID3D11ShaderResourceView*>::iterator i = pTextureRV.begin();
		i != pTextureRV.end(); i++) if(i->second) i->second->Release();
	for(std::map <std::wstring,ID3D11SamplerState*>::iterator i = pTextureSampler.begin();
		i != pTextureSampler.end(); i++) if(i->second) i->second->Release();
}

CEngineMaterial Material;