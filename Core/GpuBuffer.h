#pragma once

#include"Graphic.h"
#include"GpuResource.h"
#include"UploadBuffer.h"
namespace YiaEngine:: Graphic
{
	class GpuBuffer:public GpuResource
	{
	public:
		
		void Create(size_t Size, size_t stride, void* data = nullptr);
		void Create(const wchar_t* Name, int numElement,int elementSize,const void* initData);
		void Create(const wchar_t* Name, int numElement, int elementSize, const UploadBuffer& initData,UINT offset = 0);
		D3D12_VERTEX_BUFFER_VIEW VertexBufferView(size_t offset, UINT stride, UINT dataSize)const;

		D3D12_INDEX_BUFFER_VIEW IndexBufferView(size_t offset, UINT stride, UINT dataSize)const;
		size_t BufferSize() { return bufferSize_; }
	protected:
		//void Create(size_t data_size, size_t stride, void* data);
		D3D12_RESOURCE_DESC DescriptBuffer();
		D3D12_RESOURCE_FLAGS resource_flag_;
		size_t bufferSize_;
		UINT elementSize_;
		UINT elementCount_;
	};
		
}