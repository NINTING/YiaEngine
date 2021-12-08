#pragma once
#include "pch.h"
#include"Graphic.h"
#include"GpuResource.h"
#include"UploadBuffer.h"

#include "ResourceAllocator.h"
namespace YiaEngine:: Graphic
{
	enum BufferCreationFlages
	{
		/// Default flag (Buffer will use aliased memory, buffer will not be cpu accessible until mapBuffer is called)
		BUFFER_CREATION_FLAG_NONE = 0x01,
		/// Buffer will allocate its own memory (COMMITTED resource)
		BUFFER_CREATION_FLAG_OWN_MEMORY_BIT = 0x02,
		/// Buffer will be persistently mapped
		BUFFER_CREATION_FLAG_PERSISTENT_MAP_BIT = 0x04,
		/// Flag to specify not to allocate descriptors for the resource
		BUFFER_CREATION_FLAG_NO_DESCRIPTOR_VIEW_CREATION = 0x8,
	};
	MAKE_ENUM_FLAG(UINT, BufferCreationFlages);
	struct BufferDesc
	{
		/// Size of the buffer (in bytes)
		uint64_t Size;
		/// Alignment
		uint32_t Alignment;
		/// Decides which memory heap buffer will use (default, upload, readback)
		ResourceUsageEnum MemoryUsage;
		/// Creation flags of the buffer
		BufferCreationFlages CreationFlags;
		
		DescriptorTypeFlags DescriptorType;

		const char* name;
		void* InitData;
	};

	class GpuBuffer:public GpuResource
	{
	public:
		void Create(const char* Name, size_t Size, void* data = nullptr);
		void CreateResource(size_t Size, ResourceUsageEnum usage = RESOURCE_USAGE_GPU_ONLY);
	
		void Create(const char* Name, int numElement,int elementSize,const void* initData = nullptr);
		void Create(const char* Name, int numElement, int elementSize, const UploadBuffer& initData,UINT offset = 0);
		void Create(const BufferDesc& desc);
		D3D12_VERTEX_BUFFER_VIEW VertexBufferView(size_t offset, UINT stride, UINT dataSize)const;

		D3D12_INDEX_BUFFER_VIEW IndexBufferView(size_t offset, UINT stride, UINT dataSize)const;
		

//		GpuBuffer* CreateUploadBuffer(int size, BufferCreationFlages creationFlags, DescriptorTypeFlags descriptorTypes);
		static GpuBuffer* CreateBuffer(BufferDesc desc);

		size_t BufferSize() { return bufferSize_; }
	protected:
		//É¾³ýº¯Êý
		void CreateConstBufferView(UINT offset, UINT size);

		void CreateResource(BufferDesc desc);
		void CreateConstBufferView();
		D3D12_RESOURCE_DESC DescriptBuffer(BufferDesc desc);
		//void Create(size_t data_size, size_t stride, void* data);
	protected:
	
		D3D12_RESOURCE_FLAGS resource_flag_;
	//	DescriptorHandle cbvHandle;
		size_t bufferSize_;
		/*UINT elementSize_;
		UINT elementCount_;*/
	};
		
}