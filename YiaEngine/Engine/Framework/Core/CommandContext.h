#pragma once

#include<string>

#include"Graphic.h"
#include"GpuResource.h"
#include"ResourceAllocator.h"
#include"DescriptorHeap.h"
#include"CommandQueue.h"
#include"PipelineStateObject.h"
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
			static CommandContext* Begin(const wchar_t* name = L"");
			void End(bool wait_for_complete = true);

			AllocateBuffer GetTemraryUploadBuffer(size_t size_byte) {
				return upload_allocator_.Allocate(size_byte);
			}
			void TransitionBarrier(GpuResource& resource, D3D12_RESOURCE_STATES source, D3D12_RESOURCE_STATES dest);

			/// <summary>
			/// At most one CBV/SRV/UAV combined heap and one Sampler heap can be bound at any one time. 
			/// These heaps are shared between both the graphics and compute pipelines
			/// (described in their PSOs).
			/// </summary>
			/// <param name="type"></param>
			/// <param name="heap"></param>
			void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12DescriptorHeap* heap);
			void SetPipelineState(const PipelineStateObject& pso);

		public:
			static void InitializeTexture(GpuResource&dest,UINT subresource_num, D3D12_SUBRESOURCE_DATA data);
			

		public:

			ID3D12GraphicsCommandList* command_list() { return commandList_.Get(); };
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
			std::wstring name;

			GpuDescriptorAllocator viewDescriptorAllocator;
		protected:
			ID3D12PipelineState* pipelineState_;
		protected:
			static const int kMaxDescriptorNum = 256;
			static const int kMaxDescriptorTableNum = 16;
			GpuDescriptorTable tableCache_[kMaxDescriptorTableNum];
			D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorPool_[kMaxDescriptorNum];
			UINT tableSize_ = 0;
			ID3D12DescriptorHeap* currentDesdcriptorHeaps[2];
		};
	}
		
}