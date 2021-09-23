#include "pch.h"
#include "WinWindow.h"
#include "Common/logger.h"
#include"Event/Event.h"
namespace YiaEngine
{
    Window* Window::s_Window;
    Window* Window::Create(const WindowData& data)
    {
        auto ret = new WinWindow(data);
        s_Window = ret;
        return ret;
    }


    LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_SIZE:
            return 0;
        case WM_MOUSEACTIVATE:

        case WM_CREATE:
        {
            // Save the DXSample* passed in to CreateWindow.
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
            return 0;
        }

        case WM_CHAR:
        {
            KeyCodeEvent event{ char(wParam) };
            Window::Dispatch(event);

            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    void YiaEngine::WinWindow::Init(const WindowData& data)
    {
        WNDCLASSEX windowClass = { 0 };
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = GetModuleHandle(0);
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.lpszClassName = LPCWSTR(data.Name.c_str());
        RegisterClassEx(&windowClass);

        RECT windowRect = { 0, 0,(LONG)data.Width,(LONG) data.Height };
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
        hwnd_ = CreateWindow(
            windowClass.lpszClassName,
            L"main",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,        // We have no parent window.
            nullptr,        // We aren't using menus.
            GetModuleHandle(0), NULL);

        ShowWindow(hwnd_, SW_SHOWDEFAULT);
      
    }

    void YiaEngine::WinWindow::OnUpdate()
    {
        MSG msg = {};
        bool done = true;
        while (done)
        {
            msg = {};
            while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
                if (msg.message == WM_QUIT)
                    done = true;
            }
        }
    }


}