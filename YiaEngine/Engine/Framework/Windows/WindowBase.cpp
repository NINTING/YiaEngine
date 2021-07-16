


#include"Dx12Sample.h"
#include "WindowBase.h"
HWND g_hwnd = nullptr;
// Main message handler for the sample.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


    switch (message)
    {
    case WM_CREATE:
    {
        // Save the DXSample* passed in to CreateWindow.
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        return 0;
    }
    case WM_LBUTTONDOWN:
        MessageBox(0, "hello world","Hello", MB_OK);
        return 0;
    case WM_LBUTTONDBLCLK:

        return 0;



    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    // Handle any messages the switch statement didn't.
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WinMainApp( HINSTANCE hInstance, int nCmdShow, App* app)
{
    // Parse the command line parameters
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
   
    LocalFree(argv);

    // Initialize the window class.
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = LPCSTR( L"DXSampleClass");
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, static_cast<LONG>(app->g_width), static_cast<LONG>(app->g_height) };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // Create the window and store a handle to it.
    g_hwnd = CreateWindow(
        windowClass.lpszClassName,
        "WinMain",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,        // We have no parent window.
        nullptr,        // We aren't using menus.
        hInstance,NULL);

    // Initialize the sample. OnInit is defined in each child-implementation of DXSample.
   

    ShowWindow(g_hwnd, nCmdShow);
    app->InitEngine();
    app->LoadPipeline(g_hwnd);
    app->LoadAsset();
    // Main sample loop.
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        // Process any messages in the queue.
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
         
            app->Render();
        }
    }
    app->Destroy();
    

    // Return this part of the WM_QUIT message to Windows.
    return static_cast<char>(msg.wParam);
}

