// YiaEngine.cpp: 定义应用程序的入口点。
//

<<<<<<< HEAD


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
=======
#include "YiaEngine.h"
#include "Interface.h"
#include "Application.h"
using namespace std;

int main()
{
	cout << "Fuck CMake。" << endl;
	printf("dssas");
	f();
	app();
	return 0;
}
>>>>>>> 1b64f9a6cddcadbb4d87f89773874e0d8cff1652
