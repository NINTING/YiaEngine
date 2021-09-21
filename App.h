#pragma once
#include"pch.h"  
namespace YiaEngine
{
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
		
	};
}
YiaEngine::Application* CreateApplication();