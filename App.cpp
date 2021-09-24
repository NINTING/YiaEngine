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
		listener.Listen<KeyCodeEvent>([](KeyCodeEvent& e) {YIA_INFO(e.Key); return true; });
		listener.Listen<MouseMoveEvent>([](MouseMoveEvent& e) {YIA_INFO("MousePos:{0},{1}",e.X, e.Y); return true; });
		listener.Listen<MouseButtonDownEvent>([](MouseButtonDownEvent& e) {YIA_INFO("MouseDownPos:{0},{1}", e.X, e.Y); return true; });
		listener.Listen<MouseButtonRealseEvent>([](MouseButtonRealseEvent& e) {YIA_INFO("MouseRealsePos:{0},{1}", e.X, e.Y); return true; });
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