#include <Windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <DXGI.h>
#include <vector>
#include "Resource.h"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3dx11d.lib")
#pragma comment(lib,"DXGI.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"Winmm.lib")

#define WIDTH 1366;
#define HEIGHT 768;

#pragma once

struct D3DVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
	D3DVertex(){}
	D3DVertex(float _x, float _y, float _z,float _xn, float _yn, float _zn,float _u, float _v)
		: Pos(_x,_y,_z),Normal(_xn,_yn,_zn),TexCoord(_u,_v){}
	D3DVertex(XMFLOAT3 _pos,XMFLOAT3 _normal,float _u, float _v)
		: Pos(_pos),Normal(_normal),TexCoord(_u,_v){}
	D3DVertex(XMFLOAT3 _pos,XMFLOAT3 _normal,XMFLOAT2 _texcoord)
		: Pos(_pos),Normal(_normal),TexCoord(_texcoord){}
};

D3D11_INPUT_ELEMENT_DESC D3DLayout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },  
};

class CEngineCore
{
public:
	IDXGISwapChain* pD3DSwapChain;
	ID3D11Device* pD3DDevice;
	ID3D11DeviceContext* pD3DContext;
	ID3D11RenderTargetView* pD3DRenderTarget;
	ID3D11DepthStencilView* pDepthView;
	ID3D11RasterizerState* WireFrame;
	ID3D11Texture2D* pDepthBuffer;
	HWND hMainWnd;

	void _Init(HINSTANCE hInstance,WNDPROC WndProc,int nShowCmd);
	~CEngineCore();
};

void CEngineCore::_Init(HINSTANCE hInstance,WNDPROC WndProc,int nShowCmd){
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.style = CS_HREDRAW|CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = NULL;
	WndClass.cbWndExtra = NULL;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+2);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = L"SomeWndClass";
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&WndClass)){
		MessageBox(NULL,L"Ошибка регистрации оконного класса",L"Ошибка",MB_OK|MB_ICONERROR);
		return;}
	hMainWnd = CreateWindowEx(NULL,L"SomeWndClass",L"Fractal",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT,
		1366, 768,NULL,NULL,hInstance,NULL);
	if (!hMainWnd){
		MessageBox(NULL,L"Невозможно создать окно",L"Ошибка",MB_OK|MB_ICONERROR);
		return;}
	ShowWindow(hMainWnd,nShowCmd);
	UpdateWindow(hMainWnd);

	DXGI_MODE_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(DXGI_MODE_DESC));
	BufferDesc.Width = WIDTH;
	BufferDesc.Height = HEIGHT;
	BufferDesc.RefreshRate.Numerator = 60;
	BufferDesc.RefreshRate.Denominator = 1;
	BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC SwapChainDesc; 
	ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	SwapChainDesc.BufferDesc = BufferDesc;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = hMainWnd; 
	SwapChainDesc.Windowed = TRUE; 
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGIFactory* pDXGIFactory;
	CreateDXGIFactory(__uuidof(IDXGIFactory),(void**)&pDXGIFactory);
	IDXGIAdapter* pAdapter;
	pDXGIFactory->EnumAdapters(0,&pAdapter);
	pDXGIFactory->Release();	

	D3D11CreateDeviceAndSwapChain(pAdapter,D3D_DRIVER_TYPE_UNKNOWN,NULL,NULL,NULL,NULL,
		D3D11_SDK_VERSION,&SwapChainDesc,&pD3DSwapChain,&pD3DDevice,NULL,&pD3DContext);

	if(pAdapter) pAdapter->Release();
	
	ID3D11Texture2D* BackBuffer = NULL;
	pD3DSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&BackBuffer);
	pD3DDevice->CreateRenderTargetView(BackBuffer,NULL,&pD3DRenderTarget);
	BackBuffer->Release();

	D3D11_TEXTURE2D_DESC DepthDesc;
	DepthDesc.Width = WIDTH;
	DepthDesc.Height = HEIGHT;
	DepthDesc.MipLevels = 1;
	DepthDesc.ArraySize = 1;
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthDesc.SampleDesc.Count = 1;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthDesc.CPUAccessFlags = 0; 
	DepthDesc.MiscFlags = 0;

	D3D11_RASTERIZER_DESC StateDesc;
	ZeroMemory(&StateDesc, sizeof(D3D11_RASTERIZER_DESC));
	StateDesc.FillMode = D3D11_FILL_WIREFRAME;
	StateDesc.CullMode = D3D11_CULL_NONE;
	pD3DDevice->CreateRasterizerState(&StateDesc, &WireFrame);

	D3D11_VIEWPORT ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3D11_VIEWPORT));
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = WIDTH;
	ViewPort.Height = HEIGHT;
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;
	pD3DContext->RSSetViewports(1,&ViewPort);
	pD3DDevice->CreateTexture2D(&DepthDesc,NULL,&pDepthBuffer);
	pD3DDevice->CreateDepthStencilView(pDepthBuffer,NULL,&pDepthView);
	pD3DContext->OMSetRenderTargets(1,&pD3DRenderTarget,pDepthView);
	return;
}


CEngineCore::~CEngineCore(){
	pD3DSwapChain->Release();
	pD3DDevice->Release();
	pD3DContext->Release();
	pD3DRenderTarget->Release();
	pDepthView->Release();
	pDepthBuffer->Release();
}

CEngineCore Core;