#pragma once
#include"pch.h"
#include<queue>


#include"GraphicContext.h"
#include"RootSignature.h"
#include"DescriptorHeap.h"

#include"Common/Utility.h"
namespace YiaEngine
{
	namespace Graphic
	{
		 GraphicContext& GraphicContext::Begin(const wchar_t* Name)
		{
			 CommandContext* commandContext = CommandContext::Begin(Name);
			 
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
			for (UINT  i = 0; i < rootSignature.ParamenterCount(); i++)
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
		
		void GraphicContext::BindCpuDescriptor(int rootIndex,int offset, size_t num,const DescriptorHandle descriptorHandles[])
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
 			SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, viewDescriptorAllocator.CurrentUseHeap().NativeHeap());
			auto gpuStartHandle = viewDescriptorAllocator.Alloc(tableSize_);
			UINT handleIncrementSize = viewDescriptorAllocator.ViewDescriptorIncrementSize();

			for (UINT i = 0; i < kMaxDescriptorTableNum; i++)
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
		void GraphicContext::BindTestConstBufferView(int rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
		{
			commandList_->SetGraphicsRootConstantBufferView(rootIndex, address);
		}
		void GraphicContext::BindDynamicConstBufferView(int rootIndex, UINT size, void* data)
		{
			AllocateBuffer allcBuffer = GetAllocateUploadBuffer(size, 16);
			memcpy_s(allcBuffer.CpuAddress, size, data, size);
			commandList_->SetGraphicsRootConstantBufferView(rootIndex, allcBuffer.GpuAddress);

		}
		void GraphicContext::BindConstBufferView(int rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
		{
			commandList_->SetGraphicsRootConstantBufferView(rootIndex, address);
		}
		void GraphicContext::BindConstBufferView(int rootIndex,UINT size, void* data,const char* Name)
		{
			AllocateBuffer allcBuffer =  GetAllocateUploadBuffer(size,16);

			wchar_t* wname = new wchar_t[strlen( Name )+ 1];
			Char2Wchar(Name,  wname);
			allcBuffer.Buffer.SetResourceName(wname);
			memcpy_s(allcBuffer.CpuAddress,size,data,size);

			commandList_->SetGraphicsRootConstantBufferView(rootIndex, allcBuffer.GpuAddress);
		
			delete wname;
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
		void GraphicContext::ClearRenderTarget(DescriptorHandle renderTarget, const Math::Vec4f& colors)
		{
			commandList_->ClearRenderTargetView(renderTarget, (const float*)(&colors), 0, nullptr);
		
		}
		void GraphicContext::ClearDepthStencil(DescriptorHandle depth,bool clearDepth,float depthValue,bool clearStencil,UINT8 stencilValue)
		{
			D3D12_CLEAR_FLAGS clearFlag = {};
			if (clearDepth)
				clearFlag |= D3D12_CLEAR_FLAG_DEPTH;
			if (clearStencil)
				clearFlag |= D3D12_CLEAR_FLAG_STENCIL;
			commandList_->ClearDepthStencilView(depth, clearFlag, depthValue, stencilValue, 0, nullptr);

		}

		void GraphicContext::ClearRenderTarget(DescriptorHandle renderTarget)
		{
			Math::Vec4f black = { 0.2f,0.3f,0.4f,1.0f };

			ClearRenderTarget(renderTarget, black);
		}
		
		void GraphicContext::SetRenderTargets(UINT numRT,const DescriptorHandle RTS[],const DescriptorHandle* depth)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE renderTargets[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
			for (UINT i = 0; i < numRT; i++)
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