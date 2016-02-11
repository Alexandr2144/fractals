Texture2D Texture;
SamplerState Sampler;

cbuffer ConstBuffer
{
	float4 CamPos;
	float4 GlobalProp;
	float4 LightPosition[15];
	float4 LightDirection[10];
	float4 LightProp[15];
	float4 LightColor[15];
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
	float2 Tex : TEXCOORD1;
	float3 WorldPos : TEXCOORD2;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 finalColor = GlobalProp.w;
	float3 lightVec;
	float3 viewVec;
	float dist;
	for(int i = 0; i < GlobalProp.x; i++){
		dist = distance(LightPosition[i],input.WorldPos);
		finalColor += saturate((LightProp[i].y-dist)/LightProp[i].y)*saturate(dot((float3)LightDirection[i],input.Norm)*LightColor[i]);
		
		//viewVec = normalize(input.WorldPos-(float3)CamPos);
		//lightVec = reflect(LightDirection[i],input.Norm);
		//finalColor += saturate(pow(dot(viewVec,lightVec),12)*LightColor[i]);
	}
	for(int i = 5; i < GlobalProp.y+5; i++){
		lightVec = (float3)LightPosition[i]-input.WorldPos;
		dist = length(lightVec);
		lightVec /= dist;
		finalColor += saturate(dot(lightVec,input.Norm)/(LightProp[i].x*dist+LightProp[i].y*dist*dist)*LightColor[i]);
		
		viewVec = normalize(input.WorldPos-(float3)CamPos);
		lightVec = reflect(lightVec,input.Norm);
		finalColor += saturate(pow(dot(viewVec,lightVec),12)*LightColor[i]/2);
		//lightVec = normalize(viewVec+lightVec);
		//finalColor += saturate(pow(dot(input.Norm,lightVec),3)*LightColor[i]);
	}
	for(int i = 10; i < GlobalProp.z+10; i++){
		lightVec = (float3)LightPosition[i]-input.WorldPos;
		dist = length(lightVec);
		lightVec /= dist;
		finalColor += saturate(dot(1.57f*lightVec/LightProp[i].z,input.Norm)/(LightProp[i].x*dist+LightProp[i].y*dist*dist)*LightColor[i]);
	}
	finalColor *= Texture.Sample(Sampler,input.Tex);
	return finalColor;
}