#include "CEngineRender.h"
class CTriangle3D
{
protected:
	int Count;
	float Length;
	short Depth;
public:
	void _Create(XMFLOAT3 Vertex[4],short Step,short MaxDepth);
	void Draw();
	void Load();
	void Unload();
	void Update();
	CTriangle3D(std::wstring TextureFile,float length);
};

void CTriangle3D::_Create(XMFLOAT3 Vertex[4], short Step, short MaxDepth)
{
	if(Step == MaxDepth){
		Count++;
		Render.AddVertex(D3DVertex(Vertex[2],XMFLOAT3(-0.75f,
							0.5f,0.433f),XMFLOAT2(0.5f,0.0f)));
		Render.AddVertex(D3DVertex(Vertex[1],XMFLOAT3(-0.75f,
							0.5f,0.433f),XMFLOAT2(0.0f,1.0f)));
		Render.AddVertex(D3DVertex(Vertex[0],XMFLOAT3(-0.75f,
							0.5f,0.433f),XMFLOAT2(1.0f,1.0f)));

		Render.AddVertex(D3DVertex(Vertex[3],XMFLOAT3(0,
							0.5f,-0.866f),XMFLOAT2(0.5f,0.0f)));
		Render.AddVertex(D3DVertex(Vertex[2],XMFLOAT3(0,
							0.5f,-0.866f),XMFLOAT2(1.0f,1.0f)));
		Render.AddVertex(D3DVertex(Vertex[0],XMFLOAT3(0,
							0.5f,-0.866f),XMFLOAT2(0.0f,1.0f)));

		Render.AddVertex(D3DVertex(Vertex[1],XMFLOAT3(0.75f,
							0.5f,0.433f),XMFLOAT2(0.5f,0.0f)));
		Render.AddVertex(D3DVertex(Vertex[3],XMFLOAT3(0.75f,
							0.5f,0.433f),XMFLOAT2(1.0f,1.0f)));
		Render.AddVertex(D3DVertex(Vertex[0],XMFLOAT3(0.75f,
							0.5f,0.433f),XMFLOAT2(0.0f,1.0f)));

		Render.AddVertex(D3DVertex(Vertex[1],XMFLOAT3(0,-1,0),XMFLOAT2(0.5f,0.0f)));
		Render.AddVertex(D3DVertex(Vertex[2],XMFLOAT3(0,-1,0),XMFLOAT2(1.0f,1.0f)));
		Render.AddVertex(D3DVertex(Vertex[3],XMFLOAT3(0,-1,0),XMFLOAT2(0.0f,1.0f)));
		return;
	}else
	{
		XMFLOAT3 Position[4];
		Position[0] =	Vertex[0];
		Position[1] =	XMFLOAT3((Vertex[0].x+Vertex[1].x)/2,(Vertex[0].y+Vertex[1].y)/2,(Vertex[0].z+Vertex[1].z)/2);
		Position[2] =	XMFLOAT3((Vertex[0].x+Vertex[2].x)/2,(Vertex[0].y+Vertex[2].y)/2,(Vertex[0].z+Vertex[2].z)/2);
		Position[3] =	XMFLOAT3((Vertex[0].x+Vertex[3].x)/2,(Vertex[0].y+Vertex[3].y)/2,(Vertex[0].z+Vertex[3].z)/2);
		_Create(Position,Step+1,MaxDepth);
		Position[0] =	XMFLOAT3((Vertex[0].x+Vertex[1].x)/2,(Vertex[0].y+Vertex[1].y)/2,(Vertex[0].z+Vertex[1].z)/2);
		Position[1] =	Vertex[1];
		Position[2] =	XMFLOAT3((Vertex[1].x+Vertex[2].x)/2,(Vertex[1].y+Vertex[2].y)/2,(Vertex[1].z+Vertex[2].z)/2);
		Position[3] =	XMFLOAT3((Vertex[1].x+Vertex[3].x)/2,(Vertex[1].y+Vertex[3].y)/2,(Vertex[1].z+Vertex[3].z)/2);
		_Create(Position,Step+1,MaxDepth);
		Position[0] =	XMFLOAT3((Vertex[0].x+Vertex[2].x)/2,(Vertex[0].y+Vertex[2].y)/2,(Vertex[0].z+Vertex[2].z)/2);
		Position[1] =	XMFLOAT3((Vertex[1].x+Vertex[2].x)/2,(Vertex[1].y+Vertex[2].y)/2,(Vertex[1].z+Vertex[2].z)/2);
		Position[2] =	Vertex[2];
		Position[3] =	XMFLOAT3((Vertex[2].x+Vertex[3].x)/2,(Vertex[2].y+Vertex[3].y)/2,(Vertex[2].z+Vertex[3].z)/2);
		_Create(Position,Step+1,MaxDepth);
		Position[0] =	XMFLOAT3((Vertex[0].x+Vertex[3].x)/2,(Vertex[0].y+Vertex[3].y)/2,(Vertex[0].z+Vertex[3].z)/2);
		Position[1] =	XMFLOAT3((Vertex[1].x+Vertex[3].x)/2,(Vertex[1].y+Vertex[3].y)/2,(Vertex[1].z+Vertex[3].z)/2);
		Position[2] =	XMFLOAT3((Vertex[2].x+Vertex[3].x)/2,(Vertex[2].y+Vertex[3].y)/2,(Vertex[2].z+Vertex[3].z)/2);
		Position[3] =	Vertex[3];
		_Create(Position,Step+1,MaxDepth);
	}
	return;
}

