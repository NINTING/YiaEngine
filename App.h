#pragma once
#include"pch.h"

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
	private:
		std::unique_ptr<Window>pWindow;
	};
}
YiaEngine::Application* CreateApplication();