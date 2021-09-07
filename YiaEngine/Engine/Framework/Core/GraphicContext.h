#pragma once

#include<queue>


#include"CommandContext.h"
namespace YiaEngine
{
	namespace Graphic
	{
		class RootSignature;
		class PipelineStateObject;
		class DescriptorHeap;
		class DescriptorHandle;
		/// <summary>
		/// 描述DescriptorTable的绑定描述符数量以及Gpu Handle 起始位置
		/// </summary>
		struct GpuDescriptorTable
		{
			D3D12_CPU_DESCRIPTOR_HANDLE* StartHandle;
			UINT TableSize;
			UINT BaseOffset;
		};


		class GraphicContext : CommandContext
		{
		public:
			void SetRootSignature(const RootSignature& rootSignature);
			void ParseRootSignature(const RootSignature& rootSignature);
			void SetPipelineState(const PipelineStateObject& pso);
			void SetDescriptorHeaps(D3D12_DESCRIPTOR_HEAP_TYPE type, const DescriptorHeap&);
			void BindDescriptorTable(int rootIndex,const DescriptorHandle& startHandle);
			void BindCpuDescriptor(int rootIndex, int offset, int num, const DescriptorHandle descriptorHandles[]);
			void BindGpuDescriptor();
			void BindCpuDescriptor(int rootIndex, int registerBase, const DescriptorHandle startHandle[]);
			void SetVertexBuffer();
			void SetIndexBuffer();
			void SetPrimitiveTopology();
			void DrawInstance();
		private:
			static const int kMaxDescriptorNum = 256;
			static const int kMaxDescriptorTableNum = 16;
			GpuDescriptorTable tableCache_[kMaxDescriptorTableNum];
			D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorPool_[kMaxDescriptorNum];
			UINT tableSize_ = 0;
		};
	}
		
}