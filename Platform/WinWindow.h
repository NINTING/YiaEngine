#pragma once

#include"pch.h"


#include"YiaWindow.h"
namespace YiaEngine
{
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	class WinWindow : public Window
	{
		friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	public:
		WinWindow(const WindowData& data):windowData_(data){};
		// 通过 Window 继承
		virtual UINT GetWidth() override { return windowData_.Width; }
		virtual UINT GetHeight() override { return windowData_.Height; }
		virtual void Resize(UINT width, UINT height);
		virtual void ResizeFullScreen();
		virtual void OnUpdate() override;
		virtual void OnDestroy()override;
	
		virtual void* NativeHandle() override;
		LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		virtual void Init(const WindowData& data) override;
	protected:
	private:
		WindowData windowData_;
		HWND hwnd_;
		std::string name_;
		bool isFullScreen_;
		// 通过 Window 继承
	

	};
}

