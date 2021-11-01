#include "pch.h"
#include "Input.h"
#include"Platform/WindowInput.h"
namespace YiaEngine
{
	Input* Input::s_Instance;
	void Input::HandleMessage(void* msg)
	{
		if (s_Instance == nullptr)
		{
#ifdef YIA_WINDOW
			s_Instance = new WindowInput();
#endif // YIA_WINDOW

		}
		s_Instance->HandleMessageImpl(msg);
	}
}