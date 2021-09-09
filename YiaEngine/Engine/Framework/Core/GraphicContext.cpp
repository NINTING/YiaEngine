#pragma once

#include<queue>


#include"GraphicContext.h"
#include"RootSignature.h"
#include"DescriptorHeap.h"
namespace YiaEngine
{
	namespace Graphic
	{
		GraphicContext& GraphicContext::Begin(const wchar_t* name)
		{
			auto commandContext = CommandContext::Begin(name);
			return reinterpret_cast<GraphicContext&>(commandContext);
		}
		void GraphicContext::SetRootSignature(const RootSignature& rootSignature)
		{
			commandList_->SetGraphicsRootSignature(rootSignature.RawRootSignature());
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
						viewDescriptorAllocator.CopyToGpuDescriptor(validCount, copySrc, gpuCurrentHandle.GetCpuAddress());
						gpuCurrentHandle += validCount * handleIncrementSize;
						validCount = 0;
					}
				}
				if(validCount!=0)
					viewDescriptorAllocator.CopyToGpuDescriptor(validCount, copySrc, gpuCurrentHandle.GetCpuAddress());
			}
		}
		void GraphicContext::SetViewPortAndScissorRects(const D3D12_VIEWPORT* pViewPort, const D3D12_RECT* pAcissorRect)
		{
			commandList_->RSSetViewports(1, pViewPort);
			commandList_->RSSetScissorRects(1, pAcissorRect);

		}
		void GraphicContext::SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE* renderTarget, D3D12_CPU_DESCRIPTOR_HANDLE* depth)
		{
			SetRenderTargets(1, renderTarget, depth);
		}
		
		void GraphicContext::SetRenderTargets(UINT numRT, D3D12_CPU_DESCRIPTOR_HANDLE RTS[], D3D12_CPU_DESCRIPTOR_HANDLE* depth)
		{
			commandList_->OMSetRenderTargets(numRT, RTS, false, depth);
		}
		void GraphicContext::SetVertexBuffers(UINT slot, UINT bufferCount, const D3D12_VERTEX_BUFFER_VIEW* vertexView)
		{
			commandList_->IASetVertexBuffers(slot, bufferCount, vertexView);
		}

		void GraphicContext::SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* indexView)
		{
			commandList_->IASetIndexBuffer(indexView);
		}
		
		
		void GraphicContext::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
		{
			commandList_->IASetPrimitiveTopology(topology);
		}

		void GraphicContext::DrawInstance()
		{

		}

		void GraphicContext::DrawIndexInstance(UINT indexCountPerInstance, UINT instanceCount,
		UINT startIndexLocation,
		INT baseVertexLocation,
		UINT startInstanceLocation)
		{
			commandList_->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startIndexLocation);
		}
	}
		
}