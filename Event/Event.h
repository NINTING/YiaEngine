#pragma once
#include"pch.h"

namespace YiaEngine
{
	enum EventType
	{
		/*Graphic*/
		RenderBegin,
		/*KeyBoard*/
		KeyCode,
		
	};
	enum EventCategory
	{
		EventCategoryGraphic	= 1<<1,
		EventCategoryKeyBoard	= 1<<2,
		EventCategoryMouse		= 1<<3
	};
	class Event
	{
	public:
		virtual std::string Name()const = 0;
		virtual int GetEventCategory()const = 0;
		virtual EventType GetEventType() const = 0;
		virtual std::string ToString()const { return Name(); }
		bool Handled;
	};
	inline std::ostream& operator << (std::ostream& os, const Event & e)
	{
		return os << e.ToString();
	}

#define REGISTER_EVENT_CLASS(category,type)							\
	static EventType GetStaticEventType() { return EventType::##type; }\
	virtual EventType GetEventType() const override { return GetStaticEventType() ; }\
	virtual std::string Name()const{ return #type; };	\
	virtual int GetEventCategory()const{return category;}
	


	class EventDispatch
	{
	public:
		EventDispatch(Event& e) :event_(e) {}

		template<typename T,template<typename T>typename F>
		bool Dispatch(const F<bool(T&)>& func)
		{
			if (event_.GetEventType() == T::GetStaticEventType())
			{
				event_.Handled = func(static_cast<T&>(event_));
				return true;
			}
			return false;
		}
	private:
		Event& event_;
	};
}




