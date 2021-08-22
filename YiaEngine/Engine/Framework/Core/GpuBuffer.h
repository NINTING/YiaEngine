#pragma once

#include"Graphic.h"
#include"GpuResource.h"
namespace YiaEngine:: Graphic
{
	class GpuBuffer:GpuResource
	{
		void Create(void* data, size_t data_size, size_t stride, int index);
		
	protected:
		void Create(size_t data_size, size_t stride, void* data);
		D3D12_RESOURCE_DESC DescriptBuffer();
		D3D12_RESOURCE_FLAGS resource_flag_;
		size_t buffer_size_;
	};
		
}