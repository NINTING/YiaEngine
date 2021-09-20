#pragma once



#include <windows.h>
#include <windowsx.h>
#include <initguid.h>
#include <d3d12.h>
#include <DXGI1_4.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <dxgi1_6.h>
#include<stdexcept>
#include <string>

#include"DxSample.h"
//#include "d3dx12.h"
HWND g_hwnd = nullptr;
// Main message handler for the sample.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  /*  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;*/

    switch (msg)
    {
    case WM_SIZE:
        /* if(app-> Graphic::g_device !=NULL && wParam != SIZE_MINIMIZED)
         {
             ImGui_ImplDX12_InvalidateDeviceObjects();


             ImGui_ImplDX12_CreateDeviceObjects();
         }*/
        return 0;
    case WM_CREATE:
    {
        // Save the DXSample* passed in to CreateWindow.
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    // Handle any messages the switch statement didn't.
    return DefWindowProc(hWnd, msg, wParam, lParam);


}

void DrawUI()
{

}

int WinMainApp(HINSTANCE hInstance, int nCmdShow)
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
    windowClass.lpszClassName = LPCWSTR(L"DXSampleClass");
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, 1024, 1024 };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // Create the window and store a handle to it.
    g_hwnd = CreateWindow(
        windowClass.lpszClassName,
        L"WinMain",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,        // We have no parent window.
        nullptr,        // We aren't using menus.
        hInstance, NULL);

    // Initialize the sample. OnInit is defined in each child-implementation of DXSample.


    ShowWindow(g_hwnd, nCmdShow);
    MSG msg = {};
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //ImGui::StyleColorsDark();

    //app->InitEngine();
    //app->LoadPipeline(g_hwnd);

    //auto font_srv_handle = Graphic::g_GpuCommonDescriptoHeap.Alloc();
    //ImGui_ImplWin32_Init(g_hwnd);
    //ImGui_ImplDX12_Init(Graphic::g_Device.Get(), app->frames_count_,
    //    DXGI_FORMAT_R8G8B8A8_UNORM, Graphic::g_GpuCommonDescriptoHeap.RawHeap(),
    //    font_srv_handle,
    //    font_srv_handle);

    //app->LoadAsset();
    //bool done = false;
    //MSG msg = {};
    //while (!done)
    //{

    //    // Main sample loop.
    //    msg = {};
    //    while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
    //    {
    //        ::TranslateMessage(&msg);
    //        ::DispatchMessage(&msg);
    //        if (msg.message == WM_QUIT)
    //            done = true;
    //    }
    //    if (done)
    //        break;
    //    ImGui_ImplDX12_NewFrame();
    //    ImGui_ImplWin32_NewFrame();
    //    ImGui::NewFrame();
    //    //  app->WaitForPreviousFrame();

    //      //app->PopulateSceneCommandList();
    //      //app->Render();

    //    YiaEngine::Time::DetalTime = io.DeltaTime;
    //    //===============   GAMEPLAY   ======================





    //    app->PopulateScene();
    //    app->Update();
    //    //===============   UI   ======================

    //    app->DrawUI();

    //    app->PopulateUICommandList();

    //    //app->ExecuteCommand();
    //    //app->WaitForPreviousFrame();

    //    app->SwapPresent();

    //}
    //app->Destroy();

    //ImGui_ImplDX12_Shutdown();
    //ImGui_ImplWin32_Shutdown();
    //ImGui::DestroyContext();

    // Return this part of the WM_QUIT message to Windows.
    return static_cast<char>(msg.wParam);
}


int main()
{
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    LocalFree(argv);

    // Initialize the window class.
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = GetModuleHandle(0);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = LPCWSTR(L"DXSampleClass");
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, 1024, 1024 };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // Create the window and store a handle to it.
    g_hwnd = CreateWindow(
        windowClass.lpszClassName,
        L"WinMain",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,        // We have no parent window.
        nullptr,        // We aren't using menus.
        GetModuleHandle(0), NULL);

    // Initialize the sample. OnInit is defined in each child-implementation of DXSample.


    ShowWindow(g_hwnd, SW_SHOWDEFAULT);
    MSG msg = {};
    App app;
    app.InitEngine();
    app.LoadPipeline(g_hwnd);
    app.LoadAsset();
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
        app.PopulateScene();
        app.Update();
        //===============   UI   ======================

     //   app.DrawUI();

     //   app.PopulateUICommandList();

        //app->ExecuteCommand();
        //app->WaitForPreviousFrame();

        app.SwapPresent();

    }
    app.Destroy();
    return 0;
}