#include"pch.h"

#include <dxgi1_6.h>

#include"App.h"
#include"Event/Event.h"
#include"Platform/WinWindow.h"
#include"Common/Logger.h"
#include"Core/Graphic.h"
#include"Core/CommandManager.h"
#include"Core/RenderBuffer.h"
namespace YiaEngine
{

	Application::Application()
	{
		YIA_INFO("App init");
		isWindowClose_ = false;
	}
	

	void Application::Init()
	{
		WindowData win{ "YiaEngine",512,512};
		Window& window = Window::Create(win);
		window.SetEventCallBack([this](Event& e) { this->OnEvent(e); });
		InitGraphicEngine();

	}
	void Application::InitGraphicEngine()
	{
		ComPtr<IDXGIFactory4> factory;
		ASSERT_SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));
		ComPtr<IDXGIAdapter1> hardwareAdapter;

		Graphic::GetHardwareAdapter(factory.Get(), &hardwareAdapter, false);
		ASSERT_SUCCEEDED(D3D12CreateDevice(
			hardwareAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&Graphic::g_Device)
		));

		Graphic::g_commandManager.Create(Graphic::g_Device.Get());

		Window& window = Window::CurrentWindow();

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = SWAP_CHAIN_COUNT;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferDesc.Width = window.GetWidth();
		swapChainDesc.BufferDesc.Height = window.GetHeight();
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.OutputWindow =(HWND) window.NativeHandle();
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = true;

		ComPtr<IDXGISwapChain> swapChain;

		ASSERT_SUCCEEDED(factory->CreateSwapChain(Graphic::g_commandManager.GetQueue(D3D12_COMMAND_LIST_TYPE_DIRECT).NativeCommandQueue(), &swapChainDesc, &swapChain));

		ASSERT_SUCCEEDED(swapChain.As(&Graphic::g_SwapChain));

		//¿ØÖÆ´°¿ÚÇÐ»»(alt-enter)
		ASSERT_SUCCEEDED(factory->MakeWindowAssociation((HWND)window.NativeHandle(), DXGI_MWA_NO_ALT_ENTER));

		Graphic::g_FrameIndex = Graphic::g_SwapChain->GetCurrentBackBufferIndex();

		for (UINT i = 0; i < 2; i++)
		{
			ID3D12Resource* swapChainResource;
			ASSERT_SUCCEEDED(Graphic::g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&swapChainResource)));
			Graphic::g_SwapRenderTarget[i].CreateFromSwapChian(L"Screen Render Target", swapChainResource);
		}
	}
	void  Application::OnEvent(Event& e)
	{

		EventListener listener(e);
		listener.Listen<WindowResizeEvent>(BIND_MEMBER_CALLBACK(OnResizeEvent));
		listener.Listen<WindowCloseEvent>(BIND_MEMBER_CALLBACK(OnWindowCloseEvent));
		for (auto it = layerStack_.End(); it != layerStack_.Begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled == true)
				break;
		}
		
	}
	Window& Application::CurrentWindow()
	{
		return Window::CurrentWindow();
	}
	void Application::PushLayer(Layer*layer)
	{
		layerStack_.PushLayer(layer);
	}
	void Application::PushLayerOverlay(Layer* layer)
	{
		layerStack_.PushLayerOverlay(layer);
	}
	void Application::PopLayer(Layer* layer)
	{
		layerStack_.PopLayer(layer);
	}
	void Application::PopLayerOverlay(Layer* layer)
	{
		layerStack_.PopLayerOverlay(layer);
	}
	bool Application::IsClose()
	{
		return isWindowClose_;
	}
	bool Application::OnResizeEvent(WindowResizeEvent& e)
	{
		YIA_CORE_INFO("window resize {0},{1}", e.Width, e.Height);
		Graphic::ResizeScreen(e.Width, e.Height);
		return true;
	}
	void Application::Run()
	{

		Window::CurrentWindow().OnUpdate();
	
		if (IsClose())
			return;
	//	YIA_CORE_INFO("App Run");
		Update();

		ASSERT_SUCCEEDED(Graphic::g_SwapChain->Present(1, 0));
		Graphic::g_FrameIndex = Graphic::g_SwapChain->GetCurrentBackBufferIndex();
	}
	void Application::End()
	{
		YIA_CORE_INFO("App End");
		Destroy();
	}
	bool Application::OnWindowCloseEvent(WindowCloseEvent)
	{
		isWindowClose_ = true;
		return true;
	}
}