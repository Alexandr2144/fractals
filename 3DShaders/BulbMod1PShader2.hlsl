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
};

float3 trace(float3 StartPos,float3 Step)
{
	float3 Ray = StartPos;
	float3 Z = 0;
	float3 tZ;
	float3 pw3;
	float3 pw4;
	float3 pw5;
	for(int Count = 0; Count < 1000; Count++)
	{
		for(int i = 0; i < 12; i++){
			if(dot(Z,Z) > 2) break;
			pw3 = Z*Z*Z;
			pw4 = pw3*Z;
			pw5 = pw4*Z;
			tZ.x = pw5.x-10*pw3.x*dot(Z.yz,Z.yz)+5*Z.x*(pw4.y+pw4.z);
			tZ.y = pw5.y-10*pw3.y*dot(Z.xz,Z.xz)+5*Z.y*(pw4.x+pw4.z);
			tZ.z = pw5.z-10*pw3.z*dot(Z.xy,Z.xy)+5*Z.z*(pw4.x+pw4.y);
			Z = tZ+Ray;
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
	float3 normal;
	float3 light1 = float3(3,0,-5);
	float3 light2 = input.Position;
	float4 finalColor = float4(0.1,0.1,0.1,1);
	float3 lightVec;
	float3 viewVec;
	float dist;

	float3 p = trace(input.Position,step);
	float3 p1 = trace(input.Position-float3(0.006,0,0),step);
	float3 p2 = trace(input.Position-float3(0,0.006,0),step);
	normal = cross(p2-p,p1-p);
	if(dot(normal,input.ViewPoint-p) < 0) normal = -normal;
	normal = normalize(normal);
	lightVec = light1-p;
	dist = length(lightVec);
	lightVec /= dist;
	finalColor += saturate(abs(dot(lightVec,normal))/(0.1*dist+0.1*dist*dist));
	
	//viewVec = normalize(p-input.ViewPoint);
	//lightVec = reflect(lightVec,normal);
	//finalColor += saturate(pow(dot(viewVec,lightVec),12))/10;

	//lightVec = light2-p;
	//dist = length(lightVec);
	//lightVec /= dist;
	//finalColor += saturate(dot(lightVec,normal)/(0.3*dist+0.2*dist*dist));
	float a = length(p);
	if(a < 0.6)
		return float4(0,finalColor.g*(a-0.4)*5,0,1);
	if(a < 0.8)
		return float4(0,finalColor.g*(0.8-a)*5,finalColor.b*(a-0.6)*5,1);
	//if(a < 1.0)
	//	return float4(finalColor.r(a-0.8)*5,0,finalColor.b*(1-a)*5,1);
	if(a < 1.0)
		return float4(finalColor.r*exp((a-0.8)*10-2),0,finalColor.b*exp((1-a)*10-2),1);
	if(a < 1.2)
		return float4(finalColor.r*(1.2-a)*5,finalColor.g*(a-1)*5,0,1);
	return float4(0.4,0.4,0.4,1);
}