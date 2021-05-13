// YiaEngine.cpp: 定义应用程序的入口点。
//



#include "YiaEngine.h"
#include "Interface.h"
#include "Application.h"
#include "Dx12Sample.h"
#include "WindowBase.h"

using namespace std;

//extern int WinMainApp(HINSTANCE hInstance, int nCmdShow, App* app);

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	
	
	App sampleApp;
	return WinMainApp(hInstance, nCmdShow, &sampleApp);
}