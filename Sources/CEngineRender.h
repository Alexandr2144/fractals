#include <vector>
#include <string>
#include <map>
#include "CEngineCore.h"
#include "CEngineShader.h"
#include "CEngineCamera.h"
#include "CEngineMaterial.h"
//#include "CEngineFont.h"
//#include "CEngineInput.h"

#pragma once

struct CVertexGroup
{
	D3D11_PRIMITIVE_TOPOLOGY DrawMethod;
	std::wstring Material;
	std::vector <D3DVertex> Vertex;
	std::vector <DWORD> Index;
	bool Visible;

	void SetVisible() {Visible = true;}
	void SetInvisible() {Visible = false;}
};

class CEngineRender
{
protected:
	std::map <std::wstring,CVertexGroup> VertexGroups;
	std::wstring CurrentGroup;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	float BackgroundColor[4];

	bool BufferIsChange;
	void UpdateBuffer();

	short FPS;
	void SetFPS();
public:
	~CEngineRender();
	CEngineRender();

	void CreateGroup(std::wstring GroupName, std::wstring MaterialName, D3D11_PRIMITIVE_TOPOLOGY DrawMethod);
	void CreateGroup(std::wstring GroupName, D3D11_PRIMITIVE_TOPOLOGY DrawMethod);
	void UseGroup(std::wstring GroupName) {CurrentGroup = GroupName;}
	void AddVertex(D3DVertex vertex);
	void AddIndex(DWORD index1, DWORD index2, DWORD index3);
	void ClearVertex();
	void ClearAll();
	void ClearMemory();
	void SetBackgroundColor(float a,float r,float g,float b);
	void SetBackgroundColor(XMFLOAT4 Color);
	void Draw();
	D3D11_MAPPED_SUBRESOURCE DrawToTexture();
	short GetFPS(){return FPS;}
};

CEngineRender::CEngineRender(){
	FPS = 0;
	BackgroundColor[0] = 0;  BackgroundColor[1] = 0;
	BackgroundColor[2] = 0;  BackgroundColor[3] = 0;
	BufferIsChange = false;
	return;
}

void CEngineRender::SetBackgroundColor(XMFLOAT4 Color){
	BackgroundColor[0] = Color.x; BackgroundColor[1] = Color.y;
	BackgroundColor[2] = Color.z; BackgroundColor[3] = Color.w;
	return;
}

void CEngineRender::SetBackgroundColor(float a, float r, float g, float b){
	BackgroundColor[0] = a; BackgroundColor[1] = r;
	BackgroundColor[2] = g; BackgroundColor[3] = b;
	return;
}

void CEngineRender::CreateGroup(std::wstring GroupName, std::wstring MaterialName, D3D11_PRIMITIVE_TOPOLOGY DrawMethod)
{
	Material.AddMaterial(MaterialName);
	VertexGroups[GroupName].DrawMethod = DrawMethod;
	VertexGroups[GroupName].Material = MaterialName;
	VertexGroups[GroupName].Visible = true;
	return;
}

void CEngineRender::CreateGroup(std::wstring GroupName, D3D11_PRIMITIVE_TOPOLOGY DrawMethod)
{
	VertexGroups[GroupName].DrawMethod = DrawMethod;
	VertexGroups[GroupName].Visible = true;
	return;
}

void CEngineRender::AddVertex(D3DVertex vertex)
{
	VertexGroups[CurrentGroup].Vertex.push_back(vertex);
	BufferIsChange = true;
}

void CEngineRender::AddIndex(DWORD index1, DWORD index2, DWORD index3)
{
	VertexGroups[CurrentGroup].Index.push_back(index1);
	VertexGroups[CurrentGroup].Index.push_back(index2);
	VertexGroups[CurrentGroup].Index.push_back(index3);
	BufferIsChange = true;
}

void CEngineRender::ClearVertex()
{
	VertexGroups[CurrentGroup].Vertex.clear();
	VertexGroups[CurrentGroup].Index.clear();
	return;
}

