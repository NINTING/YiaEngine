#pragma once

#include"Graphic.h"
#include"GpuResource.h"
#include"ResourceAllocator.h"
#include "CommandContext.h"
#include"CommandManager.h"
namespace YiaEngine
{
	namespace Graphic
	{
		CommandContext::CommandContext(D3D12_COMMAND_LIST_TYPE type)
			:upload_allocator_(AllocateType::kUpload),type_(type)
		{
	
			command_list_ = nullptr;
			//ThrowIfFailed(Graphic::g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_commandAllocator.Get(), pso.pipeline_state_object(), IID_PPV_ARGS(&g_commandList)));
			command_allocator_ = nullptr;
		}
		CommandContext* CommandContext::Begin()
		{
			auto ret = new CommandContext(D3D12_COMMAND_LIST_TYPE_DIRECT);
			ret->Initialize();
		
	/*		ASSERT_SUCCEEDED(ret->command_allocator_->Reset());
			ASSERT_SUCCEEDED(ret->command_list_->Reset(ret->command_allocator_.Get(), NULL));*/
			return ret;
		}
		void CommandContext::Initialize()
		{
		//	command_allocator_ = command_queue_.RequireCommandAlloctor();
		//	ASSERT_SUCCEEDED(Graphic::g_device->CreateCommandList(1, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator_.Get(), nullptr, IID_PPV_ARGS(&command_list_)));
			g_commandManager.CreateNewCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT, &command_list_, &command_allocator_);
		//	ASSERT_SUCCEEDED(command_list_->Close());
		
		}

		void CommandContext::End(bool wait_for_complete)
		{
			auto command_queue = g_commandManager.GetQueue(type_);
		//	UINT64 fence = command_queue_.Execute(command_list_.Get());
			UINT64 fence = command_queue.Execute(command_list_.Get());
			//ResourceAllocator->clean();
			command_queue.DiscardCommandAlloctor(fence, command_allocator_.Get());
			command_allocator_ = nullptr;

			if (wait_for_complete)
			{
				command_queue.WaitForFence(fence);
			}
			
		}
	}
		
}