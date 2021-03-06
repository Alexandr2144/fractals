#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <stack>
#include <map>
#define LSYS LSConst[State]

struct CTurtleState
{
	double PositionX;
	double PositionY;
	double LookX;
	double LookY;
};

struct CLSysConst
{
public:
	std::string Axiom;
	std::map <char,std::string> Rules;
	XMFLOAT2 StartPos;
	double Length;
	double Angle;
	double Scale;
	short Depth;
};

class CLSystem
{
protected:
	CLSysConst LSConst[10];
	short State;

	std::stack <CTurtleState> SaveState;
	CTurtleState TurtleState;
	void _Update(short Step);
public:
	CLSystem();
	void Draw();
	void Load();
	void Unload();
	void Update();
	void LoadFile(std::string FileName);
};

CLSystem::CLSystem(){
	State = 1;
	Render.CreateGroup(L"LSystem",D3DPT_LINELIST);
	return;
}

void CLSystem::Draw(){
	bool none = true;
	TurtleState.LookX = LSYS.Length; 
	TurtleState.LookY = 0;
	TurtleState.PositionX = LSYS.StartPos.x-LSYS.Length/2; 
	TurtleState.PositionY = LSYS.StartPos.y;

	Render.UseGroup(L"LSystem");
	Render.ClearVertex();
	std::string tCommand = LSYS.Axiom;
	std::string Command;
	for(int i=0; i < LSYS.Depth; i++){
		for(UINT j=0; j < tCommand.size(); j++){
			for(std::map <char,std::string>::iterator cur = LSYS.Rules.begin();
				cur != LSYS.Rules.end(); cur++){
				none = true;
				if(tCommand[j] == (*cur).first){
					Command.append((*cur).second);
					none = false;
					break;
				}
			}
			if(none) Command.push_back(tCommand[j]);
		}
		TurtleState.LookX /= LSYS.Scale;
		TurtleState.LookY /= LSYS.Scale;
		tCommand = Command;
		Command.clear();
	}
	for(UINT i=0; i < tCommand.size(); i++){
		if(tCommand[i] == 'F'){
			Render.AddVertex(D3DVertex(XMFLOAT3((float)TurtleState.PositionX,(float)TurtleState.PositionY,0),XMFLOAT3(0,0,0),0,0));
			TurtleState.PositionX += TurtleState.LookX;
			TurtleState.PositionY += TurtleState.LookY;
			Render.AddVertex(D3DVertex(XMFLOAT3((float)TurtleState.PositionX,(float)TurtleState.PositionY,0),XMFLOAT3(0,0,0),0,0));
		}if(tCommand[i] == 'b' || tCommand[i] == 'G'){
			TurtleState.PositionX += TurtleState.LookX;
			TurtleState.PositionY += TurtleState.LookY;
		}else if(tCommand[i] == '+'){
			double t1 = TurtleState.LookX*cos(LSYS.Angle)-TurtleState.LookY*sin(LSYS.Angle);
			double t2 = TurtleState.LookX*sin(LSYS.Angle)+TurtleState.LookY*cos(LSYS.Angle);
			TurtleState.LookX = t1;
			TurtleState.LookY = t2;
		}else if(tCommand[i] == '-'){
			double t1 = TurtleState.LookX*cos(LSYS.Angle)+TurtleState.LookY*sin(LSYS.Angle);
			double t2 = TurtleState.LookY*cos(LSYS.Angle)-TurtleState.LookX*sin(LSYS.Angle);
			TurtleState.LookX = t1;
			TurtleState.LookY = t2;
		}else if(tCommand[i] == '['){
			SaveState.push(TurtleState);
		}else if(tCommand[i] == ']'){
			TurtleState = SaveState.top();
			SaveState.pop();
		}
	}
}
void CLSystem::Load(){
	Light.SetAmbientLight(1);
	Render.SetBackgroundColor(XMFLOAT4(1,1,1,1));
	Draw();
	return;
}

void CLSystem::Unload(){
	Render.UseGroup(L"LSystem");
	Render.ClearVertex();
	return;
}

void CLSystem::Update(){
	static DWORD DefaultTime = timeGetTime();
	if(Input.Keyboard(DIK_W)) Camera.MoveUp();
	if(Input.Keyboard(DIK_S)) Camera.MoveDown();
	if(Input.Keyboard(DIK_A)) Camera.MoveLeft();
	if(Input.Keyboard(DIK_D)) Camera.MoveRight();
	if(Input.Keyboard(DIK_0)) State = 0,Draw();
	if(Input.Keyboard(DIK_1)) State = 1,Draw();
	if(Input.Keyboard(DIK_2)) State = 2,Draw();
	if(Input.Keyboard(DIK_3)) State = 3,Draw();
	if(Input.Keyboard(DIK_4)) State = 4,Draw();
	if(Input.Keyboard(DIK_5)) State = 5,Draw();
	if(Input.Keyboard(DIK_6)) State = 6,Draw();
	if(Input.Keyboard(DIK_7)) State = 7,Draw();
	if(Input.Keyboard(DIK_8)) State = 8,Draw();
	if(Input.Keyboard(DIK_9)) State = 9,Draw();
	Camera.Scaling(float(Input.Mouse(DIM_GETZ))/300);
	if(timeGetTime()-DefaultTime > 100){
		if(Input.Keyboard(DIK_ADD)) LSYS.Depth++,Draw();
		if(Input.Keyboard(DIK_SUBTRACT)) 
			if(LSYS.Depth) LSYS.Depth--,Draw();
		DefaultTime = timeGetTime();
	}
}

void CLSystem::LoadFile(std::string FileName){
	std::ifstream FileIn(&FileName[0]);
	std::string szInput;
	int i = 0;
	float dInput;
	while(1)
	{
		szInput = "0";
		szInput[0] = 25;
		FileIn >> szInput;
		if(szInput[0] == '/' || szInput[1] == '/'){
		}else if(szInput[0] == 'S' && szInput[2] == ':' && szInput[1] >= '0' && szInput[1] <= '9'){
			i = szInput[1]-'0';
			LSConst[i].Depth = 0;
			LSConst[i].Length = 1;
			LSConst[i].Scale = 3;
			LSConst[i].Angle = 3.14159265358979323846;
			LSConst[i].StartPos = XMFLOAT2(0,0);
		}else if(szInput == "Position:"){
			FileIn >> dInput;
			LSConst[i].StartPos.x = dInput;
			FileIn >> dInput;
			LSConst[i].StartPos.y = dInput;
		}else if(szInput == "Depth:"){
			FileIn >> dInput;
			LSConst[i].Depth = short(dInput);
		}else if(szInput == "Length:"){
			FileIn >> dInput;
			LSConst[i].Length = dInput;
		}else if(szInput == "Scale:"){
			FileIn >> dInput;
			LSConst[i].Scale = dInput;
		}else if(szInput == "Axiom:"){
			FileIn >> szInput;
			LSConst[i].Axiom = szInput;
		}else if(szInput == "Angle:"){
			FileIn >> dInput;
			LSConst[i].Angle *= dInput/180;
		}else if(szInput[0] == 25){
			break;
		}else{
			std::string _rules;
			FileIn >> _rules;
			LSConst[i].Rules[szInput[0]] = _rules;
		}
	}
}