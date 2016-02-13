#include <stdlib.h>

class CPlane3D
{
protected:
	float Length;
	short Depth;
	int _IndexCnt;
public:
	void _Create(XMFLOAT3 Vertex[4],short Step,short MaxDepth);
	void Draw();
	void Load();
	void Unload();
	void Update();
	CPlane3D(std::wstring TextureFile,float length);
};
float median = 0;
void CPlane3D::_Create(XMFLOAT3 Vertex[4],short Step,short MaxDepth){
	if(Step == MaxDepth){
		XMFLOAT3 Normal;
		XMStoreFloat3(&Normal,XMVector3Normalize(XMVector3Cross(
			XMLoadFloat3(Vertex)-XMLoadFloat3(Vertex+1),
			XMLoadFloat3(Vertex)-XMLoadFloat3(Vertex+2)	)));
		Render.AddVertex(D3DVertex(Vertex[0],Normal,XMFLOAT2(1,1)));
		Render.AddVertex(D3DVertex(Vertex[1],Normal,XMFLOAT2(0,1)));
		Render.AddVertex(D3DVertex(Vertex[2],Normal,XMFLOAT2(0,0)));
		XMStoreFloat3(&Normal,XMVector3Normalize(XMVector3Cross(
			XMLoadFloat3(Vertex)-XMLoadFloat3(Vertex+2),
			XMLoadFloat3(Vertex)-XMLoadFloat3(Vertex+3)	)));
		Render.AddVertex(D3DVertex(Vertex[0],Normal,XMFLOAT2(1,1)));
		Render.AddVertex(D3DVertex(Vertex[2],Normal,XMFLOAT2(0,0)));
		Render.AddVertex(D3DVertex(Vertex[3],Normal,XMFLOAT2(1,0)));
		_IndexCnt++;
	}else
	{
		XMFLOAT3 Pos[9];
		Pos[0] = Vertex[0];
		Pos[1] = XMFLOAT3(Vertex[0].x,(Vertex[0].y+Vertex[1].y)/2,(Vertex[0].z+Vertex[1].z)/2);
		Pos[2] = Vertex[1];
		Pos[3] = XMFLOAT3((Vertex[0].x+Vertex[3].x)/2,(Vertex[0].y+Vertex[3].y)/2,Vertex[0].z);
		Pos[5] = XMFLOAT3((Vertex[1].x+Vertex[2].x)/2,(Vertex[1].y+Vertex[2].y)/2,Vertex[1].z);
		Pos[6] = Vertex[3];
		Pos[7] = XMFLOAT3(Vertex[2].x,(Vertex[2].y+Vertex[3].y)/2,(Vertex[2].z+Vertex[3].z)/2);
		Pos[8] = Vertex[2];
		float Center = (Pos[1].y+Pos[3].y+Pos[5].y+Pos[7].y)/4
			+(float(rand()%20000)-7000)/2000/(Step+1)/(Step+1);
		Pos[4] = XMFLOAT3((Vertex[0].x+Vertex[3].x)/2,Center,(Vertex[0].z+Vertex[2].z)/2);
		Vertex[0] = Pos[0];
		Vertex[1] = Pos[1];
		Vertex[2] = Pos[4];
		Vertex[3] = Pos[3];
		_Create(Vertex,Step+1,MaxDepth);
		Vertex[0] = Pos[1];
		Vertex[1] = Pos[2];
		Vertex[2] = Pos[5];
		Vertex[3] = Pos[4];
		_Create(Vertex,Step+1,MaxDepth);
		Vertex[0] = Pos[3];
		Vertex[1] = Pos[4];
		Vertex[2] = Pos[7];
		Vertex[3] = Pos[6];
		_Create(Vertex,Step+1,MaxDepth);
		Vertex[0] = Pos[4];
		Vertex[1] = Pos[5];
		Vertex[2] = Pos[8];
		Vertex[3] = Pos[7];
		_Create(Vertex,Step+1,MaxDepth);
	}
}

void CPlane3D::Draw(){
	_IndexCnt = 0;
	XMFLOAT3 Pos[4];
	Pos[0] = XMFLOAT3(-Length*0.5f,(float(rand()%100))/50-2,-Length*0.5f);
	Pos[1] = XMFLOAT3(-Length*0.5f,(float(rand()%100))/50-2, Length*0.5f);
	Pos[2] = XMFLOAT3( Length*0.5f,(float(rand()%100))/50-2, Length*0.5f);
	Pos[3] = XMFLOAT3( Length*0.5f,(float(rand()%100))/50-2,-Length*0.5f);
	Render.UseGroup(L"Plane3D");
	Render.ClearVertex();
	_Create(Pos,0,Depth);
	return;
}

void CPlane3D::Load(){
	Light.SetAmbientLight(0);
	Render.SetBackgroundColor(XMFLOAT4(0,0,0,0));
	Light.AddPointLight("Light_1",XMFLOAT4( 0,1, 0,0),0.1f,0.1f,10,XMFLOAT4(1,1,1,1));
	Light.AddPointLight("Light_2",XMFLOAT4( 4,1, 4,0),0.1f,0.1f,10,XMFLOAT4(1,1,1,1));
	Light.AddPointLight("Light_3",XMFLOAT4( 4,1,-4,0),0.1f,0.1f,10,XMFLOAT4(1,1,1,1));
	Light.AddPointLight("Light_4",XMFLOAT4(-4,1,-4,0),0.1f,0.1f,10,XMFLOAT4(1,1,1,1));
	Light.AddPointLight("Light_5",XMFLOAT4(-4,1, 4,0),0.1f,0.1f,10,XMFLOAT4(1,1,1,1));
	Draw();
}

void CPlane3D::Unload(){
	Render.UseGroup(L"Plane3D");
	Render.ClearVertex();
	Light.DeleteLight("Light_1");
	Light.DeleteLight("Light_2");
	Light.DeleteLight("Light_3");
	Light.DeleteLight("Light_4");
	Light.DeleteLight("Light_5");
	Shader.UseDefaultShader();
	Camera.Reset();
}

void CPlane3D::Update(){
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
}

CPlane3D::CPlane3D(std::wstring TextureFile,float length){
	Render.CreateGroup(L"Plane3D",TextureFile,D3DPT_TRIANGLELIST);
	Depth = 0;
	Length = length;
	return;
}