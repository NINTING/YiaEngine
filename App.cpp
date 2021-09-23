#include"pch.h"

#include"App.h"
#include"Event/Event.h"
#include"Platform/WinWindow.h"
#include"Common/Logger.h"
namespace YiaEngine
{

	Application::Application():pWindow(nullptr)
	{
		YIA_INFO("App init");
	}
	

	void Application::Init()
	{
		WindowData win{ "YiaEngine",512,512 };
		pWindow = std::unique_ptr<Window>(Window::Create(win));
		pWindow->SetEventCallBack([this](Event& e) { this->OnEvent(e); });
	}
	void  Application::OnEvent(Event& e)
	{
		EventListener listener(e);
		listener.Listen<KeyCodeEvent>([](KeyCodeEvent& e) {YIA_INFO(e.KeyCode); return true; });
	}
	void Application::Run()
	{
		pWindow->OnUpdate();
	//	YIA_CORE_INFO("App Run");
		Update();
	}
	void Application::End()
	{
		YIA_CORE_INFO("App End");
		Destroy();
	}
}