struct CBVMandelbrot3D
{
	XMMATRIX MatWorld;
	XMMATRIX MatView;
	XMMATRIX MatProj;
	XMVECTOR ViewPoint;
};

class CMandelbrot3D
{
protected:
	CBVMandelbrot3D VShaderConst;
	int Theme;
public:
	CMandelbrot3D();
	void Load();
	void Unload();
	void Update();
};

void CMandelbrot3D::Update(){
	if(Input.Keyboard(DIK_1)) Shader.UseShader(L"3DShaders//BulbMod1PShader.hlsl",PIXEL);
	if(Input.Keyboard(DIK_2)) Shader.UseShader(L"3DShaders//BulbMod2PShader.hlsl",PIXEL);
	if(Input.Keyboard(DIK_3)) Shader.UseShader(L"3DShaders//BulbMod3PShader.hlsl",PIXEL);
	if(Input.Keyboard(DIK_4)) Shader.UseShader(L"3DShaders//BulbMod4PShader.hlsl",PIXEL);
	if(Input.Keyboard(DIK_5)) Shader.UseShader(L"3DShaders//BulbMod5PShader.hlsl",PIXEL);
	Camera.RotX(-float(Input.Mouse(DIM_GETX))/300);
	Camera.RotY(float(Input.Mouse(DIM_GETY))/300);
	VShaderConst.MatView = XMMatrixTranspose(Camera.GetMatView());
	VShaderConst.MatProj = XMMatrixTranspose(Camera.GetMatProj());
	VShaderConst.ViewPoint = XMVectorSet(0,0,-3,0);
	Shader.UpdateConst(L"3DShaders//BulbVShader.hlsl");
	return;
}

CMandelbrot3D::CMandelbrot3D(){
	Shader.AddShader(L"3DShaders//BulbVShader.hlsl",VERTEX,sizeof(VShaderConst),&VShaderConst);
	Shader.AddShader(L"3DShaders//BulbMod1PShader.hlsl",PIXEL,0,NULL);
	Shader.AddShader(L"3DShaders//BulbMod2PShader.hlsl",PIXEL,0,NULL);
	Shader.AddShader(L"3DShaders//BulbMod3PShader.hlsl",PIXEL,0,NULL);
	Shader.AddShader(L"3DShaders//BulbMod4PShader.hlsl",PIXEL,0,NULL);
	Shader.AddShader(L"3DShaders//BulbMod5PShader.hlsl",PIXEL,0,NULL);
	Render.CreateGroup(L"Mandelbrot3D",D3DPT_TRIANGLELIST);
	return;
}

void CMandelbrot3D::Load(){
	Shader.UseShader(L"3DShaders//BulbVShader.hlsl",VERTEX);
	Shader.UseShader(L"3DShaders//BulbMod1PShader.hlsl",PIXEL);
	Render.UseGroup(L"Mandelbrot3D");
	Render.AddVertex(D3DVertex(XMFLOAT3(-1,-1,-2),XMFLOAT3(0,0,-1),XMFLOAT2(-1,-1)));
	Render.AddVertex(D3DVertex(XMFLOAT3(-1, 1,-2),XMFLOAT3(0,0,-1),XMFLOAT2(-1, 1)));
	Render.AddVertex(D3DVertex(XMFLOAT3( 1, 1,-2),XMFLOAT3(0,0,-1),XMFLOAT2( 1, 1)));
	Render.AddVertex(D3DVertex(XMFLOAT3( 1,-1,-2),XMFLOAT3(0,0,-1),XMFLOAT2( 1,-1)));
	Render.AddIndex(0,1,2);
	Render.AddIndex(0,2,3);
	Camera.SetPos(0,0,0);
	return;
}

void CMandelbrot3D::Unload(){
	Camera.Reset();
	Render.UseGroup(L"Mandelbrot3D");
	Render.ClearVertex();
	Shader.UseDefaultShader();
	return;
}