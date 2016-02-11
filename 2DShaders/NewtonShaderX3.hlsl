cbuffer CBPixel
{
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
	float2 Z;
	float3 Vec;
	Z.x = input.Tex.y/Scale-CoordY;
	Z.y = input.Tex.x/Scale-CoordX;
	while(Count != MaxCount)
	{	
		if(Z.x > 0.999 && Z.x < 1.001 && Z.y > -0.001 && Z.y < 0.001)
			return float4(float(40-Count)/40,0,float(40-Count)/40,0);
		if(Z.x < -0.499 && Z.x > -0.501 && Z.y > 0.865 && Z.y < 0.867)
			return float4(float(40-Count)/40,float(40-Count)/40,0,0);
		if(Z.x < -0.499 && Z.x > -0.501 && Z.y > -0.867 && Z.y < -0.865)
			return float4(0,float(40-Count)/40,float(40-Count)/40,0);

		Vec.x = 1/dot(Z,Z);
		sincos(-2*atan(Z.y/Z.x),Vec.y,Vec.z);
		Z.x = 2*Z.x/3+Vec.x*Vec.z/3+ConstRe;
		Z.y = 2*Z.y/3+Vec.x*Vec.y/3+ConstIm;
		Count++;
	}
	return float4(1,1,1,1);
}
