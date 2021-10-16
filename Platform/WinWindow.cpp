#include "pch.h"
#include "WinWindow.h"
#include "Common/logger.h"
#include"Event/Event.h"
#include"Core/Graphic.h"
namespace YiaEngine
{
    std::unique_ptr<Window> Window::s_Window;
    Window& Window::Create(const WindowData& data)
    {
        s_Window = std::unique_ptr<Window>(new WinWindow(data));
        s_Window->Init(data);
        return *s_Window;
    }

    void Window::ResizeScreen(UINT width, UINT height)
    {
        if (s_Window != nullptr)
        {
            s_Window->Resize(width, height);
        }
    }
    void Window::FullScreen()
    {
        if (s_Window != nullptr)
        {
            s_Window->ResizeFullScreen();
        }
    }

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {

        case WM_SIZE:
        {
            if (wParam == SIZE_RESTORED)
            {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                Window::ResizeScreen(width, height);
                WindowResizeEvent event(width, height);
                Window::Dispatch(event);

            }
            else if (wParam == SIZE_MAXIMIZED)
            {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                Window::ResizeScreen(width, height);
                WindowResizeEvent event(width, height);
                Window::Dispatch(event);
                //Window::FullScreen();
            }

            return 0;
        }

        case WM_MOUSEACTIVATE:
            return 0;
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

            break;
        }
        case WM_LBUTTONDOWN:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            MouseButtonDownEvent event(xPos, yPos, MouseButton::LButton);
            YIA_INFO("button event dispatch");
            Window::Dispatch(event);
            break;
        }
        case WM_LBUTTONUP:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            MouseButtonReleaseEvent event(xPos, yPos, MouseButton::LButton);
            Window::Dispatch(event);
            break;
        }
        case WM_MOUSEMOVE:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            MouseMoveEvent event(xPos, yPos);
            Window::Dispatch(event);
            break;
        }
        case WM_MOUSEWHEEL:
        {

            float  zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            MouseWheelEvent event(zDelta);
            Window::Dispatch(event);
            return 0;
        }
        case WM_DESTROY:
        {
            WindowCloseEvent event;
            Window::Dispatch(event);
            PostQuitMessage(0);
            return 0;
        }
        
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    void YiaEngine::WinWindow::Init(const WindowData& data)
    {
        name_ = data.Name;
        WNDCLASSEX windowClass = { 0 };
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = GetModuleHandle(0);
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.lpszClassName = LPCWSTR(data.Name.c_str());
        RegisterClassEx(&windowClass);
        
        RECT windowRect = { data.LTPosX, data.LTPosY,(LONG)data.Width,(LONG) data.Height };
        
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

        ShowWindow(hwnd_, SW_MAXIMIZE);
        ::UpdateWindow(hwnd_);
    }

    void* WinWindow::NativeHandle()
    {
        return hwnd_;
    }

    void WinWindow::Resize(UINT width, UINT height)
    {

        windowData_.Height = height;
        windowData_.Width = width;
    }

    void WinWindow::ResizeFullScreen()
    {

        if (isFullScreen_)
        {
            SetWindowLong(hwnd_, GWL_STYLE, WS_OVERLAPPEDWINDOW);

            SetWindowPos(
                hwnd_,
                HWND_NOTOPMOST,
                windowData_.LTPosX,
                windowData_.LTPosY,
                windowData_.Width,
                windowData_.Height,
                SWP_FRAMECHANGED | SWP_NOACTIVATE);

            ShowWindow(hwnd_, SW_NORMAL);
        }
        else
        {
            //RECT windowRect;
            //GetWindowRect(hwnd, &windowRect);
            SetWindowLong(hwnd_, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));
            RECT fullscreenWindowRect;
         

            ComPtr<IDXGIOutput> pOutput;
             (Graphic::g_SwapChain->GetContainingOutput(&pOutput));
            DXGI_OUTPUT_DESC Desc;
            ASSERT_SUCCEEDED(pOutput->GetDesc(&Desc));
            fullscreenWindowRect = Desc.DesktopCoordinates;

            SetWindowPos(
                hwnd_,
                HWND_TOPMOST,
                fullscreenWindowRect.left,
                fullscreenWindowRect.top,
                fullscreenWindowRect.right,
                fullscreenWindowRect.bottom,
                SWP_FRAMECHANGED | SWP_NOACTIVATE);


            ShowWindow(hwnd_, SW_MAXIMIZE);
        }
        isFullScreen_ = !isFullScreen_;
    }

    void YiaEngine::WinWindow::OnUpdate()
    {
        MSG msg = {};
        bool done = true;
    
        msg = {};
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        } 
  
    }

    void WinWindow::OnDestroy()
    {
        ::DestroyWindow(hwnd_);
        ::UnregisterClass(LPCWSTR(name_.c_str()), GetModuleHandle(0));
    }


}