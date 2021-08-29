#pragma once

#include"Graphic.h"
#include"GpuResource.h"
#include"ResourceAllocator.h"
#include"CommandQueue.h"
namespace YiaEngine
{
	namespace Graphic
	{
		enum CommandType
		{
			kDirect
		};
		class CommandContextManager
		{

		};

		//CommandContext��һ����С��GPU����ִ�е�Ԫ
		class CommandContext
		{
		public:
	
			static CommandContext* Begin();
			void End(bool wait_for_complete = true);

			AllocateBuffer GetTemraryUploadBuffer(size_t size_byte) {
				return upload_allocator_.Allocate(size_byte);
			}

			ID3D12GraphicsCommandList* command_list() { return command_list_.Get(); };
		private:
			CommandContext(D3D12_COMMAND_LIST_TYPE type);
			void Initialize();
		private:
			//CommandQueue command_queue_;


			ComPtr<ID3D12GraphicsCommandList> command_list_;
			ComPtr <ID3D12CommandAllocator> command_allocator_;
			ResourceAllocator upload_allocator_;
			D3D12_COMMAND_LIST_TYPE type_;
			//CommandType type_;
		};
	}
		
}