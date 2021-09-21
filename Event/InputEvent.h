#pragma once
#include"pch.h"
#include"Event.h"

namespace YiaEngine
{
	class KeyCodeEvent : public Event
	{
		KeyCodeEvent(int keyCode) :keyCode_(keyCode) {}
		REGISTER_EVENT_CLASS(EventCategoryKeyBoard, KeyCode);
	private:
		int keyCode_;
	};
}