#pragma once
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"D3D12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")


#include<d3d12.h>
#include<d3dcompiler.h>
#include <dxgi1_6.h>
#include<d3d12shader.h>
#include<dxcapi.h>
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
#define SWAP_CHAIN_COUNT 2
		class CommandManager;
		class CpuDescriptorAllocator;
		class DescriptorHeap;
		class RenderBuffer;
		class GpuResourceAllocator;

		extern CommandManager g_commandManager;
		extern ComPtr<ID3D12Device> g_Device;
		extern DescriptorHeap g_GpuCommonDescriptoHeap;
		extern ComPtr< IDXGISwapChain3> g_SwapChain;
		extern UINT g_FrameIndex;
		extern GpuResourceAllocator g_GpuResourceAllocator;
		extern CpuDescriptorAllocator g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] ;
		
		extern RenderBuffer g_SwapRenderTarget[2]; 
		extern RenderBuffer g_SceneFinalTarget;
		
		void GraphicInit();
		void GetHardwareAdapter(IDXGIFactory1* pFactory,
			IDXGIAdapter1** ppAdapter,
			bool requestHighPerformanceAdapter);
		void ResizeScreen(UINT width, UINT Height);

		enum DescriptorTypeFlags
		{
			DESCRIPTOR_TYPE_UNDEFINED = 0,
			DESCRIPTOR_TYPE_SAMPLER = 0x01,
			// SRV Read only texture
			DESCRIPTOR_TYPE_TEXTURE = (DESCRIPTOR_TYPE_SAMPLER << 1),

			DESCRIPTOR_TYPE_UNIFORM_BUFFER = DESCRIPTOR_TYPE_TEXTURE << 1,

			DESCRIPTOR_TYPE_VERTEX_BUFFER = (DESCRIPTOR_TYPE_UNIFORM_BUFFER << 1),
			DESCRIPTOR_TYPE_INDEX_BUFFER = (DESCRIPTOR_TYPE_VERTEX_BUFFER << 1),
			DESCRIPTOR_TYPE_BUFFER = (DESCRIPTOR_TYPE_INDEX_BUFFER << 1),
			DESCRIPTOR_TYPE_RW_BUFFER = (DESCRIPTOR_TYPE_BUFFER << 1),
		};
		MAKE_ENUM_FLAG(UINT, DescriptorTypeFlags);

		enum ResourceUsageEnum
		{
			RESOURCE_USAGE_UNKONE = 0,
			RESOURCE_USAGE_GPU_ONLY = RESOURCE_USAGE_UNKONE + 1,
			RESOURCE_USAGE_CPU_TO_GPU = RESOURCE_USAGE_GPU_ONLY + 1,
			RESOURCE_USAGE_GPU_TO_CPU = RESOURCE_USAGE_CPU_TO_GPU + 1,
			RESOURCE_USAGE_CPU_ONLY = RESOURCE_USAGE_GPU_TO_CPU + 1,
		};
	}
}