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
			 CommandContext* commandContext = CommandContext::Begin(name);
			 
			return reinterpret_cast<GraphicContext&>(*commandContext);
		}
		void GraphicContext::SetRootSignature(const RootSignature& rootSignature)
		{
			commandList_->SetGraphicsRootSignature(rootSignature.RawRootSignature());
			ParseRootSignature(rootSignature);
		}
		void GraphicContext::ParseRootSignature(const RootSignature& rootSignature)
		{
			tableSize_ = 0;
			memset(cpuDescriptorPool_, 0, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * kMaxDescriptorNum);
			for (int i = 0; i < rootSignature.ParamenterCount(); i++)
			{
				tableCache_[i].BaseOffset = tableSize_;
				
				
				tableCache_[i].StartHandle = &cpuDescriptorPool_[tableSize_];
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
			
			auto& cputableCache = tableCache_[rootIndex];

			//cputableCache.StartHandle = &cpuDescriptorPool_[cputableCache.BaseOffset + offset];
			for (size_t i = 0; i < num; i++)
			{
				cputableCache.StartHandle[i+ offset] = descriptorHandles[i];
			}
		}

		void  GraphicContext::BindGpuDescriptor()
		{
			//ID3D12DescriptorHeap* heaps[] = { viewDescriptorAllocator.CurrentUseHeap().heap_ptr() };
			//commandList_->SetDescriptorHeaps(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, heaps);
			
			static const int kMaxDescriptorNumPerCopy = 16;
			
			if(! viewDescriptorAllocator.HasSpace(tableSize_))
			{ 
				viewDescriptorAllocator.RetireCurrentHeap();
			}
 			SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, viewDescriptorAllocator.CurrentUseHeap().RawHeap());
			auto gpuStartHandle = viewDescriptorAllocator.Alloc(tableSize_);
			int handleIncrementSize = viewDescriptorAllocator.ViewDescriptorIncrementSize();

			for (size_t i = 0; i < kMaxDescriptorTableNum; i++)
			{
				auto gpuCurrentHandle = gpuStartHandle;
				if (tableCache_[i].TableSize == 0)continue;
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
		void GraphicContext::SetRenderTarget(const DescriptorHandle* renderTarget,const DescriptorHandle* depth)
		{
			SetRenderTargets(1, renderTarget, depth);
		}
		void GraphicContext::ClearRenderTarget(DescriptorHandle renderTarget,const float colors[])
		{
			commandList_->ClearRenderTargetView(renderTarget, colors, 0, nullptr);
		}

		void GraphicContext::ClearRenderTarget(DescriptorHandle renderTarget)
		{
			float black[] = { 0,0,0,1.0f };
			ClearRenderTarget(renderTarget, black);
		}
		
		void GraphicContext::SetRenderTargets(UINT numRT,const DescriptorHandle RTS[],const DescriptorHandle* depth)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE renderTargets[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
			for (int i = 0; i < numRT; i++)
			{
				renderTargets[i] = RTS[i];
			}
			commandList_->OMSetRenderTargets(numRT, renderTargets, false, depth->GetCpuAddress());
		}
		void GraphicContext::SetVertexBuffers(UINT slot, UINT bufferCount, const D3D12_VERTEX_BUFFER_VIEW* vertexView)
		{
			commandList_->IASetVertexBuffers(slot, bufferCount, vertexView);
		}

		void GraphicContext::SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* indexView)
		{
			commandList_->IASetIndexBuffer(indexView);
		}
		void GraphicContext::ExecuteBundle(ID3D12GraphicsCommandList* bundle)
		{
			commandList_->ExecuteBundle(bundle);
		}
		
		void GraphicContext::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
		{
			commandList_->IASetPrimitiveTopology(topology);
		}

		void GraphicContext::DrawInstance(UINT vertexCountPerInstance,
			UINT instanceCount,
			UINT startVertexLocation,
			UINT startInstanceLocation)
		{
			commandList_->DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
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