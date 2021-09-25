#pragma once

#include"Graphic.h"
#include"GpuResource.h"
namespace YiaEngine:: Graphic
{
	class GpuBuffer:public GpuResource
	{
	public:
		
		void Create(size_t data_size, size_t stride, void* data = nullptr);
		D3D12_VERTEX_BUFFER_VIEW VertexBufferView(size_t offset, UINT stride, UINT dataSize);

		D3D12_INDEX_BUFFER_VIEW IndexBufferView(size_t offset, UINT stride, UINT dataSize);
		
	protected:
		//void Create(size_t data_size, size_t stride, void* data);
		D3D12_RESOURCE_DESC DescriptBuffer();
		D3D12_RESOURCE_FLAGS resource_flag_;	
		size_t buffer_size_;
	};
		
}