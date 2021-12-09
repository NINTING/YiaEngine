#pragma once

#include<d3d12.h>
#include<wrl/client.h>
#include<stdint.h>
#include<vector>
#include <cassert>
#include<queue>


#include"Graphic.h"

/*
*	shader-visible ���������ѻὨ����WRITE_COMBINE memory , GPU local memory.��ȡ�ٶȻ�ǳ��� 
*	�����Դ��Ҫ��CPU��ʹ����Ҫ�󶨵�GPU�У���Ҫ�ȴ洢��non-shader-visible��Ȼ���ڿ�����shader-visible heap
*/

/*
* ����������Ҫ��Ϊ�˾����ܵİ���һ������Ⱦ֡����Ҫ����������С��
* ����Texture��Ҫ�ڲ�ͬ��״̬�л�(e.g. cpu access,shader visible),����ҪΪ��ͬ��״̬��������Ӧ��������
* �������ѿ���ʹ�ü򵥵�Ѱַ����ʹ��������
*/

//https://docs.microsoft.com/en-us/windows/win32/direct3d12/descriptor-heaps-overview
namespace YiaEngine
{
	namespace Graphic
	{
		class DescriptorHandle
		{
		public:
			DescriptorHandle() {
				cpu_handle_.ptr = ADDRESS_NULL;
				gpu_handle_.ptr = ADDRESS_NULL;
			};
			DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle) {
				cpu_handle_ = cpu_handle;
				gpu_handle_ = gpu_handle;
			};
			bool IsValide() { return cpu_handle_.ptr != ADDRESS_NULL; }
			operator D3D12_CPU_DESCRIPTOR_HANDLE()const { return cpu_handle_; };
			operator D3D12_GPU_DESCRIPTOR_HANDLE()const { return gpu_handle_; };
			const D3D12_CPU_DESCRIPTOR_HANDLE* GetCpuHandleAddress()const { return &cpu_handle_; };
			const D3D12_GPU_DESCRIPTOR_HANDLE* GetGpuHandleAddress()const { return &gpu_handle_; };
			SIZE_T CpuDescriptorValuePtr()const { return cpu_handle_.ptr; };
			SIZE_T GpuDescriptorValuePtr()const { return gpu_handle_.ptr; };
			DescriptorHandle operator +(int offset)const
			{
				DescriptorHandle ret = *this;
				ret += offset;
				return ret;
			}
			void operator +=(int offset)
			{
				assert(cpu_handle_.ptr != -1);
				cpu_handle_.ptr += offset;
				if (gpu_handle_.ptr != 0)
					gpu_handle_.ptr += offset;
			}
		private:
			D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
			D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
		};


		//������������
		class DescriptorHeap
		{
		public:
			DescriptorHeap(const wchar_t* Name, D3D12_DESCRIPTOR_HEAP_DESC desc);
			DescriptorHeap() = default;

			void Create(const wchar_t* Name, D3D12_DESCRIPTOR_HEAP_DESC desc);
			void CreateShaderVisibleType(const wchar_t* Name, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT32 count);
			void CreateNonShaderVisibleType(const wchar_t* Name, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT32 count);
			//	virtual void Create(const wchar_t* name, D3D12_DESCRIPTOR_HEAP_TYPE type,UINT32 count, ID3D12Device* device); 
			DescriptorHandle Alloc(uint32_t count = 1);
			void Reset();
			bool HasFreeSpace(size_t count) { return free_descriptor_num_ >= count; }
		public:
			ID3D12DescriptorHeap* NativeHeap() const { return heap_.Get(); }
			DescriptorHandle operator[](uint32_t index) { return first_handle_ + index * descriptor_size_; }
		protected:
			void InitHeap();

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;
			D3D12_DESCRIPTOR_HEAP_DESC desc_;
			UINT32 descriptor_size_;
			uint32_t free_descriptor_num_;
			DescriptorHandle first_handle_;
			DescriptorHandle head_free_handle_;
			std::wstring name_;
		};

		//���޵�no-shader-visible������������е,��������Ҫ��������߱��档
		//��������Ҫ��CPU����Դ���ж�ȡ���������,��Cpu�����ݴ档
		// ����Ҫ��ɫ���ɼ�ʱ����Ҫ������DynamicDescriptorAllocator
		//https://docs.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12device-copydescriptors
		class CpuDescriptorAllocator
		{
		public:
			CpuDescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type)
				:type_(type) {}
			DescriptorHandle Alloc(uint32_t count);
			void AllocHeap(D3D12_DESCRIPTOR_HEAP_TYPE type);
			static DescriptorHandle AllocateDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t count = 1);


		private:
			static const int descriptor_count_perframe_ = 256;
			std::vector<DescriptorHeap> heaps_;

			D3D12_DESCRIPTOR_HEAP_TYPE type_;

		};
		class RootSignature;

		/// <summary>
		/// ShaderVisible heap �����ڴ�����(��Լ96MB)
		/// https://docs.microsoft.com/en-us/windows/win32/direct3d12/shader-visible-descriptor-heaps
		/// GPU handles are not for immediate use��they identify locations from a command list,
		///  for use at GPU execution time. They must be preserved until any command lists referencing 
		/// them have executed entirely.
		/// </summary>
		class GpuDescriptorAllocator
		{
		public:
			GpuDescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type) :heapType_(type) {}

			void ParseRootSignature(const RootSignature& rootSignature);

			DescriptorHandle Alloc(UINT count = 1);
			DescriptorHandle CopyToGpuDescriptor(UINT count, const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle[]);
			void CopyToGpuDescriptor(UINT count, const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle[], const D3D12_CPU_DESCRIPTOR_HANDLE destHandle[]);
			void RetireCurrentHeap();

			bool HasSpace(UINT size) { return currentHeap_ && currentHeap_->HasFreeSpace(size); };
			DescriptorHeap& CurrentUseHeap();// { return *currentHeap; }
			UINT ViewDescriptorIncrementSize() { return Graphic::g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV); };
			void Clean(UINT64 fence);
		private:
			static void DiscardUseHeaps(D3D12_DESCRIPTOR_HEAP_TYPE type, uint64_t fence, const std::vector<DescriptorHeap*>& useHeap);
			static DescriptorHeap* requestNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE type);
		private:
			std::vector<DescriptorHeap> viewHeapPool_;

			std::vector<DescriptorHeap*> useHeaps_;
			static std::vector<DescriptorHeap> s_heapPool_[2];
			static std::queue<std::pair<uint64_t, DescriptorHeap*>> s_retireHeaps[2];
			static std::queue<DescriptorHeap*> s_avalibleHeaps[2];
			DescriptorHeap* currentHeap_ = nullptr;
			D3D12_DESCRIPTOR_HEAP_TYPE heapType_;
		};
	}

}