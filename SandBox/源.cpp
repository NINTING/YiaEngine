#include<YiaEngine.h>
using namespace YiaEngine;

class SampleLayer :public Layer
{
public:
	SampleLayer():Layer("SampleLayer"){}
	virtual void OnEvent(Event& e) {
		EventListener listener(e);
		listener.Listen<MouseButtonDownEvent>([](MouseButtonDownEvent& e) {
			YIA_INFO("SampleLayer"); 
			YIA_INFO(e);
			return true;
		});
	};
	virtual void OnUpdate() {};
};

class SampleApp :public Application
{
public:
	SampleApp() :Application(), sampleLayer() {}
	virtual void OnEvent(Event& e)
	{
		Application::OnEvent(e);

	}
	virtual void Init() 
	{
		Application::Init();
		imGuiLayer.OnAwake();
		PushLayer(&sampleLayer);
		PushLayer(&imGuiLayer);
	};
	virtual void Update()
	{
		imGuiLayer.OnUpdate();
	}
	SampleLayer sampleLayer; 
	ImguiLayer imGuiLayer;
};


Application* CreateApplication()
{
	return new SampleApp();
}