#pragma once
#include"pch.h"
#include"Event.h"

namespace YiaEngine
{
	class KeyCodeEvent : public Event
	{
	public:
		KeyCodeEvent(int keyCode) :Key(keyCode) {}
		REGISTER_EVENT_CLASS(EventCategoryKeyBoard, KeyCode);
	
		int Key;
	};

	class MouseMoveEvent:public Event
	{
	public:
		MouseMoveEvent(int x, int y) :X(x), Y(y) {}
		REGISTER_EVENT_CLASS(EventCategoryMouse, MouseMove);
		int X,  Y;
	};
	class MouseButtonDownEvent :public Event
	{
	public:
		MouseButtonDownEvent(int x, int y) :X(x), Y(y) {}
		REGISTER_EVENT_CLASS(EventCategoryMouse, MouseButtonDown);
		virtual std::string ToString() const override{
			std::stringstream ss;
			ss << "MouseDown:";
			ss << "CursorPos: ";
			ss << X <<" , " << Y;
			return ss.str();
		}
		int X, Y;
	};
	class MouseButtonRealseEvent :public Event
	{
	public:
		MouseButtonRealseEvent(int x, int y) :X(x), Y(y) {}
		REGISTER_EVENT_CLASS(EventCategoryMouse, MouseButtonRelease);
		int X, Y;
	};
}