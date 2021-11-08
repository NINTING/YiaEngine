#pragma once
#include"pch.h"
#include"Event.h"

namespace YiaEngine
{
	class RenderBeginEvent : public Event
	{
	public:
		REGISTER_EVENT_CLASS(EventCategoryGraphic,RenderBegin);
		virtual std::string Tostring() { return Name(); }
	};


	class WindowCloseEvent : public Event
	{
	public:
		REGISTER_EVENT_CLASS(EventCategoryApplication, WindowClose);
		virtual std::string Tostring() { return Name(); }
	};
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(UINT width, UINT Height):Width(width),Height(Height){}
		REGISTER_EVENT_CLASS(EventCategoryApplication, WindowResize);
		virtual std::string Tostring() { return Name(); }
		UINT Width, Height;
	};
}