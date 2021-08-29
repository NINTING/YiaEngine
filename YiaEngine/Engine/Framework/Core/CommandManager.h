#pragma once

#include"Graphic.h"
#include"GpuResource.h"
#include"ResourceAllocator.h"
#include"CommandQueue.h"
namespace YiaEngine
{
	namespace Graphic
	{
		class CommandManager
		{
		
		
		public:
			CommandManager():
				graphic_command_queue_(D3D12_COMMAND_LIST_TYPE_DIRECT) {};
			void Create(ID3D12Device* device) 
			{
				graphic_command_queue_.Create(device);

			}
			CommandQueue& graphic_command_queue() {
				return graphic_command_queue_;
			};
			UINT64 ExecuteGraphicCommandList(ID3D12CommandList* list)
			{
				return graphic_command_queue_.Execute(list);
					}
			CommandQueue& GetQueue(D3D12_COMMAND_LIST_TYPE type)
			{
				switch (type)
				{
				case D3D12_COMMAND_LIST_TYPE_DIRECT:
					return graphic_command_queue_;
				case D3D12_COMMAND_LIST_TYPE_BUNDLE:
					break;
				case D3D12_COMMAND_LIST_TYPE_COMPUTE:
					break;
				case D3D12_COMMAND_LIST_TYPE_COPY:
					break;
				default:
					break;
				}
			}
			void CreateNewCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList** out_list, ID3D12CommandAllocator** out_allocator,const wchar_t*name = L"")
			{
			
				switch (type)
				{
				case D3D12_COMMAND_LIST_TYPE_DIRECT:
					*out_allocator = graphic_command_queue_.RequireCommandAlloctor();
					break;
				case D3D12_COMMAND_LIST_TYPE_BUNDLE:
					break;
				case D3D12_COMMAND_LIST_TYPE_COMPUTE:
					break;
				case D3D12_COMMAND_LIST_TYPE_COPY:
					break;
				default:
					break;
				}
				g_device->CreateCommandList(1, type, *out_allocator, nullptr, IID_PPV_ARGS(out_list));
				(*out_list)->SetName(name);
			
			}
		private:
			CommandQueue graphic_command_queue_;
		};
	}
		
}