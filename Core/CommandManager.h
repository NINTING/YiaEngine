#pragma once
#include"pch.h"
#include"Graphic.h"
#include"GpuResource.h"
#include"ResourceAllocator.h"
#include"CommandQueue.h"
namespace YiaEngine
{
	namespace Graphic
	{
		/*
		* CommandManager 是分配Command_Queue和CommandAllcator的管理器
		* CommandQueue 每种类型全局唯一                                                                     
		*/
		class CommandManager
		{
		
		
		public:
			CommandManager():
				graphicCommandQueue_(D3D12_COMMAND_LIST_TYPE_DIRECT), bundleCommandQueue_(D3D12_COMMAND_LIST_TYPE_BUNDLE), 
				computerCommandQueue_(D3D12_COMMAND_LIST_TYPE_COMPUTE),copyCommandQueue_(D3D12_COMMAND_LIST_TYPE_COPY) {};
			void Create(ID3D12Device* device) 
			{
				graphicCommandQueue_.Create(device);
				copyCommandQueue_.Create(device);
			}
			CommandQueue& graphic_command_queue() {
				return graphicCommandQueue_;
			};
			CommandQueue& copy_command_queue() {
				return copyCommandQueue_;
			};

			UINT64 ExecuteGraphicCommandList(ID3D12CommandList* list)
			{
				return graphicCommandQueue_.Execute(list);
					}
			CommandQueue& GetQueue(D3D12_COMMAND_LIST_TYPE type)
			{

				switch (type)
				{
				case D3D12_COMMAND_LIST_TYPE_DIRECT:
					return graphicCommandQueue_;
				case D3D12_COMMAND_LIST_TYPE_BUNDLE:
					YIA_GRAPHIC_ERR("Not Impelemnt Bundle Queue");
					return bundleCommandQueue_;
				case D3D12_COMMAND_LIST_TYPE_COMPUTE:
					YIA_GRAPHIC_ERR("Not Impelemnt Computer Queue");
					return computerCommandQueue_;
				case D3D12_COMMAND_LIST_TYPE_COPY:
					return copyCommandQueue_;
					break;
				default:
					YIA_GRAPHIC_ERR("Use Unkown Queue");
					return graphicCommandQueue_;
					break;
				}
				
			}
			void CreateNewCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList** out_list, ID3D12CommandAllocator** out_allocator,const wchar_t*Name = L"")
			{
			
				switch (type)
				{
				case D3D12_COMMAND_LIST_TYPE_DIRECT:
					*out_allocator = graphicCommandQueue_.RequireCommandAlloctor();
					break;
				case D3D12_COMMAND_LIST_TYPE_BUNDLE:
					break;
				case D3D12_COMMAND_LIST_TYPE_COMPUTE:
					break;
				case D3D12_COMMAND_LIST_TYPE_COPY:
					*out_allocator = copyCommandQueue_.RequireCommandAlloctor();
					break;
				default:
					break;
				}
				g_Device->CreateCommandList(1, type, *out_allocator, nullptr, IID_PPV_ARGS(out_list));
				(*out_list)->SetName(Name);
			
			}
			bool IsComplete(UINT64 fence_value)
			{
				return GetQueue((D3D12_COMMAND_LIST_TYPE)(fence_value >> 56)).IsFenceComplete(fence_value);
			}
		private:
			CommandQueue graphicCommandQueue_;
			CommandQueue bundleCommandQueue_;
			CommandQueue computerCommandQueue_;
			CommandQueue copyCommandQueue_;
		};
	}
		
}