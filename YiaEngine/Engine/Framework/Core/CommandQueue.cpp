#pragma once

#include<algorithm>
#include <Windows.h>
#include <stdarg.h>
#include<stdio.h>


#include"Graphic.h"
#include"GpuResource.h"
#include"ResourceAllocator.h"
#include"CommandQueue.h"
namespace YiaEngine
{
	namespace Graphic
	{
		CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE type)
			:command_allocator_manager(type),
			command_queue_(nullptr),
			fence_(nullptr), 
			fence_value_((UINT64)type<<56 | 1), 
			complete_fence_value_((UINT64)type << 56),
			fence_event_(nullptr)
		{
		}
		void CommandQueue::Create(ID3D12Device* device)
		{
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.Type = type_;

			ASSERT_SUCCEEDED(Graphic::g_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)));
			ASSERT_SUCCEEDED(Graphic::g_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&command_queue_)));
			fence_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			
			if (fence_event_ == nullptr)
			{
				ASSERT_SUCCEEDED(HRESULT_FROM_WIN32(GetLastError()));
			}
		}

		UINT64 CommandQueue::Execute(ID3D12CommandList* list)
		{

			ASSERT_SUCCEEDED(((ID3D12GraphicsCommandList*)list)->Close());
			//auto hr = ((ID3D12GraphicsCommandList*)list)->Close();
			
		/*	if (FAILED(hr))
			{
				
				YiaEngine::Utility::Print("\nHRESULT failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n");
				YiaEngine::Utility::PrintSubMessage("hr = 0x%08X", hr); 
				YiaEngine::Utility::Print("\n"); 
				__debugbreak();
			}*/

			command_queue_->ExecuteCommandLists(1, &list);
			ASSERT_SUCCEEDED(command_queue_->Signal(fence_.Get(), fence_value_));
			return fence_value_++;
		}
		void CommandQueue::WaitForFence(UINT64 fence_value)
		{
			if (fence_value > complete_fence_value_)
			{
				if (IsFenceComplete(fence_value))
					return;
				if (complete_fence_value_ < fence_value)
				{
					fence_->SetEventOnCompletion(fence_value, fence_event_);
					WaitForSingleObject(fence_event_, INFINITE);
					complete_fence_value_ = fence_value;
				}
			}	
			complete_fence_value_ =max(complete_fence_value_, fence_value);
		}
		ID3D12CommandAllocator* CommandQueue::RequireCommandAlloctor()
		{
			
			return command_allocator_manager.RequestAllocator(fence_->GetCompletedValue());
		}
		void CommandQueue::DiscardCommandAlloctor(UINT64 fence, ID3D12CommandAllocator* allocator)
		{
			command_allocator_manager.DiscardAllocator(fence, allocator);
		}
		bool CommandQueue::IsFenceComplete(UINT64 fence_value)
		{
			
			if (fence_value > complete_fence_value_)
				complete_fence_value_ = max(fence_->GetCompletedValue(), complete_fence_value_);
			return complete_fence_value_ >= fence_value;
			
		}
	}
		
}