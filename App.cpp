#include"pch.h"

#include"App.h"
#include"Event/ApplicationEvent.h"
namespace YiaEngine
{

	Application::Application()
	{
		YIA_INFO("App init");
	}
	
	bool func(RenderBeginEvent& e)
	{
		YIA_INFO("ÆÕÍ¨º¯Êý");
		return true;
	}
	void Application::Init()
	{
		
		RenderBeginEvent e;
		EventDispatch dispatch(e);
		std::function<bool(RenderBeginEvent&)> f =  [](RenderBeginEvent& e) {YIA_INFO(e); return true; };
		dispatch.Dispatch(f);
	//	dispatch.Dispatch(func);
	}

	void Application::Run()
	{
	//	YIA_CORE_INFO("App Run");
		Update();
	}
	void Application::End()
	{
		YIA_CORE_INFO("App End");
		Destroy();
	}
}