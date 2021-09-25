#pragma once
#include"pch.h"
#include"Event/Event.h"
namespace YiaEngine
{

	struct WindowData
	{

		WindowData(const char* name, UINT width, UINT height) 
			:Name(name), Width(width), Height(height) {}
		std::string Name;
		UINT Width;
		UINT Height;
	};
	class Window
	{
		
	public:
		using EventCallBack = std::function<void(Event&)>;
		Window() {};
		virtual void Init(const WindowData& data) = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDestroy() = 0;
		virtual UINT GetWidth() = 0;
		virtual UINT GetHeight() = 0;
		virtual void* NativeHandle() = 0;
		static Window& Create(const WindowData& data);
		virtual void SetEventCallBack(const EventCallBack& callBack) { callBack_ = callBack; };
		static void Dispatch(Event& e) {if(s_Window!=nullptr) s_Window->callBack_(e); };
		static Window& CurrentWindow() { return *s_Window; }
	protected:
		static std::unique_ptr<Window> s_Window;
		EventCallBack callBack_;
	};
	
	
}


