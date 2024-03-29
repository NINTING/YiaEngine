#pragma once
#include"pch.h"

#include"Event/Event.h"
#include"Layer/Layer.h"
#include"Common/TimeStep.h"
namespace YiaEngine
{
	class Window;
	class YIA_API Application
	{
	public:
		Application();
		virtual void Init();
		void InitGraphicEngine();
		virtual void Update(Timestep  timestep) = 0;
		virtual void Destroy() {};
		void Start();
		void Run();
		void End();
		virtual void OnEvent(Event& e);
		Window& CurrentWindow();

		void PushLayer(Layer*);
		void PushLayerOverlay(Layer*);
		void PopLayer(Layer*);
		void PopLayerOverlay(Layer*);
		bool IsClose();
	protected:
		bool OnWindowCloseEvent(WindowCloseEvent);
	private:
		bool OnResizeEvent(WindowResizeEvent& e);
	private:
		bool isWindowClose_;
		LayerStack layerStack_;
		//Timestep timestep_;
		float lastFrameTime;
	};
}
YiaEngine::Application* CreateApplication();