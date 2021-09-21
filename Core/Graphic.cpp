#pragma once
#include"pch.h"
#include"Graphic.h"
#include"CommandManager.h"
#include"DescriptorHeap.h"
#include"RenderBuffer.h"
namespace YiaEngine
{
	namespace Graphic
	{
		Microsoft::WRL::ComPtr<ID3D12Device> g_Device = nullptr;
		DescriptorHeap g_GpuCommonDescriptoHeap;
		CommandManager g_commandManager;
		
		CpuDescriptorAllocator g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] =
		{
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ,
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV ,
			D3D12_DESCRIPTOR_HEAP_TYPE_DSV
		};
		RenderBuffer g_SwapRenderTarget[2];
	}
}