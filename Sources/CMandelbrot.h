struct CBMJ
{
	float CoordX;
	float CoordY;
	float ConstRe;
	float ConstIm;
	float Scale;
	float Exp;
	int MaxCount;
	int Theme;
};

class CMandelbrot
{
protected:
	CBMJ MandelbrotConst;
	CBMJ JuliaConst;
	CBMJ NewtonX3Const;
	CBMJ NewtonX5Const;
	CBMJ* CurrentConst;
	short State;
	bool ConstIsChange;
public:
	CMandelbrot();
	void Reset();
	void Update();
	void Load();
	void Unload();

	void AddConstRe(){CurrentConst->ConstRe += 0.005f;ConstIsChange=true;}
	void AddConstIm(){CurrentConst->ConstIm += 0.005f;ConstIsChange=true;}
	void SubtractConstRe(){CurrentConst->ConstRe -= 0.005f;ConstIsChange=true;}
	void SubtractConstIm(){CurrentConst->ConstIm -= 0.005f;ConstIsChange=true;}

	void AddDepth(){CurrentConst->MaxCount++;ConstIsChange=true;}
	void SubtractDepth(){if(CurrentConst->MaxCount) CurrentConst->MaxCount--;ConstIsChange=true;}

	void AddExp(){CurrentConst->Exp += 0.005f;ConstIsChange=true;}
	void SubtractExp(){CurrentConst->Exp -= 0.005f;ConstIsChange=true;}

	void Scaling(float Value){CurrentConst->Scale += Value*CurrentConst->Scale;ConstIsChange=true;}

	void MoveUp(){CurrentConst->CoordY -= 0.005f/CurrentConst->Scale;ConstIsChange=true;}
	void MoveDown(){CurrentConst->CoordY += 0.005f/CurrentConst->Scale;ConstIsChange=true;}
	void MoveRight(){CurrentConst->CoordX -= 0.005f/CurrentConst->Scale;ConstIsChange=true;}
	void MoveLeft(){CurrentConst->CoordX += 0.005f/CurrentConst->Scale;ConstIsChange=true;}

	void SetTheme(short Value){CurrentConst->Theme = Value;ConstIsChange=true;}
	void MandelbrotSet(){State=0;CurrentConst=&MandelbrotConst;ConstIsChange=true;}
	void JuliaSet(){State=1;CurrentConst=&JuliaConst;ConstIsChange=true;}
	void NewtonX3Set(){State=2;CurrentConst=&NewtonX3Const;ConstIsChange=true;}
	void NewtonX5Set(){State=3;CurrentConst=&NewtonX5Const;ConstIsChange=true;}
};

