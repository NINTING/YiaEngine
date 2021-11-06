#include "pch.h"
#include "WindowInput.h"
#include"Event/InputEvent.h"
#include"WinWindow.h"
#include "Common/logger.h"
namespace YiaEngine
{
	void WindowInput::UpdateImpl()
	{
		if (textCount_)
		{
			YIA_INFO(" ‰»Î{0}", textBuffer_);
		}
	
		textCount_ = 0;
		memset(textBuffer_, 0, sizeof(textBuffer_));
		mouseMove_ = Math::Vec2f::Zero();
	}
	bool YiaEngine::WindowInput::IsKeyPressedImpl(int keycode)
	{
		return keyButtonState[keycode];
	}
	void WindowInput::HandleMessageImpl(void* pMsg)
	{
		
		MSG* msg = (MSG*)(pMsg);
		
		UINT message = msg->message;
		WPARAM wParam = msg->wParam;
		LPARAM lParam = msg->lParam;
		bool pressed = false;
		unsigned winKey;
		switch (message)
		{
		case WM_CHAR:
		{
			KeyCodeEvent event{ char(wParam) };
			Window::Dispatch(event);
			textBuffer_[textCount_++] = char(wParam);

		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			pressed = true;
			winKey = (uint32_t)wParam;
			keyButtonState[winKey] = true;
			YIA_INFO(" ‰»Î{0}", winKey);
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			pressed = false;
			winKey = (uint32_t)wParam;
			keyButtonState[winKey] = false;
			break;


		case WM_RBUTTONDOWN:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			winKey = (uint32_t)wParam;
			YIA_INFO(" ‰»Î{0}", winKey);
			MouseButtonDownEvent event(xPos, yPos, MouseButton::RButton);
			YIA_INFO("button event dispatch");
			Window::Dispatch(event);
			
			keyButtonState[VirtualKey::RightMouse] = true;
			break;
		}
		case WM_RBUTTONUP:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			winKey = (uint32_t)wParam;
			MouseButtonReleaseEvent event(xPos, yPos, MouseButton::RButton);
			YIA_INFO("button event dispatch");
			Window::Dispatch(event);
			
			keyButtonState[VirtualKey::RightMouse] = false;
			break;
		}
		case WM_MOUSEMOVE:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
		
			MouseMoveEvent event(xPos, yPos);
			Window::Dispatch(event);
			mouseMove_ = { xPos - mousePos_.x(),yPos - mousePos_.y()};

			mousePos_ = { xPos ,yPos };
			break;
		}
		}

	}
}