void CEngineRender::ClearAll()
{
	for(std::map <std::wstring,CVertexGroup>::iterator pCurrentNode = VertexGroups.begin();
		pCurrentNode != VertexGroups.end(); pCurrentNode++){
		pCurrentNode->second.Vertex.clear();
		pCurrentNode->second.Index.clear();
	}
	return;
}

void CEngineRender::ClearMemory(){
	for(std::map <std::wstring,CVertexGroup>::iterator pCurrentNode = VertexGroups.begin();
		pCurrentNode != VertexGroups.end(); pCurrentNode++){
		pCurrentNode->second.Vertex.swap(pCurrentNode->second.Vertex);
		pCurrentNode->second.Index.swap(pCurrentNode->second.Index);
	}
	return;
}
void CEngineRender::Draw()
{
	if(BufferIsChange) UpdateBuffer();
	/*if(FPS < 10)*/ Camera.SetSpeed(4/float(FPS+1));
	UINT Stride = sizeof(D3DVertex);
	UINT VOffset = 0;
	UINT IOffset = 0;
	UINT Offset = 0;
	//Core.pD3DContext->RSSetState(Core.WireFrame);
	CBDefVConst.mWorld = Camera.GetMatScale();
	CBDefVConst.mView  = XMMatrixTranspose(Camera.GetMatView());
	CBDefVConst.mProj  = XMMatrixTranspose(Camera.GetMatProj());
	CBDefPConst.CamPos = Camera.GetPos();
	Shader.UpdateConst(DefVSName);
	Core.pD3DContext->ClearRenderTargetView(Core.pD3DRenderTarget, BackgroundColor);
	Core.pD3DContext->ClearDepthStencilView(Core.pDepthView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	for(std::map <std::wstring,CVertexGroup>::iterator pCurrentNode = VertexGroups.begin();
		pCurrentNode != VertexGroups.end(); pCurrentNode++){
		if(pCurrentNode->second.Visible){
		Core.pD3DContext->IASetPrimitiveTopology(pCurrentNode->second.DrawMethod);
		Material.UseMaterial(pCurrentNode->second.Material);
		if(pCurrentNode->second.Index.size()){
			Core.pD3DContext->IASetVertexBuffers(0,1,&pVertexBuffer,&Stride,&Offset);
			Core.pD3DContext->IASetIndexBuffer(pIndexBuffer,DXGI_FORMAT_R32_UINT,0);
			Core.pD3DContext->DrawIndexed(pCurrentNode->second.Index.size(),IOffset,VOffset);
			VOffset += pCurrentNode->second.Vertex.size();
			IOffset += pCurrentNode->second.Index.size();
		}else{
			Core.pD3DContext->IASetVertexBuffers(0,1,&pVertexBuffer,&Stride,&Offset);
			Core.pD3DContext->Draw(pCurrentNode->second.Vertex.size(),VOffset);
			VOffset += pCurrentNode->second.Vertex.size();
		}}
	}
	Core.pD3DSwapChain->Present(0,0);
	SetFPS();
	return;
}

D3D11_MAPPED_SUBRESOURCE CEngineRender::DrawToTexture()
{
	if(BufferIsChange) UpdateBuffer();
	///*if(FPS > 10) */Camera.SetSpeed(4/float(FPS+1));
	UINT Stride = sizeof(D3DVertex);
	UINT VOffset = 0;
	UINT IOffset = 0;
	UINT Offset = 0;
	//Core.pD3DContext->RSSetState(Core.WireFrame);
	CBDefVConst.mWorld = Camera.GetMatScale();
	CBDefVConst.mView  = XMMatrixTranspose(Camera.GetMatView());
	CBDefVConst.mProj  = XMMatrixTranspose(Camera.GetMatProj());
	CBDefPConst.CamPos = Camera.GetPos();
	Shader.UpdateConst(DefVSName);
	Core.pD3DContext->ClearRenderTargetView(Core.pD3DRenderTarget, BackgroundColor);
	Core.pD3DContext->ClearDepthStencilView(Core.pDepthView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	for(std::map <std::wstring,CVertexGroup>::iterator pCurrentNode = VertexGroups.begin();
		pCurrentNode != VertexGroups.end(); pCurrentNode++){
		if(pCurrentNode->second.Visible){
		Core.pD3DContext->IASetPrimitiveTopology(pCurrentNode->second.DrawMethod);
		Material.UseMaterial(pCurrentNode->second.Material);
		if(pCurrentNode->second.Index.size()){
			Core.pD3DContext->IASetVertexBuffers(0,1,&pVertexBuffer,&Stride,&Offset);
			Core.pD3DContext->IASetIndexBuffer(pIndexBuffer,DXGI_FORMAT_R32_UINT,0);
			Core.pD3DContext->DrawIndexed(pCurrentNode->second.Index.size(),IOffset,VOffset);
			VOffset += pCurrentNode->second.Vertex.size();
			IOffset += pCurrentNode->second.Index.size();
		}else{
			Core.pD3DContext->IASetVertexBuffers(0,1,&pVertexBuffer,&Stride,&Offset);
			Core.pD3DContext->Draw(pCurrentNode->second.Vertex.size(),VOffset);
			VOffset += pCurrentNode->second.Vertex.size();
		}}
	}
	D3D11_MAPPED_SUBRESOURCE ms;
	ID3D11Resource* res;
	Core.pD3DRenderTarget->GetResource(&res);
	HRESULT hr1 = Core.pD3DContext->Map(res,0,D3D11_MAP_READ,NULL,&ms);
	HRESULT hr2 = Core.pD3DContext->Map(res,0,D3D11_MAP_WRITE,NULL,&ms);
	HRESULT hr3 = Core.pD3DRenderTarget->GetPrivateData(__uuidof(ID3D11Texture2D),&ms.DepthPitch,ms.pData);
	int i;
	i = 0;
	return ms;
}

void CEngineRender::SetFPS()
{
	static DWORD DefaultTime = timeGetTime();
	static short Count = 0;
	if(timeGetTime()-DefaultTime > 1000){
		FPS = Count;
		Count = 0;
		DefaultTime = timeGetTime();
	}
	else
		Count++;
	return;
}

void CEngineRender::UpdateBuffer(){
	std::vector <D3DVertex> Vertices;
	std::vector <DWORD> Indexes;
	DWORD CurrentIndex = 0;
	for(std::map <std::wstring,CVertexGroup>::iterator pCurrentNode = VertexGroups.begin();
		pCurrentNode != VertexGroups.end(); pCurrentNode++){
		if(pCurrentNode->second.Visible){
			for(size_t i = 0; i < pCurrentNode->second.Vertex.size(); i++)
				Vertices.push_back(pCurrentNode->second.Vertex[i]);
			for(size_t i = 0; i < pCurrentNode->second.Index.size(); i++)
				Indexes.push_back(pCurrentNode->second.Index[i]);
		}
	}
	if(Vertices.size()){
	D3D11_BUFFER_DESC VBDesc;
	ZeroMemory(&VBDesc,sizeof(VBDesc));
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.ByteWidth = sizeof(D3DVertex)*Vertices.size();
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA VBData; 
	ZeroMemory(&VBData,sizeof(VBData));
	VBData.pSysMem = &Vertices[0];
	Core.pD3DDevice->CreateBuffer(&VBDesc,&VBData,&pVertexBuffer);}
	if(Indexes.size()){
	D3D11_BUFFER_DESC IBDesc;
	ZeroMemory(&IBDesc,sizeof(IBDesc));
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.ByteWidth = sizeof(DWORD)*Indexes.size();
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA IBData;
	ZeroMemory(&IBData,sizeof(IBData));
	IBData.pSysMem = &Indexes[0];
	Core.pD3DDevice->CreateBuffer(&IBDesc,&IBData,&pIndexBuffer);
	Core.pD3DContext->IASetIndexBuffer(pIndexBuffer,DXGI_FORMAT_R32_UINT,0);
	BufferIsChange = false;}
}

CEngineRender::~CEngineRender(){
	if(pVertexBuffer) pVertexBuffer->Release();
	if(pIndexBuffer) pIndexBuffer->Release();
}

CEngineRender Render;