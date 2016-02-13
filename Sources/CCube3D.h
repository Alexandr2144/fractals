class CCube3D
{
protected:
	int Count;
	float Length;
	short Depth;
	int _IndexCnt;
public:
	void _Create(XMFLOAT3 Vertex[2],short Step,short MaxDepth,short Cull);
	void Draw();
	void Load();
	void Unload();
	void Update();
	CCube3D(wchar_t* TextureFile,float length);
};

void CCube3D::_Create(XMFLOAT3 Vertex[2], short Step, short MaxDepth,short Cull)
{
	if(Step == MaxDepth){
		Count++;
		if(!(Cull&1)){
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[1].x,Vertex[0].y,Vertex[1].z),XMFLOAT3(0,-1,0),XMFLOAT2(1,1)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[0].x,Vertex[0].y,Vertex[1].z),XMFLOAT3(0,-1,0),XMFLOAT2(0,1)));
			Render.AddVertex(D3DVertex(Vertex[0],XMFLOAT3(0,-1,0),XMFLOAT2(0,0)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[1].x,Vertex[0].y,Vertex[0].z),XMFLOAT3(0,-1,0),XMFLOAT2(1,0)));
			Render.AddIndex(4*_IndexCnt,1+4*_IndexCnt,2+4*_IndexCnt);
			Render.AddIndex(4*_IndexCnt,2+4*_IndexCnt,3+4*_IndexCnt);
			_IndexCnt++;}
		if(!(Cull&2)){
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[0].x,Vertex[1].y,Vertex[0].z),XMFLOAT3(0,1,0),XMFLOAT2(0,0)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[0].x,Vertex[1].y,Vertex[1].z),XMFLOAT3(0,1,0),XMFLOAT2(0,1)));
			Render.AddVertex(D3DVertex(Vertex[1],XMFLOAT3(0,1,0),XMFLOAT2(1,1)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[1].x,Vertex[1].y,Vertex[0].z),XMFLOAT3(0,1,0),XMFLOAT2(1,0)));
			Render.AddIndex(4*_IndexCnt,1+4*_IndexCnt,2+4*_IndexCnt);
			Render.AddIndex(4*_IndexCnt,2+4*_IndexCnt,3+4*_IndexCnt);
			_IndexCnt++;}
		if(!(Cull&4)){
			Render.AddVertex(D3DVertex(Vertex[0],XMFLOAT3(0,0,-1),XMFLOAT2(0,0)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[0].x,Vertex[1].y,Vertex[0].z),XMFLOAT3(0,0,-1),XMFLOAT2(0,1)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[1].x,Vertex[1].y,Vertex[0].z),XMFLOAT3(0,0,-1),XMFLOAT2(1,1)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[1].x,Vertex[0].y,Vertex[0].z),XMFLOAT3(0,0,-1),XMFLOAT2(1,0)));
			Render.AddIndex(4*_IndexCnt,1+4*_IndexCnt,2+4*_IndexCnt);
			Render.AddIndex(4*_IndexCnt,2+4*_IndexCnt,3+4*_IndexCnt);
			_IndexCnt++;}
		if(!(Cull&8)){
			Render.AddVertex(D3DVertex(Vertex[1],XMFLOAT3(0,0,1),XMFLOAT2(1,1)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[0].x,Vertex[1].y,Vertex[1].z),XMFLOAT3(0,0,1),XMFLOAT2(0,1)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[0].x,Vertex[0].y,Vertex[1].z),XMFLOAT3(0,0,1),XMFLOAT2(0,0)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[1].x,Vertex[0].y,Vertex[1].z),XMFLOAT3(0,0,1),XMFLOAT2(1,0)));
			Render.AddIndex(4*_IndexCnt,1+4*_IndexCnt,2+4*_IndexCnt);
			Render.AddIndex(4*_IndexCnt,2+4*_IndexCnt,3+4*_IndexCnt);
			_IndexCnt++;}
		if(!(Cull&16)){
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[0].x,Vertex[1].y,Vertex[1].z),XMFLOAT3(-1,0,0),XMFLOAT2(1,1)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[0].x,Vertex[1].y,Vertex[0].z),XMFLOAT3(-1,0,0),XMFLOAT2(0,1)));
			Render.AddVertex(D3DVertex(Vertex[0],XMFLOAT3(-1,0,0),XMFLOAT2(0,0)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[0].x,Vertex[0].y,Vertex[1].z),XMFLOAT3(-1,0,0),XMFLOAT2(1,0)));
			Render.AddIndex(4*_IndexCnt,1+4*_IndexCnt,2+4*_IndexCnt);
			Render.AddIndex(4*_IndexCnt,2+4*_IndexCnt,3+4*_IndexCnt);
			_IndexCnt++;}
		if(!(Cull&32)){
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[1].x,Vertex[0].y,Vertex[0].z),XMFLOAT3(1,0,0),XMFLOAT2(0,0)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[1].x,Vertex[1].y,Vertex[0].z),XMFLOAT3(1,0,0),XMFLOAT2(0,1)));
			Render.AddVertex(D3DVertex(Vertex[1],XMFLOAT3(1,0,0),XMFLOAT2(1,1)));
			Render.AddVertex(D3DVertex(XMFLOAT3(Vertex[1].x,Vertex[0].y,Vertex[1].z),XMFLOAT3(1,0,0),XMFLOAT2(1,0)));
			Render.AddIndex(4*_IndexCnt,1+4*_IndexCnt,2+4*_IndexCnt);
			Render.AddIndex(4*_IndexCnt,2+4*_IndexCnt,3+4*_IndexCnt);
			_IndexCnt++;}
	}else
	{
		short Flag[20];
		ZeroMemory(Flag,40);
		Flag[0] = Flag[0]|(Cull&1);    Flag[1] = Flag[1]|(Cull&1);    Flag[2] = Flag[2]|(Cull&1);    Flag[3] = Flag[3]|(Cull&1);
		Flag[4] = Flag[4]|(Cull&1);    Flag[5] = Flag[5]|(Cull&1);    Flag[6] = Flag[6]|(Cull&1);    Flag[7] = Flag[7]|(Cull&1);
		Flag[12] = Flag[12]|(Cull&2);  Flag[14] = Flag[14]|(Cull&2);  Flag[16] = Flag[16]|(Cull&2);  Flag[18] = Flag[18]|(Cull&2);
		Flag[13] = Flag[13]|(Cull&2);  Flag[15] = Flag[15]|(Cull&2);  Flag[17] = Flag[17]|(Cull&2);  Flag[19] = Flag[19]|(Cull&2);
		Flag[0] = Flag[0]|(Cull&4);    Flag[1] = Flag[1]|(Cull&4);    Flag[2] = Flag[2]|(Cull&4);    Flag[8] = Flag[8]|(Cull&4);
		Flag[12] = Flag[12]|(Cull&4);  Flag[14] = Flag[14]|(Cull&4);  Flag[16] = Flag[16]|(Cull&4);  Flag[9] = Flag[9]|(Cull&4);
		Flag[5] = Flag[5]|(Cull&8);    Flag[6] =  Flag[6]|(Cull&8);   Flag[7] =  Flag[7]|(Cull&8);   Flag[10] = Flag[10]|(Cull&8);
		Flag[15] = Flag[15]|(Cull&8);  Flag[17] = Flag[17]|(Cull&8);  Flag[19] = Flag[19]|(Cull&8);  Flag[11] = Flag[11]|(Cull&8);
		Flag[0] =  Flag[0]|(Cull&16);  Flag[3] =  Flag[3]|(Cull&16);  Flag[5] =  Flag[5]|(Cull&16);  Flag[8] = Flag[8]|(Cull&16);
		Flag[12] = Flag[12]|(Cull&16); Flag[15] = Flag[15]|(Cull&16); Flag[17] = Flag[17]|(Cull&16); Flag[10] = Flag[10]|(Cull&16);
		Flag[2] =  Flag[2]|(Cull&32);  Flag[4] =  Flag[4]|(Cull&32);  Flag[7] =  Flag[7]|(Cull&32);  Flag[9] = Flag[9]|(Cull&32);
		Flag[14] = Flag[14]|(Cull&32); Flag[16] = Flag[16]|(Cull&32); Flag[19] = Flag[19]|(Cull&32); Flag[11] = Flag[11]|(Cull&32);

		float a = (Vertex[1].x-Vertex[0].x)/3;
		XMFLOAT3 Position[2];
		Position[0] =	XMFLOAT3(Vertex[0].x,Vertex[0].y,Vertex[0].z);
		Position[1] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y+a,Vertex[0].z+a);
		_Create(Position,Step+1,MaxDepth,42|Flag[0]);
		Position[0] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y,Vertex[0].z);
		Position[1] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y+a,Vertex[0].z+a);
		_Create(Position,Step+1,MaxDepth,48|Flag[1]);
		Position[0] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y,Vertex[0].z);
		Position[1] =	XMFLOAT3(Vertex[0].x+3*a,Vertex[0].y+a,Vertex[0].z+a);
		_Create(Position,Step+1,MaxDepth,26|Flag[2]);

		Position[0] =	XMFLOAT3(Vertex[0].x,Vertex[0].y,Vertex[0].z+a);
		Position[1] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y+a,Vertex[0].z+2*a);
		_Create(Position,Step+1,MaxDepth,12|Flag[3]);
		Position[0] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y,Vertex[0].z+a);
		Position[1] =	XMFLOAT3(Vertex[0].x+3*a,Vertex[0].y+a,Vertex[0].z+2*a);
		_Create(Position,Step+1,MaxDepth,12|Flag[4]);

		Position[0] =	XMFLOAT3(Vertex[0].x,Vertex[0].y,Vertex[0].z+2*a);
		Position[1] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y+a,Vertex[0].z+3*a);
		_Create(Position,Step+1,MaxDepth,38|Flag[5]);
		Position[0] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y,Vertex[0].z+2*a);
		Position[1] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y+a,Vertex[0].z+3*a);
		_Create(Position,Step+1,MaxDepth,48|Flag[6]);
		Position[0] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y,Vertex[0].z+2*a);
		Position[1] =	XMFLOAT3(Vertex[0].x+3*a,Vertex[0].y+a,Vertex[0].z+3*a);
		_Create(Position,Step+1,MaxDepth,22|Flag[7]);
