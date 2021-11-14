#pragma once
#include"pch.h"
#include"Graphic.h"
#include"CommandManager.h"
#include"DescriptorHeap.h"
#include"RenderBuffer.h"
#include"PipelineStateObject.h"
#include"Shader.h"
#include <dxgidebug.h>
namespace YiaEngine
{
	namespace Graphic
	{
		Microsoft::WRL::ComPtr<ID3D12Device> g_Device = nullptr;
		DescriptorHeap g_GpuCommonDescriptoHeap;
		CommandManager g_commandManager;
		ComPtr< IDXGISwapChain3> g_SwapChain;
		UINT g_FrameIndex;
		RenderBuffer g_SceneFinalTarget;
		CpuDescriptorAllocator g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] =
		{
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ,
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV ,
			D3D12_DESCRIPTOR_HEAP_TYPE_DSV
		};
		RenderBuffer g_SwapRenderTarget[2];

		void GetHardwareAdapter(
			IDXGIFactory1* pFactory,
			IDXGIAdapter1** ppAdapter,
			bool requestHighPerformanceAdapter)
		{
			*ppAdapter = nullptr;

			ComPtr<IDXGIAdapter1> adapter;

			ComPtr<IDXGIFactory6> factory6;
			if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
			{
				int adapterIndex = 0; // we'll start looking for directx 12  compatible graphics devices starting at index 0

				bool adapterFound = false; // set this to true when a good one was found

				// find first hardware gpu that supports d3d 12
				while (factory6->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
				{
					DXGI_ADAPTER_DESC1 desc;
					adapter->GetDesc1(&desc);

					if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
					{
						// we dont want a software device
						adapterIndex++; // add this line here. Its not currently in the downloadable project
						continue;
					}

					// we want a device that is compatible with direct3d 12 (feature level 11 or higher)
					ASSERT_SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr));


					adapterIndex++;
				}
			}
			else
			{
				for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
				{
					DXGI_ADAPTER_DESC1 desc;
					adapter->GetDesc1(&desc);

					if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
					{
						// Don't select the Basic Render Driver adapter.
						// If you want a software adapter, pass in "/warp" on the command line.
						continue;
					}

					// Check to see whether the adapter supports Direct3D 12, but don't create the
					// actual device yet.
					if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
					{
						break;
					}
				}
			}

			*ppAdapter = adapter.Detach();
		}

		void ResizeScreen(UINT width, UINT Height)
		{
			for (UINT i = 0; i < SWAP_CHAIN_COUNT; i++)
			{
				Graphic::g_SwapRenderTarget[i].Destroy();
			}
			ASSERT_SUCCEEDED( g_SwapChain->ResizeBuffers(SWAP_CHAIN_COUNT, width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
			for (UINT i = 0; i < SWAP_CHAIN_COUNT; i++)
			{
			
				ComPtr< ID3D12Resource> swapChainResource;
				ASSERT_SUCCEEDED(Graphic::g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&swapChainResource)));
				Graphic::g_SwapRenderTarget[i].CreateFromSwapChian(L"Screen Render Target", swapChainResource.Detach());
			}
			Graphic::g_FrameIndex = Graphic::g_SwapChain->GetCurrentBackBufferIndex();
		}
		void CommonShaderInit()
		{
			
		}
		int dxgiFactoryFlags = 0;
		void GraphicDebug()
		{
			uint32_t useDebugLayers = 0;
#if _DEBUG
			// Default to true for debug builds
			useDebugLayers = 1;
#endif
			if (useDebugLayers)
			{
				Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
				if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
				{
					debugInterface->EnableDebugLayer();

					uint32_t useGPUBasedValidation = 0;
			
					if (useGPUBasedValidation)
					{
						Microsoft::WRL::ComPtr<ID3D12Debug1> debugInterface1;
						if (SUCCEEDED((debugInterface->QueryInterface(IID_PPV_ARGS(&debugInterface1)))))
						{
							debugInterface1->SetEnableGPUBasedValidation(true);
						}
					}
				}
				else
				{
					Utility::Print("WARNING:  Unable to enable D3D12 debug validation layer\n");
				}

#if _DEBUG
				ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
				if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
				{
					dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

					dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
					dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

					DXGI_INFO_QUEUE_MESSAGE_ID hide[] =
					{
						80 /* IDXGISwapChain::GetContainingOutput: The swapchain's adapter does not control the output on which the swapchain's window resides. */,
					};
					DXGI_INFO_QUEUE_FILTER filter = {};
					filter.DenyList.NumIDs = _countof(hide);
					filter.DenyList.pIDList = hide;
					dxgiInfoQueue->AddStorageFilterEntries(DXGI_DEBUG_DXGI, &filter);
				}
#endif
			}
		}
		void GraphicInit()
		{
			GraphicDebug();
			ComPtr<IDXGIFactory4> factory;
			ASSERT_SUCCEEDED(CreateDXGIFactory2(dxgiFactoryFlags,IID_PPV_ARGS(&factory)));
			ComPtr<IDXGIAdapter1> hardwareAdapter;

			GetHardwareAdapter(factory.Get(), &hardwareAdapter, false);
			ASSERT_SUCCEEDED(D3D12CreateDevice(
				hardwareAdapter.Get(),
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&Graphic::g_Device)
			));

			Graphic::g_commandManager.Create(Graphic::g_Device.Get());
			PipelineStateObject::StaticPSOInit();
			CommonShaderInit();
		}
	}
}