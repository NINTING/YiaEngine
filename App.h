#pragma once
#include"pch.h"

#include"Event/Event.h"
#include"Common/Layer.h"
namespace YiaEngine
{
	class Window;
	class YIA_API Application
	{
	public:
		Application();
		virtual void Init();
		virtual void Update() = 0;
		virtual void Destroy() {};
		void Start();
		void Run();
		void End();
		virtual void OnEvent(Event& e);

		void PushLayer(Layer*);
		void PushLayerOverlay(Layer*);
		void PopLayer(Layer*);
		void PopLayerOverlay(Layer*);
	private:
		std::unique_ptr<Window>pWindow;
		LayerStack layerStack_;
	};
}
YiaEngine::Application* CreateApplication();