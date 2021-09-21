#include"pch.h"


#include"App.h"

#ifdef YIA_WINDOW

extern YiaEngine::Application* CreateApplication();
int main()
{
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
