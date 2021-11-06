#include "pch.h"
#include "ImguiLayer.h"
#include "Imgui/imgui.h"
#include "Imgui/ImguiDx12.h"



#include"Core/Graphic.h"
#include"Core/GraphicContext.h"
#include"Core/CommandManager.h"
#include"Core/RenderBuffer.h"
#include"imgui/backends/imgui_impl_win32.h"
#include "YiaWindow.h"
#include<type_traits>
namespace YiaEngine
{
    ImguiLayer::ImguiLayer() : Layer("ImGuiLayer"), time_(0),imageHandleAllocator_(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
    {
   
        gpuImGuiDescriptoHeap_.CreateShaderVisibleType(L"ImGuiDescriptorHeap", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1024);
    }
    Graphic::DescriptorHandle fontSrvHandle;
    void YiaEngine::ImguiLayer::OnAwake()
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
       
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        io.BackendPlatformName = "Yia_win32";
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
        
     //    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;

      /*  io.KeyMap[ImGuiKey_Tab] = VK_TAB;
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
        io.KeyMap[ImGuiKey_Z] = 'Z';*/
        ImGui_ImplWin32_Init(Window::CurrentWindow().NativeHandle());
        
     
      //  ImGui_ImplWin32_Init(Window::CurrentWindow().NativeHandle());
         fontSrvHandle = gpuImGuiDescriptoHeap_.Alloc();

         ImGui_ImplDX12_Init(Graphic::g_Device.Get(), SWAP_CHAIN_COUNT,
             DXGI_FORMAT_R8G8B8A8_UNORM, gpuImGuiDescriptoHeap_.NativeHeap(),
             fontSrvHandle, fontSrvHandle);

         sceneHandle = gpuImGuiDescriptoHeap_.Alloc();
      
    }

    void YiaEngine::ImguiLayer::OnEvent(Event& e)
    {
        EventListener listener(e);
        listener.Listen<WindowCloseEvent>(BIND_MEMBER_CALLBACK(OnWindowCloseEvent));
        listener.Listen<MouseMoveEvent>(BIND_MEMBER_CALLBACK(OnMouseMoveEvent));
        listener.Listen<MouseButtonDownEvent>(BIND_MEMBER_CALLBACK(OnMouseDownEvent));
        listener.Listen<MouseButtonReleaseEvent>(BIND_MEMBER_CALLBACK(OnMouseReleaseEvent));
        listener.Listen<MouseWheelEvent>(BIND_MEMBER_CALLBACK(OnMouseWheelEvent));
    }

    void YiaEngine::ImguiLayer::OnUpdate(Timestep timestep)
    {
       

      
    }

    void ImguiLayer::Begin()
    {

        //ImGuiIO& io = ImGui::GetIO();
        //io.BackendRendererUserData = NULL;
        //ImGui_ImplDX12_Init(Graphic::g_Device.Get(), SWAP_CHAIN_COUNT,
        //    DXGI_FORMAT_R8G8B8A8_UNORM, gpuImGuiDescriptoHeap_.NativeHeap(),
        //    fontSrvHandle, fontSrvHandle);

        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
    void ImguiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();

        //Graphic::GraphicContext& Context = Graphic::GraphicContext::Begin();
        //Context.TransitionBarrier(sceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
        //Context.SetRenderTarget(sceneColorBuffer.RtvCpuHandlePtr(), nullptr);
        //Context.ClearRenderTarget(sceneColorBuffer.RtvCpuHandle());
        //Context.TransitionBarrier(sceneColorBuffer, D3D12_RESOURCE_STATE_COMMON);

        //Context.End(true);

        Graphic::GraphicContext& UiContext = Graphic::GraphicContext::Begin();
      
        UiContext.SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, gpuImGuiDescriptoHeap_.NativeHeap());
        UiContext.TransitionBarrier(Graphic::g_SwapRenderTarget[Graphic::g_FrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET);
        UiContext.SetRenderTarget(Graphic::g_SwapRenderTarget[Graphic::g_FrameIndex].RtvCpuHandlePtr(), nullptr);
        UiContext.ClearRenderTarget(Graphic::g_SwapRenderTarget[Graphic::g_FrameIndex].RtvCpuHandle());
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), UiContext.NativeCommandList());
        UiContext.TransitionBarrier(Graphic::g_SwapRenderTarget[Graphic::g_FrameIndex], D3D12_RESOURCE_STATE_PRESENT);
        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault(NULL, Graphic::g_Device.Get());
        }
        UiContext.End(true);
    }
    void ImguiLayer::Render()
    {
        static bool show_demo_window = true;
        static bool show_another_window = true;
        DockSpace();
        SceneWindow();
        ImGui::ShowDemoWindow(&show_demo_window);
        static int counter = 0;
       
    }
    void ImguiLayer::OnDestroy()
    {
        ImGui_ImplDX12_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        
       
    }
    bool ImguiLayer::OnWindowCloseEvent(WindowCloseEvent& e)
    {
        OnDestroy();
        return true;
    }
    void ImguiLayer::DockSpace()
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        bool open = true;
        ImGui::Begin("DockSpace", &open, window_flags);


        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        else
        {
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
            ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::SmallButton("click here"))
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        }
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Option"))
            {
            //if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    
    void ImguiLayer::SceneWindow()
    {
        bool open = true;
        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        ImGui::Begin("Scene",&open);
        ImVec2 size = ImGui::GetWindowSize();
        
        if (size.x != sceneColorBuffer.Size().x() || size.y != sceneColorBuffer.Size().y())
        {
            sceneColorBuffer.Destroy();
            sceneColorBuffer.Create(L"SceneColorRT",size.x, size.y, DXGI_FORMAT_R8G8B8A8_UNORM);
            sceneDepthBuffer.Create(L"SceneDepthRT", size.x, size.y, DXGI_FORMAT_D32_FLOAT);
            UINT pSrcDescriptorRangeSizes[1] = { 1 };
            UINT pDestDescriptorRangeSizes[1] = { 1 };
            Graphic::g_Device->CopyDescriptors(
                1,
                sceneHandle.GetCpuAddress(),
                pSrcDescriptorRangeSizes,
                1,
                sceneColorBuffer.SrvCpuHandle().GetCpuAddress(),
                pSrcDescriptorRangeSizes,
                D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
            );
       //     sceneDepthBuffer.Destroy();
        //    sceneDepthBuffer.Create(L"SceneDepthRT", size.x, size.y, DXGI_FORMAT_D32_FLOAT);
        }
        ImGui::Image((ImTextureID)sceneHandle.GetGpuAddress()->ptr, size);
    
        ImGui::End();
    }
    void ImguiLayer::Image(const Graphic::GpuTexture& texture, const ImVec2& size)
    {
        //TODO: 通过GpuDescrptorAllocator 分配GpuHandle.不需要手动绑定
       /* UINT pSrcDescriptorRangeSizes[1] = { 1 };
        UINT pDestDescriptorRangeSizes[1] = { 1 };
       Graphic::DescriptorHandle gpuHandle = gpuImGuiDescriptoHeap_.Alloc();
   
       Graphic::g_Device->CopyDescriptors(
           1,
           gpuHandle.GetCpuAddress(),
           pSrcDescriptorRangeSizes,
           1,
           texture.SrvHandle().GetCpuAddress(),
           pSrcDescriptorRangeSizes,
           D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
       );

        ImGui::Image((ImTextureID)gpuHandle.GetGpuAddress()->ptr,size);*/
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