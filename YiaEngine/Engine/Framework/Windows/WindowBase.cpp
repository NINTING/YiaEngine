


#include"Dx12Sample.h"
#include "WindowBase.h"
#include "imgui.h"
#include "ImGui/backend/imgui_impl_win32.h"
#include "ImGui/backend/imgui_impl_dx12.h"
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    app->InitEngine();
    app->LoadPipeline(g_hwnd);
    
    ImGui_ImplWin32_Init(g_hwnd);
    ImGui_ImplDX12_Init(app->g_device.Get(), app->frames_count_,
        DXGI_FORMAT_R8G8B8A8_UNORM, app->g_SRVHeap.Get(),
        app->g_SRVHeap->GetCPUDescriptorHandleForHeapStart(),
        app->g_SRVHeap->GetGPUDescriptorHandleForHeapStart());

    app->LoadAsset();
    bool done = false;
    MSG msg = {};
    while (!done)
    {

        // Main sample loop.
        msg = {};
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            // Process any messages in the queue.

            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;
       ImGui_ImplDX12_NewFrame();
       ImGui_ImplWin32_NewFrame();
       ImGui::NewFrame();

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //ImGui::Checkbox("Another Window", &show_another_window);

 //       ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //    counter++;
        ImGui::SameLine();
 //       ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

  
        ImGui::Render();

        app->WaitForPreviousFrame();
        //app->PopulateSceneCommandList();
        //app->Render();
        
        

        app->PopulateSceneCommandList();
    //    app->ExecuteCommand();

        app->PopulateUICommandList();
        app->ExecuteCommand();

        app->SwapPresent();

    }
    app->Destroy();
    
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // Return this part of the WM_QUIT message to Windows.
    return static_cast<char>(msg.wParam);
}

