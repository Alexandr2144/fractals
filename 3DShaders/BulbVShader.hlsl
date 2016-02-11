cbuffer ConstBuffer
{
	matrix MatWorld;
	matrix MatView;
	matrix MatProj;
	float4 ViewPoint;
};

struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 ScrPos    : SV_POSITION;
	float3 Position  : TEXCOORD0;
	float3 ViewPoint : TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;	
	output.ScrPos = mul(input.Position+float4(0,0,3,0),MatProj);
	output.Position = mul(input.Position,MatView);
	output.ViewPoint = mul(ViewPoint,MatView);
	//output.ViewPoint = ViewPoint;
	return output;
}

