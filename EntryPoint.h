#include"pch.h"


#include"App.h"
#include"YiaWindow.h"
#include"Common/Logger.h"
#ifdef YIA_WINDOW


int main()
{
	// HWND g_hwnd = nullptr;
	LOG_INIT();
	YiaEngine::Application* app = CreateApplication();
	app->Init();
	while (true)
	{
		app->Run();
	}
	app->End();
	
    return 0;

}

#endif // YIA_WINDOW
