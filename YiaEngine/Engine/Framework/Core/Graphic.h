#pragma once
#include<d3d12.h>
#include<d3dx12.h>
#include <wrl/client.h>

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

		extern CommandManager g_commandManager;
		extern ComPtr<ID3D12Device> g_Device;
		extern DescriptorHeap g_GpuCommonDescriptoHeap;
		extern CpuDescriptorAllocator g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] ;
		enum DescriptorType
		{
			SRV,CBV,UAV
		};
	}
}