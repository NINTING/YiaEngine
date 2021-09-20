#pragma once

#include<vector>
#include<queue>


#include"Graphic.h"


namespace YiaEngine
{
	namespace Graphic
	{
		class CommandAllocatorManager
		{
		public:
			CommandAllocatorManager(D3D12_COMMAND_LIST_TYPE type) :type_(type) {}
			
			~CommandAllocatorManager()
			{
				Distroy();
			}
			void Distroy();
			void DiscardAllocator(UINT64 fence, ID3D12CommandAllocator*allocator);
			ID3D12CommandAllocator* RequestAllocator(UINT64 fence);
		private:
			D3D12_COMMAND_LIST_TYPE type_;
			std::queue<std::pair<UINT64, ID3D12CommandAllocator*>>ready_list;
			std::vector<ID3D12CommandAllocator*>pool_;
		};
	}
		
}