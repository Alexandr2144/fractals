#include <string>
#include <map>

struct D3DLight
{
	short Type;
	XMFLOAT4 Position;
	XMFLOAT4 Direction;
	XMFLOAT4 Propecties;
	XMFLOAT4 Color;
};

class CEngineLight
{
protected:
	std::map <std::string,D3DLight> Light;
	short DirCount;
	short PointCount;
	short SpotCount;
public:
	CEngineLight::CEngineLight(){CBDefPConst.GlobalProp.w=0.01f;}
	void SetAmbientLight(float Value){CBDefPConst.GlobalProp.w=Value;}
	void AddDirectionLight(std::string Name,XMFLOAT4 Position,
							XMFLOAT4 Direction,float Brightness,float Distance,XMFLOAT4 Color);
	void AddPointLight(std::string Name,XMFLOAT4 Position,
							float Brightness,float Attenuation, float Distance,XMFLOAT4 Color);
	void AddSpotLight(std::string Name,XMFLOAT4 Position,XMFLOAT4 Direction,
							float Brightness,float Attenuation,float Angle, float Distance,XMFLOAT4 Color);
	void DeleteLight(std::string Name);
	void _Update();
};

void CEngineLight::AddDirectionLight(std::string Name,XMFLOAT4 Position,XMFLOAT4 Direction,
										float Brightness,float Distance,XMFLOAT4 Color){
//	if(DirCount == 5) return;
	Light[Name].Type = 0;
	Light[Name].Position = Position;
	Light[Name].Direction = XMFLOAT4(-Direction.x,-Direction.y,-Direction.z,0);
	Light[Name].Propecties.x = Brightness;
	Light[Name].Propecties.y = Distance;
	Light[Name].Color = Color;
	DirCount++;
}

void CEngineLight::AddPointLight(std::string Name,XMFLOAT4 Position,float Brightness,
								 float Attenuation,float Distance,XMFLOAT4 Color){
//	if(PointCount == 5) return;
	Light[Name].Type = 1;
	Light[Name].Position = Position;
	Light[Name].Propecties.x = Brightness;
	Light[Name].Propecties.y = Attenuation;
	Light[Name].Propecties.z = Distance;
	Light[Name].Color = Color;
	PointCount++;
}

void CEngineLight::AddSpotLight(std::string Name,XMFLOAT4 Position,XMFLOAT4 Direction,float Brightness,
								float Attenuation,float Angle, float Distance,XMFLOAT4 Color){
//	if(SpotCount == 5) return;
	Light[Name].Type = 2;
	Light[Name].Position = Position;
	Light[Name].Direction = XMFLOAT4(-Direction.x,-Direction.y,-Direction.z,0);;
	Light[Name].Propecties.x = Brightness;
	Light[Name].Propecties.y = Attenuation;
	Light[Name].Propecties.z = Angle;
	Light[Name].Propecties.w = Distance;
	Light[Name].Color = Color;
	SpotCount++;
}

void CEngineLight::DeleteLight(std::string Name){
	if(Light.count(Name)) Light.erase(Light.find(Name));
	return;
}

void CEngineLight::_Update(){
	short DirCount = 0;
	short PointCount = 0;
	short SpotCount = 0;
	for(std::map <std::string,D3DLight>::iterator i = Light.begin();i != Light.end();i++){
		if((*i).second.Type == 0){
			CBDefPConst.LightPosition[DirCount] = (*i).second.Position;
			CBDefPConst.LightDirection[DirCount] = (*i).second.Direction;
			CBDefPConst.LightProp[DirCount] = (*i).second.Propecties;
			CBDefPConst.LightColor[DirCount] = (*i).second.Color;
			DirCount++;
		}else if((*i).second.Type == 1){
			CBDefPConst.LightPosition[PointCount+5] = (*i).second.Position;
			CBDefPConst.LightProp[PointCount+5] = (*i).second.Propecties;
			CBDefPConst.LightColor[PointCount+5] = (*i).second.Color;
			PointCount++;
		}else if((*i).second.Type == 2){
			CBDefPConst.LightPosition[SpotCount+10] = (*i).second.Position;
			CBDefPConst.LightDirection[SpotCount+5] = (*i).second.Direction;
			CBDefPConst.LightProp[SpotCount+10] = (*i).second.Propecties;
			CBDefPConst.LightColor[SpotCount+10] = (*i).second.Color;
			SpotCount++;
		}
	}
	CBDefPConst.GlobalProp.x = DirCount;
	CBDefPConst.GlobalProp.y = PointCount;
	CBDefPConst.GlobalProp.z = SpotCount;
	Shader.UpdateConst(DefPSName);
}

CEngineLight Light;