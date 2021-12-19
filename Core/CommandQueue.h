#pragma once

#include<queue>


#include"Graphic.h"
#include"GpuResource.h"
#include"ResourceAllocator.h"
#include"CommandAllocatorManager.h"
namespace YiaEngine
{
	namespace Graphic
	{
		struct Fence
		{
			UINT64		 FenceValue;
			ID3D12Fence* FenceHandle;
			HANDLE       FenceEvent;
		};
		struct Semaphore
		{
			Fence value;
		};
		static void CreateFence(Fence*);

		class CommandQueue
		{
		public:
			CommandQueue(D3D12_COMMAND_LIST_TYPE type);
			void Create(ID3D12Device* device);
			UINT64 Execute(ID3D12CommandList*);
			void WaitForFence(UINT64 fence_value);
			ID3D12CommandAllocator* RequireCommandAlloctor();
			void DiscardCommandAlloctor(UINT64 fence, ID3D12CommandAllocator* allocator);
			bool IsFenceComplete(UINT64 fence_value);
			ID3D12CommandQueue* NativeCommandQueue() { return command_queue_.Get(); };
		private:
			D3D12_COMMAND_QUEUE_DESC desc_;
		
			ComPtr<ID3D12CommandQueue>command_queue_;
			D3D12_COMMAND_LIST_TYPE type_;

			ComPtr<ID3D12Fence> fence_;
			HANDLE fence_event_;
			UINT64 complete_fence_value_ = 0;
			UINT64 fence_value_ = 0;

			CommandAllocatorManager command_allocator_manager;

			Fence* fence;
		};
	}
		
}