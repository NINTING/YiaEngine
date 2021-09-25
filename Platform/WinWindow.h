#pragma once

#include"pch.h"


#include"YiaWindow.h"

namespace YiaEngine
{
	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowData& data):windowData_(data){ Init(data); };
		// 通过 Window 继承
		virtual UINT GetWidth() override { return windowData_.Width; }
		virtual UINT GetHeight() override { return windowData_.Height; }
		virtual void OnUpdate() override;
		virtual void OnDestroy()override;
		void OnEvent();
		virtual void* NativeHandle() override;
		LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
		virtual void Init(const WindowData& data) override;
	private:
		WindowData windowData_;
		HWND hwnd_;
		std::string name_;

		// 通过 Window 继承
	

	};
}

