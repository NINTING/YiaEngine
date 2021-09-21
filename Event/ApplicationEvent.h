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
}