//////////////////////////////////
		Position[0] =	XMFLOAT3(Vertex[0].x,Vertex[0].y+a,Vertex[0].z);
		Position[1] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y+2*a,Vertex[0].z+a);
		_Create(Position,Step+1,MaxDepth,3|Flag[8]);
		Position[0] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y+a,Vertex[0].z);
		Position[1] =	XMFLOAT3(Vertex[0].x+3*a,Vertex[0].y+2*a,Vertex[0].z+a);
		_Create(Position,Step+1,MaxDepth,3|Flag[9]);

		Position[0] =	XMFLOAT3(Vertex[0].x,Vertex[0].y+a,Vertex[0].z+2*a);
		Position[1] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y+2*a,Vertex[0].z+3*a);
		_Create(Position,Step+1,MaxDepth,3|Flag[10]);
		Position[0] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y+a,Vertex[0].z+2*a);
		Position[1] =	XMFLOAT3(Vertex[0].x+3*a,Vertex[0].y+2*a,Vertex[0].z+3*a);
		_Create(Position,Step+1,MaxDepth,3|Flag[11]);
/////////////////////////////////////
		Position[0] =	XMFLOAT3(Vertex[0].x,Vertex[0].y+2*a,Vertex[0].z);
		Position[1] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y+3*a,Vertex[0].z+a);
		_Create(Position,Step+1,MaxDepth,41|Flag[12]);
		Position[0] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y+2*a,Vertex[0].z);
		Position[1] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y+3*a,Vertex[0].z+a);
		_Create(Position,Step+1,MaxDepth,48|Flag[13]);
		Position[0] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y+2*a,Vertex[0].z);
		Position[1] =	XMFLOAT3(Vertex[0].x+3*a,Vertex[0].y+3*a,Vertex[0].z+a);
		_Create(Position,Step+1,MaxDepth,25|Flag[14]);

		Position[0] =	XMFLOAT3(Vertex[0].x,Vertex[0].y+2*a,Vertex[0].z+a);
		Position[1] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y+3*a,Vertex[0].z+2*a);
		_Create(Position,Step+1,MaxDepth,12|Flag[15]);
		Position[0] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y+2*a,Vertex[0].z+a);
		Position[1] =	XMFLOAT3(Vertex[0].x+3*a,Vertex[0].y+3*a,Vertex[0].z+2*a);
		_Create(Position,Step+1,MaxDepth,12|Flag[16]);

		Position[0] =	XMFLOAT3(Vertex[0].x,Vertex[0].y+2*a,Vertex[0].z+2*a);
		Position[1] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y+3*a,Vertex[0].z+3*a);
		_Create(Position,Step+1,MaxDepth,37|Flag[17]);
		Position[0] =	XMFLOAT3(Vertex[0].x+a,Vertex[0].y+2*a,Vertex[0].z+2*a);
		Position[1] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y+3*a,Vertex[0].z+3*a);
		_Create(Position,Step+1,MaxDepth,48|Flag[18]);
		Position[0] =	XMFLOAT3(Vertex[0].x+2*a,Vertex[0].y+2*a,Vertex[0].z+2*a);
		Position[1] =	XMFLOAT3(Vertex[0].x+3*a,Vertex[0].y+3*a,Vertex[0].z+3*a);
		_Create(Position,Step+1,MaxDepth,21|Flag[19]);
	}
	return;
}

