cbuffer ConstBuffer
{
	matrix mWorld;
	matrix mView;
	matrix mProj;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
	float2 Tex : TEXCOORD1;
	float3 WorldPos : TEXCOORD2;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.WorldPos = input.Pos;
	output.Pos = mul(input.Pos,mWorld);
	output.Pos = mul(output.Pos,mView);
	output.Pos = mul(output.Pos,mProj);
	output.Norm = input.Norm;
	output.Tex = input.Tex;
	return output;
}