void CTriangle3D::Draw(){
	Count = 0;
	XMFLOAT3 Pos[4];
	Pos[0] = XMFLOAT3(0,sqrt(6.0f)/3*Length-1,0);
	Pos[1] = XMFLOAT3(0,-1,sqrt(3.0f)/3*Length);
	Pos[2] = XMFLOAT3(-Length/2,-1,-sqrt(3.0f)/6*Length);
	Pos[3] = XMFLOAT3( Length/2,-1,-sqrt(3.0f)/6*Length);
	Render.UseGroup(L"Triangle3D");
	Render.ClearVertex();
	_Create(Pos,0,Depth);
	return;
}

void CTriangle3D::Load(){
	Light.SetAmbientLight(0.1f);
	Render.SetBackgroundColor(XMFLOAT4(0,0,0,0));
	Light.AddPointLight("Light_1",XMFLOAT4(0,0.0f,0,0),0.5f,0.2f,3,XMFLOAT4(0.07f,0.2f,0.07f,1));
	Light.AddPointLight("Light_2",XMFLOAT4(0,0.3f,-2,0),0.8f,1.0f,3,XMFLOAT4(1,1,1,1));
	Light.AddPointLight("Light_3",XMFLOAT4(1,0.3f,0.3f,0),0.8f,1.0f,3,XMFLOAT4(1,1,1,1));
	Light.AddPointLight("Light_4",XMFLOAT4(-1,0.3f,0.3f,0),0.8f,1.0f,3,XMFLOAT4(1,1,1,1));
	Light.AddPointLight("Light_5",XMFLOAT4(0,0.0f,0,0),0.5f,0.2f,3,XMFLOAT4(0.07f,0.2f,0.07f,1));
	Draw();
	return;
}

void CTriangle3D::Unload(){
	Render.UseGroup(L"Triangle3D");
	Render.ClearVertex();
	Light.DeleteLight("Light_1");
	Light.DeleteLight("Light_2");
	Light.DeleteLight("Light_3");
	Light.DeleteLight("Light_4");
	Light.DeleteLight("Light_5");
	Camera.Reset();
	return;
}

void CTriangle3D::Update(){
	static DWORD DefaultTime = timeGetTime();
	if(Input.Keyboard(DIK_W)) Camera.MoveForward();
	if(Input.Keyboard(DIK_S)) Camera.MoveBack();
	if(Input.Keyboard(DIK_A)) Camera.MoveLeft();
	if(Input.Keyboard(DIK_D)) Camera.MoveRight();
	Camera.RotX(float(Input.Mouse(DIM_GETX))/300);
	Camera.RotY(float(Input.Mouse(DIM_GETY))/300);
	if(timeGetTime()-DefaultTime > 100){
		if(Input.Keyboard(DIK_ADD)) Depth++,Draw();
		if(Input.Keyboard(DIK_SUBTRACT)) if(Depth) Depth--,Draw();
		DefaultTime = timeGetTime();
	}
	return;
}

CTriangle3D::CTriangle3D(std::wstring TextureFile,float length){
	Render.CreateGroup(L"Triangle3D",TextureFile,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Depth = 0;
	Length = length;
	return;
}