void CMandelbrot::Update(){
	if(Input.Keyboard(DIK_W)) MoveUp();
	if(Input.Keyboard(DIK_S)) MoveDown();
	if(Input.Keyboard(DIK_D)) MoveRight();
	if(Input.Keyboard(DIK_A)) MoveLeft();
	Scaling(float(Input.Mouse(DIM_GETZ))/300);
	if(Input.Keyboard(DIK_ADD)) AddDepth();
	if(Input.Keyboard(DIK_SUBTRACT)) SubtractDepth();
	if(Input.Keyboard(DIK_LSHIFT)) AddExp();
	if(Input.Keyboard(DIK_LCONTROL)) SubtractExp();
	if(Input.Keyboard(DIK_UP)) AddConstRe();
	if(Input.Keyboard(DIK_DOWN)) SubtractConstRe();
	if(Input.Keyboard(DIK_RIGHT)) AddConstIm();
	if(Input.Keyboard(DIK_LEFT)) SubtractConstIm();

	if(Input.Keyboard(DIK_1)) MandelbrotSet();
	if(Input.Keyboard(DIK_2)) JuliaSet();
	if(Input.Keyboard(DIK_3)) NewtonX3Set();
	if(Input.Keyboard(DIK_4)) NewtonX5Set();

	if(Input.Keyboard(DIK_NUMPAD0)) SetTheme(0);
	if(Input.Keyboard(DIK_NUMPAD1)) SetTheme(1);
	if(Input.Keyboard(DIK_NUMPAD2)) SetTheme(2);
	if(Input.Keyboard(DIK_NUMPAD3)) SetTheme(3);
	if(Input.Keyboard(DIK_NUMPAD4)) SetTheme(4);
	if(Input.Keyboard(DIK_NUMPAD5)) SetTheme(5);
	if(Input.Keyboard(DIK_NUMPAD6)) SetTheme(6);

	if(ConstIsChange){
	ConstIsChange=false;
	switch(State){
		case 0:
			Shader.UseShader(L"2DShaders//MandelbrotShader.hlsl",PIXEL);
			Shader.UpdateConst(L"2DShaders//MandelbrotShader.hlsl");
			break;
		case 1:
			Shader.UseShader(L"2DShaders//JuliaShader.hlsl",PIXEL);
			Shader.UpdateConst(L"2DShaders//JuliaShader.hlsl");
			break;
		case 2:
			Shader.UseShader(L"2DShaders//NewtonShaderX3.hlsl",PIXEL);
			Shader.UpdateConst(L"2DShaders//NewtonShaderX3.hlsl");
			break;
		case 3:
			Shader.UseShader(L"2DShaders//NewtonShaderX5.hlsl",PIXEL);
			Shader.UpdateConst(L"2DShaders//NewtonShaderX5.hlsl");
			break;
	}}
}

CMandelbrot::CMandelbrot(){
	NewtonX5Set();		Reset();
	NewtonX3Set();		Reset();
	JuliaSet();			Reset();
	MandelbrotSet();	Reset();
	Shader.AddShader(L"2DShaders//MandelbrotShader.hlsl",PIXEL,sizeof(MandelbrotConst),&MandelbrotConst);
	Shader.AddShader(L"2DShaders//JuliaShader.hlsl",PIXEL,sizeof(JuliaConst),&JuliaConst);
	Shader.AddShader(L"2DShaders//NewtonShaderX3.hlsl",PIXEL,sizeof(NewtonX3Const),&NewtonX3Const);
	Shader.AddShader(L"2DShaders//NewtonShaderX5.hlsl",PIXEL,sizeof(NewtonX5Const),&NewtonX5Const);
	Render.CreateGroup(L"Mandelbrot",D3DPT_TRIANGLELIST);
}

void CMandelbrot::Reset(){
	CurrentConst->CoordX = 0;
	CurrentConst->CoordY = 0;
	CurrentConst->ConstRe = 0;
	CurrentConst->ConstIm = 0;
	CurrentConst->Scale = 0.2f;
	CurrentConst->Exp = 2;
	CurrentConst->Theme = 1;
	CurrentConst->MaxCount = 100;
	ConstIsChange = true;
}

void CMandelbrot::Load(){
	Shader.UseShader(L"2DShaders//MandelbrotShader.hlsl",PIXEL);
	Render.UseGroup(L"Mandelbrot");
	Render.AddVertex(D3DVertex(XMFLOAT3(-3,-3,-0),XMFLOAT3(0,0,-1),XMFLOAT2(-2,-2)));
	Render.AddVertex(D3DVertex(XMFLOAT3(-3, 3,-0),XMFLOAT3(0,0,-1),XMFLOAT2(-2, 2)));
	Render.AddVertex(D3DVertex(XMFLOAT3( 3, 3,-0),XMFLOAT3(0,0,-1),XMFLOAT2( 2, 2)));
	Render.AddVertex(D3DVertex(XMFLOAT3( 3,-3,-0),XMFLOAT3(0,0,-1),XMFLOAT2( 2,-2)));
	Render.AddIndex(0,1,2);
	Render.AddIndex(0,2,3);
	Camera.SetPos(0,0,-1);
	Reset();
}

void CMandelbrot::Unload(){
	Camera.Reset();
	Render.UseGroup(L"Mandelbrot");
	Render.ClearVertex();
	Shader.UseDefaultShader();
}