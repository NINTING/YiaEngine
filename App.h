#pragma once
#include"pch.h"

#include"Event/Event.h"

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
		void OnEvent(Event& e);
	private:
		std::unique_ptr<Window>pWindow;
	};
}
YiaEngine::Application* CreateApplication();