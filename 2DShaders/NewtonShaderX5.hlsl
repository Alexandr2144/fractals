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
		if(Z.x > 0.308 && Z.x < 0.310 && Z.y > 0.950 && Z.y < 0.952)
			return float4(float(40-Count)/40,0,0,0);
		if(Z.x < -0.808 && Z.x > -0.810 && Z.y > 0.586 && Z.y < 0.588)
			return float4(float(40-Count)/40,float(40-Count)/40,0,0);
		if(Z.x < -0.808 && Z.x > -0.810 && Z.y < -0.586 && Z.y > -0.588)
			return float4(0,float(40-Count)/40,0,0);		
		if(Z.x > 0.308 && Z.x < 0.310 && Z.y < -0.950 && Z.y > -0.952)
			return float4(0,float(40-Count)/40,float(40-Count)/40,0);
		if(Z.x > 0.999 && Z.x < 1.001 && Z.y > -0.001 && Z.y < 0.001)
			return float4(0,0,float(40-Count)/40,0);
		Vec.x = pow(dot(Z,Z),-2);
		sincos(-4*atan(Z.y/Z.x),Vec.y,Vec.z);
		Z.x = 4*Z.x/5+Vec.x*Vec.z/5+ConstRe;
		Z.y = 4*Z.y/5+Vec.x*Vec.y/5+ConstIm;
		Count++;
	}
	return float4(0,0,0,0);
}