void CCube3D::Draw(){
	Count = 0;
	_IndexCnt = 0;
	XMFLOAT3 Pos[2];
	Pos[0] = XMFLOAT3(-Length*0.5f,-Length*0.5f,-Length*0.5f);
	Pos[1] = XMFLOAT3(Length*0.5f,Length*0.5f,Length*0.5f);
	Render.UseGroup(L"Cube3D");
	Render.ClearVertex();
	_Create(Pos,0,Depth,0);
	return;
}

void CCube3D::Load(){
	Light.SetAmbientLight(0.1f);
	Render.SetBackgroundColor(XMFLOAT4(0,0,0,0));
	Light.AddPointLight("Light_1",XMFLOAT4(0,0,0,0),0.5f,0.2f,3,XMFLOAT4(0.2f,0.2f,0.5f,1));
	Light.AddPointLight("Light_2",XMFLOAT4(Length,0,0,0),0.5f,1.0f,3,XMFLOAT4(1,1,1,1));
	Light.AddPointLight("Light_3",XMFLOAT4(-Length,0,0,0),0.5f,1.0f,3,XMFLOAT4(1,1,1,1));
	Light.AddPointLight("Light_4",XMFLOAT4(0,0,Length,0),0.5f,1.0f,3,XMFLOAT4(1,1,1,1));
	Light.AddPointLight("Light_5",XMFLOAT4(0,0,-Length,0),0.5f,1.0f,3,XMFLOAT4(1,1,1,1));
	Draw();
	return;
}

void CCube3D::Unload(){
	Render.UseGroup(L"Cube3D");
	Render.ClearVertex();
	Light.DeleteLight("Light_1");
	Light.DeleteLight("Light_2");
	Light.DeleteLight("Light_3");
	Light.DeleteLight("Light_4");
	Light.DeleteLight("Light_5");
	Camera.Reset();
	return;
}

void CCube3D::Update(){
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

CCube3D::CCube3D(wchar_t* TextureFile,float length){
	Render.CreateGroup(L"Cube3D",TextureFile,D3DPT_TRIANGLELIST);
	Depth = 0;
	Length = length;
	return;
}