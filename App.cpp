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
		for (auto it = layerStack_.End(); it != layerStack_.Begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled == true)
				break;
		}
		
	}
	void Application::PushLayer(Layer*layer)
	{
		layerStack_.PushLayer(layer);
	}
	void Application::PushLayerOverlay(Layer* layer)
	{
		layerStack_.PushLayerOverlay(layer);
	}
	void Application::PopLayer(Layer* layer)
	{
		layerStack_.PopLayer(layer);
	}
	void Application::PopLayerOverlay(Layer* layer)
	{
		layerStack_.PopLayerOverlay(layer);
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