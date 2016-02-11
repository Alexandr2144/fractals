#include "CEngineRender.h"
#include "CEngineInput.h"
#include "CEngineLight.h"
#include "CTriangle3D.h"
#include "CCube3D.h"
#include "CPlane3D.h"
#include "CMandelbrot3D.h"
#include "CMandelbrot.h"
#include "CLSystem.h"

#define UnloadAll()\
	Cube3D.Unload(),\
	Triangle3D.Unload(),\
	Plane3D.Unload(),\
	LSystem.Unload(),\
	Mandelbrot.Unload(),\
	Mandelbrot3D.Unload();

void LSystemInput(CLSystem* L_System);

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nShowCmd)
{
	INIT
	short State = 1;
	DWORD DefaultTime = timeGetTime();
	CLSystem LSystem;
	CTriangle3D Triangle3D(L"Texture.jpg",3.0f);
	CCube3D Cube3D(L"Texture.jpg",3.0f);
	CPlane3D Plane3D(L"Grass.jpg",8.0f);
	LSystem.LoadFile("gfractal.ini");
	CMandelbrot Mandelbrot;
	CMandelbrot3D Mandelbrot3D;
	Cube3D.Load();
	BEGIN
		if(timeGetTime()-DefaultTime > 100){
			if(Input.Keyboard(DIK_F1)&&State!=1&&State!=2){
				UnloadAll();
				Cube3D.Load();
				State = 1;
			}
			if(Input.Keyboard(DIK_1)&&State!=1&&State==2){
				UnloadAll();
				Cube3D.Load();
				State = 1;
			}
			if(Input.Keyboard(DIK_2)&&State==1&&State!=2){
				UnloadAll();
				Triangle3D.Load();
				State = 2;
			}
			if(Input.Keyboard(DIK_F2)&&State!=3){
				UnloadAll();
				Plane3D.Load();
				State = 3;
			}
			if(Input.Keyboard(DIK_F3)&&State!=4){
				UnloadAll();
				LSystem.Load();
				State = 4;
			}
			if(Input.Keyboard(DIK_F4)&&State!=5){
				UnloadAll();
				Mandelbrot.Load();
				State = 5;
			}
			if(Input.Keyboard(DIK_F5)&&State!=6){
				UnloadAll();
				Mandelbrot3D.Load();
				State = 6;
			}
			DefaultTime = timeGetTime();
		}
		if(State == 1)  Cube3D.Update();
		if(State == 2)  Triangle3D.Update();
		if(State == 3)  Plane3D.Update();
		if(State == 4)  LSystem.Update();
		if(State == 5)	Mandelbrot.Update();
		if(State == 6)	Mandelbrot3D.Update();
	END
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
	switch(msg)
	{
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE ){
			DestroyWindow(hwnd);}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}