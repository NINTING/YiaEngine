#pragma once

#include"Graphic.h"
#include"GpuResource.h"
#include"ResourceAllocator.h"
#include "CommandContext.h"
#include"CommandManager.h"
#include"PipelineStateObject.h"
namespace YiaEngine
{
	namespace Graphic
	{
		CommandContextManager CommandContext::s_context_manager;
	
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
			memset(tableCache_, 0, sizeof(GpuDescriptorTable) * kMaxDescriptorTableNum);
		}

		void CommandContext::Reset()
		{
			auto& queue = g_commandManager.GetQueue(type_);
			command_allocator_ =  queue.RequireCommandAlloctor();

			commandList_->Reset(command_allocator_,nullptr);
			memset(tableCache_, 0, sizeof(GpuDescriptorTable) * kMaxDescriptorTableNum);
			pipelineState_ = nullptr;
			currentDescriptorHeaps[0] = nullptr;
			currentDescriptorHeaps[1] = nullptr;
		}

		void CommandContext::End(bool wait_for_complete)
		{
			auto& command_queue = g_commandManager.GetQueue(type_);
		//	UINT64 fence = command_queue_.Execute(command_list_.Get());
			UINT64 fence = command_queue.Execute(commandList_.Get());
			//ResourceAllocator->clean();
			command_queue.DiscardCommandAlloctor(fence, command_allocator_);
			command_allocator_ = nullptr;
			
			upload_allocator_.FreeResource(fence);
			viewDescriptorAllocator.Clean(fence);
			sampleDescriptorAllocator.Clean(fence);

			if (wait_for_complete)
			{
				command_queue.WaitForFence(fence);
			}
			s_context_manager.Free(this);
		}
		CommandContext::CommandContext(D3D12_COMMAND_LIST_TYPE listType):
			type_(listType),
			upload_allocator_(AllocateType::kUpload),
			viewDescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),
			sampleDescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
			{
				command_allocator_ = nullptr;
				commandList_ = nullptr;
			}
		void CommandContext::TransitionBarrier(GpuResource& gpu_resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
		{
	
			D3D12_RESOURCE_BARRIER barrier;
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = gpu_resource.RawResource();
			barrier.Transition.StateBefore = before;
			barrier.Transition.StateAfter = after;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			commandList_->ResourceBarrier(1, &barrier);
			
		}
		void CommandContext::TransitionBarrier(GpuResource& gpu_resource, D3D12_RESOURCE_STATES destStates)
		{

			TransitionBarrier(gpu_resource, gpu_resource.Usage(), destStates);
			gpu_resource.Usage(destStates);
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
			UINT textureUploadBufferSize = GetRequiredIntermediateSize(dest.RawResource(), 0, 1);

			CommandContext* initContext = CommandContext::Begin();
			AllocateBuffer upload_buffer = initContext->GetAllocateUploadBuffer(textureUploadBufferSize);
			ASSERT_SUCCEEDED(UpdateSubresources<1>(initContext->RawCommandList(), dest.RawResource(), upload_buffer.Buffer.RawResource(), 0, 0, subresource_num, &data));
			initContext->TransitionBarrier(dest, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			initContext->End();
		}

		void CommandContext::InitializeBuffer(GpuResource& dest, UINT bufferSize, void* initData)
		{

			CommandContext* initContext = CommandContext::Begin();
			AllocateBuffer upload_buffer = initContext->GetAllocateUploadBuffer(bufferSize);
			D3D12_SUBRESOURCE_DATA data;
			data.pData = initData;
			data.RowPitch = bufferSize;
			
			initContext->TransitionBarrier(dest, D3D12_RESOURCE_STATE_COPY_DEST);
			ASSERT_SUCCEEDED(UpdateSubresources<1>(initContext->RawCommandList(), dest.RawResource(), upload_buffer.Buffer.RawResource(), 0, 0, 1, &data));
			initContext->TransitionBarrier(dest,D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			initContext->End();

		}

		void CommandContextManager::Free(CommandContext* context)
		{
			ready_queue_[context->type()].push(context);
		}


		void CommandContext::SetPipelineState(const PipelineStateObject& pso)
		{
			auto rawPso = pso.RawPipelineStateObject();
			if (rawPso == pipelineState_)
				return;
			commandList_->SetPipelineState(rawPso);
			pipelineState_ = rawPso;
		}
		void CommandContext::SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12DescriptorHeap * heap)
		{
			assert(type < D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
			if (currentDescriptorHeaps[(int)type] != heap)
			{
				currentDescriptorHeaps[(int)type] = heap;
				
				commandList_->SetDescriptorHeaps(1, &heap);
			}
		}
	}
		
}