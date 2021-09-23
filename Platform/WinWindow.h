#pragma once

#include"pch.h"


#include"YiaWindow.h"

namespace YiaEngine
{
	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowData& data):windowData_(data){ Init(data); };
		// Í¨¹ý Window ¼Ì³Ð
		virtual size_t GetWidth() override { return windowData_.Width; }
		virtual size_t GetHeight() override { return windowData_.Height; }
		virtual void OnUpdate() override;
		LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void OnEvent();
		
	protected:
		virtual void Init(const WindowData& data) override;
	private:
		WindowData windowData_;
		HWND hwnd_;
		
	};
}

