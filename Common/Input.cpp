#include "pch.h"
#include "Input.h"
#include"Platform/WindowInput.h"
#include"Common/YiaMath.h"
namespace YiaEngine
{
	Input* Input::s_Instance;
	void Input::Update()
	{
		s_Instance->UpdateImpl();
	}
	void Input::HandleMessage(void* msg)
	{
		s_Instance->HandleMessageImpl(msg);
	}
	bool Input::IsKeyPress(int keycode)
	{
		return s_Instance->IsKeyPressedImpl(keycode);
	}
	void Input::Init()
	{
#ifdef YIA_WINDOW
		s_Instance = new WindowInput();
#endif // YIA_WINDOW
	}

	Math::Vec2f Input::MouseMove()
	{
		return s_Instance->mouseMove_;
	}
}