#pragma once
#include"pch.h"
#include"Event.h"

namespace YiaEngine
{
	class KeyCodeEvent : public Event
	{
	public:
		KeyCodeEvent(int keyCode) :KeyCode(keyCode) {}
		REGISTER_EVENT_CLASS(EventCategoryKeyBoard, KeyCode);
	
		int KeyCode;
	};
}