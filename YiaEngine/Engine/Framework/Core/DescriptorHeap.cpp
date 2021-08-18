#pragma once


#include<string>

#include"Utility.h"
#include"DescriptorHeap.h"

#include"Core/Graphic.h"

namespace YiaEngine::Graphic
{
	DescriptorHeap::DescriptorHeap(const wchar_t* name,D3D12_DESCRIPTOR_HEAP_DESC desc)
	{
		desc_ = desc;
		
		auto hr = Graphic::g_device->CreateDescriptorHeap(&desc_, IID_PPV_ARGS(&heap_));
		heap_->SetName(name);

		InitHeap();
	}
	void DescriptorHeap::Create(const wchar_t* name, D3D12_DESCRIPTOR_HEAP_DESC desc)
	{
		desc_ = desc;
		auto hr = Graphic::g_device->CreateDescriptorHeap(&desc_, IID_PPV_ARGS(&heap_));
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
		descriptor_size_ = Graphic::g_device->GetDescriptorHandleIncrementSize(desc_.Type);
		free_descriptor_num_ = desc_.NumDescriptors;
		first_handle_ = DescriptorHandle(heap_->GetCPUDescriptorHandleForHeapStart(), heap_->GetGPUDescriptorHandleForHeapStart());
		head_free_handle_ = first_handle_;
	}
	
	DescriptorHandle DescriptorAllocator::Alloc(uint32_t count = 1)
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

	void DescriptorAllocator::AllocHeap(D3D12_DESCRIPTOR_HEAP_TYPE type)
	{
		DescriptorHeap heap;
		wchar_t name[25];
		wsprintf(name, L"Cpu_Alloc_%d", heaps_.size()+1);
		heap.CreateNonShaderVisibleType(name, type,descriptor_count_perframe_);
		heaps_.push_back(heap);
	}

}