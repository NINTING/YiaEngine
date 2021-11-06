#pragma once
#include"pch.h"
#include"Event/Event.h"
namespace YiaEngine
{

	struct WindowData
	{

		WindowData(const char* Name, UINT width, UINT height) 
			:Name(Name), Width(width), Height(height), LTPosX(0), LTPosY(0){}
		std::string Name;
		UINT Width;
		UINT Height;
		UINT LTPosX;
		UINT LTPosY;
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
		static void Dispatch(Event& e) {if(s_Window!=nullptr&&s_Window->callBack_) s_Window->callBack_(e); };
		static void ResizeScreen(UINT width, UINT height);
		static void FullScreen();
		static Window& CurrentWindow() { return *s_Window; }
	protected:
		virtual void Resize(UINT width, UINT height) = 0;
		virtual void ResizeFullScreen() = 0;
	protected:
		static std::unique_ptr<Window> s_Window;
		EventCallBack callBack_;
	};
	
	
}


