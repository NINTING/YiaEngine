#include "pch.h"
#include "WindowInput.h"
#include"Event/InputEvent.h"
#include"WinWindow.h"
#include "Common/logger.h"
namespace YiaEngine
{
	bool YiaEngine::WindowInput::IsKeyPressedImpl(int keycode)
	{

		return false;
	}
	void WindowInput::HandleMessageImpl(void* pMsg)
	{
		
		MSG* msg = (MSG*)(pMsg);
		
		UINT message = msg->message;
		WPARAM wParam = msg->wParam;
		LPARAM lParam = msg->lParam;
		switch (message)
		{
		case WM_CHAR:
		{
			KeyCodeEvent event{ char(wParam) };
			Window::Dispatch(event);
			YIA_INFO("keycode {0}", char(wParam));
			break;
		}
		}

	}
}