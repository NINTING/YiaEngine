#include "pch.h"
#include "ImguiLayer.h"
#include "Imgui/imgui.h"
#include "Imgui/ImguiDx12.h"
#include"Imgui/imgui_impl_win32.h"
#include"Core/Graphic.h"
#include"Core/GraphicContext.h"
#include"Core/CommandManager.h"
#include"Core/RenderBuffer.h"
#include "YiaWindow.h"
#include<type_traits>
namespace YiaEngine
{
    ImguiLayer::ImguiLayer() : Layer("ImGuiLayer"), time_(0)
    {
        gpuImGuiDescriptoHeap_.CreateShaderVisibleType(L"ImGuiDescriptorHeap", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1024);
    }
    void YiaEngine::ImguiLayer::OnAwake()
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        ImGui::StyleColorsDark();

        io.BackendPlatformName = "Yia_win32";
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
        
        io.ImeWindowHandle = Window::CurrentWindow().NativeHandle();

        io.KeyMap[ImGuiKey_Tab] = VK_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
        io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
        io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
        io.KeyMap[ImGuiKey_Home] = VK_HOME;
        io.KeyMap[ImGuiKey_End] = VK_END;
        io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
        io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
        io.KeyMap[ImGuiKey_Space] = VK_SPACE;
        io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
        io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = VK_RETURN;
        io.KeyMap[ImGuiKey_A] = 'A';
        io.KeyMap[ImGuiKey_C] = 'C';
        io.KeyMap[ImGuiKey_V] = 'V';
        io.KeyMap[ImGuiKey_X] = 'X';
        io.KeyMap[ImGuiKey_Y] = 'Y';
        io.KeyMap[ImGuiKey_Z] = 'Z';

        
        auto fontSrvHandle = gpuImGuiDescriptoHeap_.Alloc();
      //  ImGui_ImplWin32_Init(Window::CurrentWindow().NativeHandle());
        ImGui_ImplDX12_Init(Graphic::g_Device.Get(), SWAP_CHAIN_COUNT,
            DXGI_FORMAT_R8G8B8A8_UNORM, gpuImGuiDescriptoHeap_.NativeHeap(),
            fontSrvHandle,fontSrvHandle);
    }

    void YiaEngine::ImguiLayer::OnEvent(Event& e)
    {
        EventListener listener(e);
        listener.Listen<MouseMoveEvent>(BIND_MEMBER_CALLBACK(OnMouseMoveEvent));
        listener.Listen<MouseButtonDownEvent>(BIND_MEMBER_CALLBACK(OnMouseDownEvent));
        listener.Listen<MouseButtonReleaseEvent>(BIND_MEMBER_CALLBACK(OnMouseReleaseEvent));
        listener.Listen<MouseWheelEvent>(BIND_MEMBER_CALLBACK(OnMouseWheelEvent));

    }

    void YiaEngine::ImguiLayer::OnUpdate()
    {
        ImGuiIO& io = ImGui::GetIO();
    
        ImGui_ImplDX12_NewFrame();
        RECT rect = { 0, 0, 0, 0 };
        ::GetClientRect((HWND)Window::CurrentWindow().NativeHandle(), &rect);
  /*      io.DisplaySize = ImVec2((float)Window::CurrentWindow().GetWidth(),(float)Window::CurrentWindow().GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.f, 1.f);*/
       io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
        INT64 current_time = 0;
        ::QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
        io.DeltaTime = time_ > 0 ?(float)(current_time - time_) : 1.f/60.f;
        time_ = current_time;
        //ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();

        static bool show_demo_window = true;
        static bool show_another_window = true;
       ImGui::ShowDemoWindow(&show_demo_window);
        static int counter = 0;
       //ImGui::SetNextWindowSize(ImVec2(550, 680));
       // ImGui::Begin("Hello, world!");     // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
       // ////if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
       // ////    counter++;
       //
       // ImGui::End();

        ImGui::Render();


        Graphic::GraphicContext& UiContext = Graphic::GraphicContext::Begin();
        UiContext.SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, gpuImGuiDescriptoHeap_.NativeHeap());
        UiContext.TransitionBarrier(Graphic::g_SwapRenderTarget[Graphic::g_FrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET);
        UiContext.SetRenderTarget(Graphic::g_SwapRenderTarget[Graphic::g_FrameIndex].RtvCpuHandlePtr(), nullptr);
   

        UiContext.ClearRenderTarget(Graphic::g_SwapRenderTarget[Graphic::g_FrameIndex].RtvCpuHandle());

        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), UiContext.NativeCommandList());

        UiContext.TransitionBarrier(Graphic::g_SwapRenderTarget[Graphic::g_FrameIndex], D3D12_RESOURCE_STATE_PRESENT);
   
        UiContext.End();

      
    }
    void ImguiLayer::OnDestroy()
    {
        ImGui_ImplDX12_Shutdown();
        ImGui::DestroyContext();
        ImGuiIO& io = ImGui::GetIO();
        io.BackendPlatformName = NULL;
        io.BackendPlatformUserData = NULL;
       
    }
    bool ImguiLayer::OnMouseMoveEvent(MouseMoveEvent& e)
    {
        auto& io = ImGui::GetIO();
        io.MousePos = ImVec2(float(e.X), float(e.Y));
        return true;
    }
    bool ImguiLayer::OnMouseDownEvent(MouseButtonDownEvent& e)
    {
        auto& io = ImGui::GetIO();
        io.MouseDown[e.State] = true;

        YIA_INFO("ImguiLayer  Pos {0} {1}", e.X, e.Y);
        POINT pos;
        ::ScreenToClient((HWND)Window::CurrentWindow().NativeHandle(), &pos);
        YIA_INFO("ImguiLayer  CLIENT Pos {0} {1}", e.X, e.Y);
        return true;
        
    }
    bool ImguiLayer::OnMouseReleaseEvent(MouseButtonReleaseEvent& e)
    {
        auto& io = ImGui::GetIO();
        io.MouseDown[e.State] = false;
        return true;
    }
    bool ImguiLayer::OnMouseWheelEvent(MouseWheelEvent& e)
    {
        auto& io = ImGui::GetIO();
        io.MouseWheel = e.Delta / (float)WHEEL_DELTA;
        return true;
    }
}