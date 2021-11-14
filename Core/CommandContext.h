#pragma once

#include<string>

#include"Graphic.h"
#include"GpuResource.h"
#include"ResourceAllocator.h"
#include"DescriptorHeap.h"
#include"CommandQueue.h"
#include"PipelineStateObject.h"
#include"UploadBuffer.h"
#include"GpuBuffer.h"
#include"Renderer/Image.h"

namespace YiaEngine
{
	namespace Graphic
	{
		
		enum CommandType
		{
			kDirect
		};
		class CommandContext;

		class CommandContextManager
		{
		public:
			CommandContextManager() = default;
	
			CommandContext* Allocator(D3D12_COMMAND_LIST_TYPE type);
			void Free(CommandContext* context);
		private:
			std::vector<CommandContext*> context_pool_;
			std::queue<CommandContext*> ready_queue_[4];
		};
		
		struct GpuDescriptorTable
		{
			D3D12_CPU_DESCRIPTOR_HANDLE* StartHandle;
			UINT TableSize;
			UINT BaseOffset;
		};

		//CommandContext是一个最小的GPU任务执行单元
		class CommandContext
		{
			friend CommandContextManager;
		public:
			static CommandContext* Begin(const wchar_t* Name = L"");
			void End(bool wait_for_complete = true);

		//	CommandContext();

			AllocateBuffer GetAllocateUploadBuffer(UINT64 size_byte, int aligment = 256) {
				
				return upload_allocator_.Allocate(size_byte,aligment);
			}
			/*AllocateBuffer GetTemplateUploadBuffer(size_t size_byte) {

				return upload_allocator_.Allocate(size_byte);
			}*/
			void TransitionBarrier(GpuResource& NativeResource, D3D12_RESOURCE_STATES source, D3D12_RESOURCE_STATES dest);

			void TransitionBarrier(GpuResource& gpu_resource, D3D12_RESOURCE_STATES destStates);

			/// <summary>
			/// At most one CBV/SRV/UAV combined heap and one Sampler heap can be bound at any one time. 
			/// These heaps are shared between both the graphics and compute pipelines
			/// (described in their PSOs).
			/// </summary>
			/// <param name="type"></param>
			/// <param name="heap"></param>
			void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12DescriptorHeap* heap);
			void SetPipelineState(const PipelineStateObject& pso);
			void CopyBuffer(GpuResource& dest,size_t destOffset,GpuResource& src,size_t srcOffset,size_t numBytes);

		public:
			//static void InitializeTexture(Texture& dest, UINT subresource_num, D3D12_SUBRESOURCE_DATA data);
			static void InitializeTexture(GpuResource& dest, ImageData& image);
			static void UpdateBufferData(GpuBuffer& dest, const void* initData);
			static void UpdateBufferData(GpuResource& dest,UINT bufferSize,const void* initData);
			static void UpdateBufferData(GpuBuffer& dest, const UploadBuffer& uploadBuffer, UINT srcOffset = 0, UINT destOffset = 0, size_t numByte = -1);
			
		public:

			ID3D12GraphicsCommandList* NativeCommandList() { return commandList_.Get(); };
			D3D12_COMMAND_LIST_TYPE type() { return type_; };
		private:
			CommandContext(D3D12_COMMAND_LIST_TYPE type);
			void Initialize();
			void Reset();
		protected:
			//CommandQueue command_queue_;

			static CommandContextManager s_context_manager;
			ComPtr<ID3D12GraphicsCommandList> commandList_;
			ID3D12CommandAllocator* command_allocator_;
			ResourceAllocator upload_allocator_;
			D3D12_COMMAND_LIST_TYPE type_;
			std::wstring Name;

			GpuDescriptorAllocator viewDescriptorAllocator;
			GpuDescriptorAllocator sampleDescriptorAllocator;
		protected:
			ID3D12PipelineState* pipelineState_;
		protected:
			static const int kMaxDescriptorNum = 256;
			static const int kMaxDescriptorTableNum = 16;
			GpuDescriptorTable tableCache_[kMaxDescriptorTableNum];
			D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorPool_[kMaxDescriptorNum];
			UINT tableSize_ = 0;
			ID3D12DescriptorHeap* currentDescriptorHeaps[2];
		};
	}
		
}