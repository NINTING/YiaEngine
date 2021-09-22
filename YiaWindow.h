#pragma once
#include"pch.h"

namespace YiaEngine
{

	struct WindowData
	{

		WindowData(const char* name, size_t width, size_t height) 
			:Name(name), Width(width), Height(height) {}
		std::string Name;
		size_t Width;
		size_t Height;
	};
	class Window
	{
	public:
		Window() {};
		virtual void Init(const WindowData& data) = 0;
		virtual void OnUpdate() = 0;
		virtual size_t GetWidth() = 0;
		virtual size_t GetHeight() = 0;
		static Window* Create(const WindowData& data);
	private:
		static Window* s_Window;

	};
	
	
}


