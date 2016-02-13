#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

#define DefVSName		L"BaseVShader.hlsl"
#define DefVSFile		L"BaseShaders//BaseVShader.hlsl"
#define	DefVSEntryPoint "main"
#define DefPSName		L"BasePShader.hlsl"
#define DefPSFile		L"BaseShaders//BasePShader.hlsl"
#define	DefPSEntryPoint "main"

#define D3DPT_TRIANGLELIST D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
#define D3DPT_TRIANGLESTRIP D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
#define D3DPT_LINELIST D3D11_PRIMITIVE_TOPOLOGY_LINELIST
#define D3DPT_LINESTRIP D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP


#define INIT \
MSG msg;\
ZeroMemory(&msg, sizeof(MSG));\
Core._Init(hInstance,WndProc,nShowCmd);\
Shader._Init();\
Input._Init(hInstance);

#define BEGIN \
while(true){ \
BOOL PeekMessageL( LPMSG lpMsg,HWND hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg); \
if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){ \
if (msg.message == WM_QUIT) \
break; \
TranslateMessage(&msg);	\
DispatchMessage(&msg);}else{\
Input._Acquire();

#define END \
Camera._Update();\
Light._Update();\
Render.Draw();\
}}return msg.wParam;
