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
		CommandContextManager CommandContext::s_context_manager;
		CommandContext::CommandContext(D3D12_COMMAND_LIST_TYPE type)
			:upload_allocator_(AllocateType::kUpload),type_(type)
		{
	
			commandList_ = nullptr;
			//ThrowIfFailed(Graphic::g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_commandAllocator.Get(), pso.pipeline_state_object(), IID_PPV_ARGS(&g_commandList)));
			command_allocator_ = nullptr;
		}
		CommandContext* CommandContext::Begin(const wchar_t* name)
		{


			auto ret = s_context_manager.Allocator(D3D12_COMMAND_LIST_TYPE_DIRECT);
			ret->name = name;
			/*auto ret = new CommandContext(D3D12_COMMAND_LIST_TYPE_DIRECT);
			ret->Initialize();*/
		
	/*		ASSERT_SUCCEEDED(ret->command_allocator_->Reset());
			ASSERT_SUCCEEDED(ret->command_list_->Reset(ret->command_allocator_.Get(), NULL));*/
			return ret;
		}
		void CommandContext::Initialize()
		{
			g_commandManager.CreateNewCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT, &commandList_, &command_allocator_);
		}

		void CommandContext::Reset()
		{
			auto queue = g_commandManager.GetQueue(type_);
			command_allocator_ =  queue.RequireCommandAlloctor();

			commandList_->Reset(command_allocator_,nullptr);

		}

		void CommandContext::End(bool wait_for_complete)
		{
			auto command_queue = g_commandManager.GetQueue(type_);
		//	UINT64 fence = command_queue_.Execute(command_list_.Get());
			UINT64 fence = command_queue.Execute(commandList_.Get());
			//ResourceAllocator->clean();
			command_queue.DiscardCommandAlloctor(fence, command_allocator_);
			command_allocator_ = nullptr;
			
			upload_allocator_.FreeResource(fence);

			if (wait_for_complete)
			{
				command_queue.WaitForFence(fence);
			}
			
		}
		void CommandContext::TransitionBarrier(GpuResource& gpu_resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
		{
	
			D3D12_RESOURCE_BARRIER barrier;
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = gpu_resource.resource();
			barrier.Transition.StateBefore = before;
			barrier.Transition.StateAfter = after;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			commandList_->ResourceBarrier(1, &barrier);
		}
		
		CommandContext* CommandContextManager::Allocator(D3D12_COMMAND_LIST_TYPE type)
		{
			CommandContext* ret = nullptr;
			if (ready_queue_[type].empty())
			{
				ret =  new CommandContext(type);
				context_pool_.emplace_back(ret);
				ret->Initialize();
			}
			else
			{
				ret = ready_queue_[type].front();
				ready_queue_[type].pop();
				ret->Reset();
			}
			return ret;
		}

		void CommandContext::InitializeTexture(GpuResource& dest, UINT subresource_num, D3D12_SUBRESOURCE_DATA data)
		{
			UINT textureUploadBufferSize = GetRequiredIntermediateSize(dest.resource(), 0, 1);

			CommandContext* initContext = CommandContext::Begin();
			AllocateBuffer upload_buffer = initContext->GetTemraryUploadBuffer(textureUploadBufferSize);
			ASSERT_SUCCEEDED(UpdateSubresources<1>(initContext->command_list(), dest.resource(), upload_buffer.Buffer.resource(), 0, 0, subresource_num, &data));
			initContext->TransitionBarrier(dest, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			initContext->End();
		}

		void CommandContextManager::Free(CommandContext* context)
		{
			ready_queue_[context->type()].push(context);
		}



	}
		
}