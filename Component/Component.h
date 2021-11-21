#pragma once



namespace YiaEngine
{
	class GameObject;
	class Component
	{
		
	public:
		Component(GameObject& obj) :gameObject(obj) {}
		virtual void Update() {};
	protected:
		GameObject& gameObject;
	};
}