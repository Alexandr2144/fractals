struct CBVBulb3D
{
	XMMATRIX MatWorld;
	XMMATRIX MatView;
	XMMATRIX MatProj;
	XMVECTOR ViewPoint;
};

class CBulb3D
{
protected:
	CBVBulb3D VSConst;
	std::vector <size_t> Bulb;
	int Theme;
public:
	CBulb3D();
	void Load();
	void Unload();
	void Update();
	void Build();
};

void CBulb3D::Update(){
	if(Input.Keyboard(DIK_NUMPAD1)) Theme = 1;
	if(Input.Keyboard(DIK_NUMPAD2)) Theme = 2;
	if(Input.Keyboard(DIK_NUMPAD3)) Theme = 3;
	if(Input.Keyboard(DIK_NUMPAD4)) Theme = 4;
	//Camera.RotX(float(Input.Mouse(DIM_GETX))/300);
	//Camera.RotY(float(Input.Mouse(DIM_GETY))/300);
	//VSConst.MatView = XMMatrixTranspose(Camera.GetMatView());
	//VSConst.MatProj = XMMatrixTranspose(Camera.GetMatProj());
	//VSConst.ViewPoint = XMVectorSet(0,0,-3,0);
	//Shader.UpdateConst(L"3DShaders//BulbVShader.hlsl");
	//if(Theme == 1) Shader.UseShader(L"3DShaders//BulbMod1PShader.hlsl",PIXEL);
	//if(Theme == 2) Shader.UseShader(L"3DShaders//BulbMod2PShader.hlsl",PIXEL);
	//if(Theme == 3) Shader.UseShader(L"3DShaders//BulbMod3PShader.hlsl",PIXEL);
	//if(Theme == 4) Shader.UseShader(L"3DShaders//BulbMod4PShader.hlsl",PIXEL);
	return;
}

CBulb3D::CBulb3D(){
	//Shader.AddShader(L"3DShaders//BulbVShader.hlsl",VERTEX,sizeof(VShaderConst),&VShaderConst);
	//Shader.AddShader(L"3DShaders//BulbMod1PShader.hlsl",PIXEL,0,NULL);
	Render.CreateGroup(L"Bulb3D",L"Texture.jpg",D3DPT_TRIANGLELIST);
	Theme = 1;
	return;
}

void CBulb3D::Load(){
	//Shader.UseShader(L"3DShaders//BulbVShader.hlsl",VERTEX);
	Light.SetAmbientLight(1);
	Render.UseGroup(L"Bulb3D");
	Render.AddVertex(D3DVertex(XMFLOAT3(-1,-1,-2),XMFLOAT3(0,0,-1),XMFLOAT2(-1,-1)));
	Render.AddVertex(D3DVertex(XMFLOAT3(-1, 1,-2),XMFLOAT3(0,0,-1),XMFLOAT2(-1, 1)));
	Render.AddVertex(D3DVertex(XMFLOAT3( 1, 1,-2),XMFLOAT3(0,0,-1),XMFLOAT2( 1, 1)));
	Render.AddVertex(D3DVertex(XMFLOAT3( 1,-1,-2),XMFLOAT3(0,0,-1),XMFLOAT2( 1,-1)));
	Render.AddIndex(0,1,2);
	Render.AddIndex(0,2,3);
	Camera.SetPos(0,0,-4);
	Build();
	return;
}

void CBulb3D::Unload(){
	Camera.Reset();
	Render.UseGroup(L"Bulb3D");
	Render.ClearVertex();
	//Shader.UseDefaultShader();
	return;
}

void CBulb3D::Build(){
	for(int i = 0; i < 64; i++) for(int j = 0; j < 64; j++){
		XMVECTOR Ray = Camera.GetPos();
		XMVECTOR Step = (XMVectorSet(float(i)/32-1,float(j)/32-1,-2,0)-Ray)*0.003f;
		XMFLOAT3 Z = XMFLOAT3(0,0,0);
		XMFLOAT3 tZ;
		for(int Count = 0; Count < 1000; Count++)
		{
			for(int k = 0; k < 10; k++){
				if(Z.x*Z.x+Z.y*Z.y > 4) break;
				tZ.x = pow(Z.x,5)-10*pow(Z.x,3)*(pow(Z.y,2)+pow(Z.z,2))+
						5*Z.x*(pow(Z.y,4)+pow(Z.z,4))+XMVectorGetX(Ray);
				tZ.y = pow(Z.y,5)-10*pow(Z.y,3)*(pow(Z.x,2)+pow(Z.z,2))+
						5*Z.y*(pow(Z.x,4)+pow(Z.z,4))+XMVectorGetY(Ray);
				tZ.z = pow(Z.z,5)-10*pow(Z.z,3)*(pow(Z.x,2)+pow(Z.y,2))+
						5*Z.z*(pow(Z.x,4)+pow(Z.y,4))+XMVectorGetZ(Ray);
				Z = tZ;
			}
			if(Z.x*Z.x+Z.y*Z.y < 4) {
				Bulb.push_back(16777215);
				break;
			}
			Z = XMFLOAT3(0,0,0);
			Ray += Step;
		}
		Bulb.push_back(0);
	}
	Material.AddMaterialFromMemory(L"Texture.jpg",&Bulb[0],64,64);
	//Material.UseMaterial(L"Bulb3D");
	//Material.AddMaterial(L"Texture.jpg");
	//Material.UseMaterial(L"Texture.jpg");
	return;
}