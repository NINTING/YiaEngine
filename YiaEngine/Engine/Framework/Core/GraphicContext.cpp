#pragma once

#include<queue>


#include"GraphicContext.h"
#include"RootSignature.h"
#include"DescriptorHeap.h"
namespace YiaEngine
{
	namespace Graphic
	{
		void GraphicContext::SetRootSignature(const RootSignature& rootSignature)
		{
			commandList_->SetGraphicsRootSignature(rootSignature.RootSignaturePtr());
			ParseRootSignature(rootSignature);
		}
		void GraphicContext::ParseRootSignature(const RootSignature& rootSignature)
		{
			tableSize_ = 0;
			for (int i = 0; i < rootSignature.ParamenterCount(); i++)
			{
				tableCache_[i].BaseOffset = tableSize_;
				tableSize_ += rootSignature[i].DescriptorTableSize();
				tableCache_[i].TableSize = tableSize_ - tableCache_[i].BaseOffset;
			}
		}
		
		void GraphicContext::BindDescriptorTable(int rootIndex, const DescriptorHandle& startHandle)
		{
			commandList_->SetGraphicsRootDescriptorTable(rootIndex, startHandle);
		}
		
		void GraphicContext::BindCpuDescriptor(int rootIndex,int offset, int num,const DescriptorHandle descriptorHandles[])
		{
			auto cputableCache = tableCache_[rootIndex];

			cputableCache.StartHandle = &cpuDescriptorPool_[cputableCache.BaseOffset + offset];
			for (size_t i = 0; i < num; i++)
			{
				cputableCache.StartHandle[i] = descriptorHandles[i];
			}
		}

		void  GraphicContext::BindGpuDescriptor()
		{
			static const int kMaxDescriptorNumPerCopy = 16;
			auto gpuStartHandle = viewDescriptorAllocator.Alloc(tableSize_);
			
			int handleIncrementSize = viewDescriptorAllocator.ViewDescriptorIncrementSize();

			for (size_t i = 0; i < kMaxDescriptorTableNum; i++)
			{
				auto gpuCurrentHandle = gpuStartHandle;
				commandList_->SetGraphicsRootDescriptorTable(i, gpuStartHandle);
				gpuStartHandle += tableCache_[i].TableSize * handleIncrementSize;
				auto cpuHandles = tableCache_[i].StartHandle;
				int validCount = 0;
				
				D3D12_CPU_DESCRIPTOR_HANDLE copySrc[kMaxDescriptorNumPerCopy];
				for (size_t j = 0; j < tableCache_[i].TableSize; j++)
				{
					if (cpuHandles[j].ptr != 0)
					{
						copySrc[validCount++] = cpuHandles[j];
					}
					if (validCount == kMaxDescriptorNumPerCopy)
					{
						viewDescriptorAllocator.CopyToGpuDescriptor(validCount, copySrc, gpuCurrentHandle);
						gpuCurrentHandle += validCount * handleIncrementSize;
						validCount = 0;
					}
				}
				if(validCount!=0)
					viewDescriptorAllocator.CopyToGpuDescriptor(validCount, copySrc, gpuCurrentHandle);
			}
		}
	}
		
}