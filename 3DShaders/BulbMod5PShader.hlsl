Texture2D Texture;
SamplerState Sampler;

cbuffer ConstBuffer
{
	float4 ViewPoint;
	float ConstRe;
	float ConstIm;
	float Exp;
	int MaxCount;
};

struct VS_OUTPUT
{
	float4 ScrPos    : SV_POSITION;
	float3 Position  : TEXCOORD0;
	float3 ViewPoint : TEXCOORD1;
	float3 Up 	 : TEXCOORD2;
	float3 Right 	 : TEXCOORD3;
};

float3 trace(float3 StartPos,float3 Step)
{
	float3 Z,tZ;
	Z.x = dot(Step,Step);
	Z.y = dot(Step,StartPos);
	Z.z = dot(StartPos,StartPos)-4;
	tZ.z = Z.y*Z.y-Z.x*Z.z;
	if(tZ.z < 0) return float3(100,100,100);
	float3 Ray = StartPos+Step*((-Z.y-sqrt(tZ.z))/Z.x);
	Step = Step*((-Z.y+sqrt(tZ.z))/Z.x)-Step*((-Z.y-sqrt(tZ.z))/Z.x);
	Step /= 1000;
	Z = 0;

	for(int Count = 0; Count < 1000; Count++)
	{
		for(int i = 0; i < 6; i++){
			if(dot(Z,Z) > 2) break;
			tZ.x = pow(Z.x,5)-10*pow(Z.x,3)*(pow(Z.y,2)+pow(Z.z,2))+
					5*Z.x*(pow(Z.y,4)+pow(Z.z,4));
			tZ.y = pow(Z.y,5)-10*pow(Z.y,3)*(pow(Z.x,2)+pow(Z.z,2))+
					5*Z.y*(pow(Z.x,4)+pow(Z.z,4));
			tZ.z = pow(Z.z,5)-10*pow(Z.z,3)*(pow(Z.x,2)+pow(Z.y,2))+
					5*Z.z*(pow(Z.x,4)+pow(Z.y,4));
			Z = tZ;
			tZ.x = pow(Z.x,5)-10*pow(Z.x,3)*(pow(Z.y,2)+pow(Z.z,2))+
					5*Z.x*(pow(Z.y,4)+pow(Z.z,4));
			tZ.y = pow(Z.y,5)-10*pow(Z.y,3)*(pow(Z.x,2)+pow(Z.z,2))+
					5*Z.y*(pow(Z.x,4)+pow(Z.z,4));
			tZ.z = pow(Z.z,5)-10*pow(Z.z,3)*(pow(Z.x,2)+pow(Z.y,2))+
					5*Z.z*(pow(Z.x,4)+pow(Z.y,4));
			Z = -tZ+Ray;
		}
		if(dot(Z,Z) < 2) return Ray;
		Z = 0;
		Ray += Step;
	}
	return float3(100,100,100);
}

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float3 step = normalize(input.Position-input.ViewPoint)*0.003;
	float4 finalColor;

	float3 p = trace(input.Position,step);
	float3 p1 = trace(input.Position+input.Right*0.05,step);
	float3 p2 = trace(input.Position+input.Up*0.05,step);
	float3 normal = normalize(cross(p2-p,p1-p));

	float3 lightVec = normalize(float3(0,0,-5)-p);
	float3 eyeVec = normalize(input.ViewPoint-p);
	float3 halfv = normalize(lightVec+eyeVec);

	float4 Diffuse = 0.2*max(0,dot(normal,lightVec));
	float4 Specular = 0.05*pow(max(0,dot(normal,halfv)),12);
	finalColor = Diffuse+Specular;

	float a = length(p);
	if(a < 0.6)
		return float4(finalColor.r/2,finalColor.g/2+(a-0.4),finalColor.b/2,1);
	if(a < 0.8)
		return float4(finalColor.r/2,finalColor.g/2+(0.8-a),finalColor.b/2+(a-0.6),1);
	if(a < 1.0)
		return float4(finalColor.r/2+exp((a-0.8)*10-2)/5,finalColor.g/2,finalColor.b/2+exp((1-a)*10-2)/5,1);
	if(a < 1.2)
		return float4(finalColor.r/2+(1.2-a),finalColor.g/2+(a-1),finalColor.b/2,1);
	return 0;
}