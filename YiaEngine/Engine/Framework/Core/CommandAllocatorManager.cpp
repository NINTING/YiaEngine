#pragma once

#include<vector>
#include<queue>


#include"Graphic.h"
#include "CommandAllocatorManager.h"


namespace YiaEngine
{
	namespace Graphic
	{
		void CommandAllocatorManager::Distroy()
		{
			for (int i = 0; i < pool_.size(); i++)
				pool_[i]->Release();
		}
		void CommandAllocatorManager::DiscardAllocator(UINT64 fence, ID3D12CommandAllocator*allocator)
		{
			ready_list.push(std::make_pair(fence, allocator));
		}
		ID3D12CommandAllocator* CommandAllocatorManager::RequestAllocator(UINT64 fence)
		{
			ID3D12CommandAllocator* ret = nullptr;
			if (!ready_list.empty())
			{
				if (ready_list.front().first < fence)
				{
					ret = ready_list.front().second;
					ready_list.pop();
				}
			}
			if (ret == nullptr)
			{
				ASSERT_SUCCEEDED(Graphic::g_device->CreateCommandAllocator(type_, IID_PPV_ARGS(&ret)));
				pool_.push_back(ret);
			}

			return ret;
		}
	}
		
}