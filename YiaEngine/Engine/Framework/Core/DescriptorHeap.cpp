#include "DescriptorHeap.h"
#pragma once


#include<string>

#include"Utility.h"
#include"DescriptorHeap.h"

#include"Core/Graphic.h"
#include"RootSignature.h"
namespace YiaEngine::Graphic
{
	DescriptorHeap::DescriptorHeap(const wchar_t* name,D3D12_DESCRIPTOR_HEAP_DESC desc)
	{
		desc_ = desc;
		
		auto hr = Graphic::g_Device->CreateDescriptorHeap(&desc_, IID_PPV_ARGS(&heap_));
		heap_->SetName(name);

		InitHeap();
	}
	void DescriptorHeap::Create(const wchar_t* name, D3D12_DESCRIPTOR_HEAP_DESC desc)
	{
		desc_ = desc;
		auto hr = Graphic::g_Device->CreateDescriptorHeap(&desc_, IID_PPV_ARGS(&heap_));
		heap_->SetName(name);
		InitHeap();
	}
	void DescriptorHeap::CreateShaderVisibleType(const wchar_t* name, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT32 count)
	{
		desc_.Type = type;
		desc_.NumDescriptors = count;
		desc_.NodeMask = 0;
		desc_.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		Create(name, desc_);
	}
	void DescriptorHeap::CreateNonShaderVisibleType(const wchar_t* name, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT32 count)
	{
		desc_.Type = type;
		desc_.NumDescriptors = count;
		desc_.NodeMask = 0;
		desc_.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		Create(name, desc_);
	}
	//void DescriptorHeap::Create(const wchar_t* name, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT32 count, ID3D12Device* device)
	//{
	//	desc_.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//	desc_.NumDescriptors = count;
	//	desc_.Type = type;
	//	desc_.NodeMask = 0;
	//	auto hr = device->CreateDescriptorHeap(&desc_, IID_PPV_ARGS(&heap_));
	//	heap_->SetName(name);
	//	
	//	InitHeap(device);
	//}

	DescriptorHandle DescriptorHeap::Alloc(uint32_t count)
	{
		
		DescriptorHandle ret;
		if (HasFreeSpace(count)) {
			ret = head_free_handle_;
			head_free_handle_ += count * descriptor_size_;
			free_descriptor_num_ -= count;
		}
		return ret;
	}

	void DescriptorHeap::InitHeap()
	{
		descriptor_size_ = Graphic::g_Device->GetDescriptorHandleIncrementSize(desc_.Type);
		free_descriptor_num_ = desc_.NumDescriptors;
		first_handle_ = DescriptorHandle(heap_->GetCPUDescriptorHandleForHeapStart(), heap_->GetGPUDescriptorHandleForHeapStart());
		head_free_handle_ = first_handle_;
	}
	
	DescriptorHandle CpuDescriptorAllocator::Alloc(uint32_t count = 1)
	{
		DescriptorHandle ret;
		for (auto it = heaps_.begin(); it != heaps_.end(); it++)
		{
			if (it->HasFreeSpace(count))
			{
				ret = it->Alloc(count);
			}
		}
		if (!ret.IsValide())
		{
			AllocHeap(type_);
			ret = heaps_.back().Alloc(count);
		}
		return ret;
	}

	void CpuDescriptorAllocator::AllocHeap(D3D12_DESCRIPTOR_HEAP_TYPE type)
	{
		DescriptorHeap heap;
		wchar_t name[25];
		wsprintf(name, L"Cpu_Alloc_%d", heaps_.size()+1);
		heap.CreateNonShaderVisibleType(name, type,descriptor_count_perframe_);
		heaps_.push_back(heap);
	}

	DescriptorHandle CpuDescriptorAllocator::AllocateDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t count)
	{
		static CpuDescriptorAllocator s_descriptor[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] = {
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV ,
		D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ,
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV  ,
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		};
		return s_descriptor[type].Alloc(count);
	}


	// @param sda
	void GpuDescriptorAllocator::ParseRootSignature(const RootSignature& rootSignature)
	{
		for (size_t i = 0; i < rootSignature.ParamenterCount(); i++)
		{
			if (rootSignature[i].IsDescriptorTable())
			{
				
			}
		}
	}
	DescriptorHandle GpuDescriptorAllocator::Alloc(UINT count)
	{
		if (currentHeap == nullptr)
		{
			DescriptorHeap heap;
			heap.CreateShaderVisibleType(L"GpuViewHeap",D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1024);
			viewHeapPool_.emplace_back(heap);
			currentHeap = &viewHeapPool_.back();
		}
		return currentHeap->Alloc(count);
	}

	/// <summary>
	/// 按照DescriptorTable为单位 拷贝至shader visible descriptor heap
	/// </summary>
	/// <param name="count"></param>
	/// <param name="cpuHandle"></param>
	DescriptorHandle GpuDescriptorAllocator::CopyToGpuDescriptor(int count,const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle[])
	{
		UINT   NumDestDescriptorRanges = 1;
		DescriptorHandle pDestDescriptorRangeStarts = Alloc(count);
		UINT pDestDescriptorRangeSizes[1] = {count};
		UINT NumSrcDescriptorRanges;
		const D3D12_CPU_DESCRIPTOR_HANDLE* pSrcDescriptorRangeStarts;
		UINT pSrcDescriptorRangeSizes[16];
		for (size_t i = 0; i < count; i++)
		{
			pSrcDescriptorRangeSizes[i] = 1;
		}
		g_Device->CopyDescriptors(
		NumDestDescriptorRanges,
		pDestDescriptorRangeStarts.GetCpuAddress(),
		pDestDescriptorRangeSizes,
		count,
		cpuHandle,
		pSrcDescriptorRangeSizes,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);
		return pDestDescriptorRangeStarts;
	}
	DescriptorHandle GpuDescriptorAllocator::CopyToGpuDescriptor(int count, const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle[], D3D12_CPU_DESCRIPTOR_HANDLE destHandle)
	{

	}

}


