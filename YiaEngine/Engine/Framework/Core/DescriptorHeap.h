#pragma once

#include<d3d12.h>
#include<wrl/client.h>
#include<stdint.h>
#include<vector>
#include <cassert>

/*
*	shader-visible ���������ѻὨ����WRITE_COMBINE memory , GPU local memory.��ȡ�ٶȻ�ǳ��� 
*	�����Դ��Ҫ��CPU��ʹ����Ҫ�󶨵�GPU�У���Ҫ�ȴ洢��non-shader-visible��Ȼ���ڿ�����shader-visible heap
*/
namespace YiaEngine::Graphic
{
	
	class DescriptorHandle
	{
	public:
		DescriptorHandle() { 
			cpu_handle_.ptr = -1; 
			gpu_handle_.ptr = -1;
		};
		DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle) {
			cpu_handle_ = cpu_handle;
			gpu_handle_ = gpu_handle;
		};
		bool IsValide() { return cpu_handle_.ptr != -1; }
		operator D3D12_CPU_DESCRIPTOR_HANDLE()const { return cpu_handle_; };
		operator D3D12_GPU_DESCRIPTOR_HANDLE()const { return gpu_handle_; };
		D3D12_CPU_DESCRIPTOR_HANDLE* GetCpuAddress() { return &cpu_handle_; };
		D3D12_GPU_DESCRIPTOR_HANDLE* GetGpuAddress() { return &gpu_handle_; };
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
			if(gpu_handle_.ptr!=-1)
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
		DescriptorHeap(const wchar_t* name,D3D12_DESCRIPTOR_HEAP_DESC desc);
		DescriptorHeap() = default;
	
		void Create(const wchar_t* name, D3D12_DESCRIPTOR_HEAP_DESC desc);
		void CreateShaderVisibleType(const wchar_t* name, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT32 count);
		void CreateNonShaderVisibleType(const wchar_t* name, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT32 count);
	//	virtual void Create(const wchar_t* name, D3D12_DESCRIPTOR_HEAP_TYPE type,UINT32 count, ID3D12Device* device); 
		
		DescriptorHandle Alloc(uint32_t count = 1);
		bool HasFreeSpace(int count) { return free_descriptor_num_ >= count; }
	public:
		ID3D12DescriptorHeap* heap_ptr() { return heap_.Get(); }
		DescriptorHandle operator[](uint32_t index) { return first_handle_ + index * descriptor_size_; }
	protected:
		void InitHeap();

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;
		D3D12_DESCRIPTOR_HEAP_DESC desc_;
		UINT32 descriptor_size_;
		uint32_t free_descriptor_num_;
		DescriptorHandle first_handle_;
		DescriptorHandle head_free_handle_;
	};

	//���޵�no-shader-visible������������е,��������Ҫ��������߱��档
//��������Ҫ��CPU����Դ���ж�ȡ���������
	class DescriptorAllocator
	{
	public:
		DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type)
			:type_(type) {}
		DescriptorHandle Alloc(uint32_t count);
		void AllocHeap(D3D12_DESCRIPTOR_HEAP_TYPE type);

		
	private:
		static const int descriptor_count_perframe_ = 256;
		std::vector<DescriptorHeap> heaps_;
		D3D12_DESCRIPTOR_HEAP_TYPE type_;
	};

	
}