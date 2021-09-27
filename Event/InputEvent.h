#pragma once
#include"pch.h"
#include"Event.h"

namespace YiaEngine
{

	enum MouseButton : int
	{
		LButton = 0,
		RButton = 2,
		MButton = 1,
	};
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
		MouseButtonDownEvent(int x, int y,MouseButton state) :X(x), Y(y),State(state) {}
		REGISTER_EVENT_CLASS(EventCategoryMouse, MouseButtonDown);
		virtual std::string ToString() const override{
			std::stringstream ss;
			ss << "MouseDown:";
			ss << "CursorPos: ";
			ss << X <<" , " << Y;
			return ss.str();
		}
		int X, Y;
		MouseButton State;
	};
	class MouseButtonReleaseEvent :public Event
	{
	public:
		MouseButtonReleaseEvent(int x, int y, MouseButton state) :X(x), Y(y), State(state) {}
		REGISTER_EVENT_CLASS(EventCategoryMouse, MouseButtonRelease);
		int X, Y;
		MouseButton State;
	};
	class MouseWheelEvent :public Event
	{
	public:
		MouseWheelEvent(float delta) :Delta(delta){}
		REGISTER_EVENT_CLASS(EventCategoryMouse, MouseWheel);
		float Delta;
	};


}