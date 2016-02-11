cbuffer CBJPixel
{
	float4 xz;
	float CoordX;
	float CoordY;
	float ConstRe;
	float ConstIm;
	float Scale;
	float Exp;
	int MaxCount;
	int Theme;
}

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
	float2 Tex : TEXCOORD1;
	float3 WorldPos : TEXCOORD2;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	int Count = 0;
	float Length = 3.0/Scale;
	float2 TexCoord;
	TexCoord.x = Length*(2*input.Tex.x-1)+CoordX;
	TexCoord.y = Length*(2*input.Tex.y-1)+CoordY;
	float4 Z;
	Z.x = TexCoord.x;
	Z.y = TexCoord.y;
	Z.w = 0;
	while(Z.w < 4)
	{	
		if(Count == MaxCount)
			return float4(0,0,0,0);
		Z.z = atan(Z.y/Z.x);
		Z.w = Z.x*Z.x+Z.y*Z.y;
		Z.x = pow(Z.w,Exp*0.5)*cos(Exp*Z.z)+ConstRe;
		Z.y = pow(Z.w,Exp*0.5)*sin(Exp*Z.z)+ConstIm;
		Count++;
	}
	/*if(Theme == 1)
	{
		if(Count < 20)
			return float4(1,float(Count)/20,0,1);
		if(Count < 50)
			return float4(float(50-Count)/30,1,0,1);
		if(Count < 90)
			return float4(0,1,float(Count-50)/40,1);
		if(Count < 140)
			return float4(0,float(140-Count)/50,1,1);
		if(Count < 200)
			return float4(float(Count-140)/60,0,1,1);
		if(Count < 270)
			return float4(float(270-Count)/70,0,float(270-Count)/70,1);
	}
	if(Theme == 2)
	{
		if(Count < 20)
			return float4(float(20-Count)/20,0,1,1);
		if(Count < 50)
			return float4(0,float(Count-20)/30,1,1);
		if(Count < 90)
			return float4(0,1,float(90-Count)/40,1);
		if(Count < 140)
			return float4(float(Count-90)/50,1,0,1);
		if(Count < 200)
			return float4(1,float(200-Count)/60,0,1);
		if(Count < 270)
			return float4(float(270-Count)/70,0,0,1);
	}
	if(Theme == 4)
	{
		if(Count < 50)
			return float4(float(Count)/50,0,0,1);
		if(Count < 100)
			return float4(1,float(Count-50)/50,0,1);
		if(Count < 250)
			return float4(float(250-Count)/150,float(250-Count)/150,0,1);
	}
	if(Theme == 5)
	{*/
		if(Count < 50)
			return float4(0,float(Count)/50,0,1);
		if(Count < 100)
			return float4(float(Count-50)/50,1,0,1);
		if(Count < 250)
			return float4(float(250-Count)/150,float(250-Count)/150,0,1);
	/*}
	if(Theme == 6)
	{
		if(Count < 50)
			return float4(0,0,float(Count)/50,1);
		if(Count < 100)
			return float4(0,float(Count-50)/50,1,1);
		if(Count < 150)
			return float4(0,1,float(150-Count)/50,1);
		if(Count < 250)
			return float4(0,float(250-Count)/100,0,1);
	}
	if(Theme == 7)
	{
		if(Count < 100)
			return float4(float(Count)/100,float(Count)/100,float(Count)/100,1);
		return float4(1,1,1,1);
	}*/
	return float4(1,1,1,1);
}