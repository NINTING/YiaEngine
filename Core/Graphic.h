#pragma once
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"D3D12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")


#include<d3d12.h>
#include <dxgi1_6.h>

#include <wrl/client.h>

#include"d3dx12.h"
#include"Utility.h"
using namespace Microsoft::WRL;


#define ADDRESS_UNKOWN -1
#define ADDRESS_NULL 0

namespace YiaEngine
{

	namespace Graphic
	{
		class CommandManager;
		class CpuDescriptorAllocator;
		class DescriptorHeap;
		class RenderBuffer;

		extern CommandManager g_commandManager;
		extern ComPtr<ID3D12Device> g_Device;
		extern DescriptorHeap g_GpuCommonDescriptoHeap;
		extern ComPtr< IDXGISwapChain3> g_SwapChain;
		extern UINT g_FrameIndex;

		extern CpuDescriptorAllocator g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] ;
		
		extern RenderBuffer g_SwapRenderTarget[2]; 
		
		void GraphicInit();
		void GetHardwareAdapter(IDXGIFactory1* pFactory,
			IDXGIAdapter1** ppAdapter,
			bool requestHighPerformanceAdapter);
		enum DescriptorType
		{
			SRV,CBV,UAV
		};

	}
}