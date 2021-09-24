#pragma once
#include"pch.h"
#include"Event/Event.h"
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
		using EventCallBack = std::function<void(Event&)>;
		Window() {};
		virtual void Init(const WindowData& data) = 0;
		virtual void OnUpdate() = 0;
		virtual size_t GetWidth() = 0;
		virtual size_t GetHeight() = 0;
		static Window* Create(const WindowData& data);
		virtual void SetEventCallBack(const EventCallBack& callBack) { callBack_ = callBack; };
		static void Dispatch(Event& e) {if(s_Window!=nullptr) s_Window->callBack_(e); };
	protected:
		static Window* s_Window;
		EventCallBack callBack_;
	};
	
	
}


