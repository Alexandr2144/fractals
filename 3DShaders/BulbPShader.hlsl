Texture2D Texture;
SamplerState Sampler;

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
	float tmp;
	for(int Count = 0; Count < 1000; Count++)
	{
		for(int i = 0; i < 10; i++){
			if(dot(Z,Z) > 2) break;
			tZ.x = pow(Z.x,5)-10*pow(Z.x,3)*(pow(Z.y,2)+pow(Z.z,2))+
					5*Z.x*(pow(Z.y,4)+pow(Z.z,4))+Ray.x;
			tZ.y = pow(Z.y,5)-10*pow(Z.y,3)*(pow(Z.x,2)+pow(Z.z,2))+
					5*Z.y*(pow(Z.x,4)+pow(Z.z,4))+Ray.y;
			tZ.z = pow(Z.z,5)-10*pow(Z.z,3)*(pow(Z.x,2)+pow(Z.y,2))+
					5*Z.z*(pow(Z.x,4)+pow(Z.y,4))+Ray.z;
			Z = tZ;
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
	float3 light1 = float3(0,0,-5);
	float3 light2 = float3(0,0,5);
	float4 finalColor;
	float3 lightVec;
	float dist;

	float3 p = trace(input.Position,step);
	float3 p1 = trace(input.Position-float3(0.01,0,0),step);
	float3 p2 = trace(input.Position-float3(0,0.01,0),step);
	normal = cross(p2-p,p1-p);
	normal = normalize(normal);
	lightVec = light1-p;
	dist = length(lightVec);
	lightVec /= dist;
	finalColor = saturate(dot(lightVec,normal)/(0.3*dist+0.1*dist*dist));
	
	//lightVec = light2-p;
	//dist = length(lightVec);
	//lightVec /= dist;
	//finalColor += saturate(dot(lightVec,normal)/(0.3*dist+0.1*dist*dist));
	float a = length(p);
	if(a < 0.6)
		return float4(finalColor.r/2,finalColor.g/2+(a-0.4),finalColor.b/2,1);
	if(a < 0.8)
		return float4(finalColor.r/2,finalColor.g/2+(0.8-a),finalColor.b/2+(a-0.6),1);
	//if(a < 1.0)
	//	return float4(finalColor.r/2+(a-0.8),finalColor.g/2,finalColor.b/2+(1-a),1);
	if(a < 1.0)
		return float4(finalColor.r/2+exp((a-0.8)*10-2)/5,finalColor.g/2,finalColor.b/2+exp((1-a)*10-2)/5,1);
	if(a < 1.2)
		return float4(finalColor.r/2+(1.2-a),finalColor.g/2+(a-1),finalColor.b/2,1);
	return 0;
}