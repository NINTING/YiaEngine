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
		/// ����DescriptorTable�İ������������Լ�Gpu Handle ��ʼλ��
		/// </summary>
		struct GpuDescriptorTable
		{
			DescriptorHandle StartHandle;
			UINT TableSize;
		};


		class GraphicContext : CommandContext
		{
		public:
			void SetRootSignature(const RootSignature& rootSignature);
			void ParseRootSignature();
			void SetPipelineState(const PipelineStateObject& pso);
			void SetDescriptorHeaps(D3D12_DESCRIPTOR_HEAP_TYPE type, const DescriptorHeap&);
			void BindDescriptorTable(int rootIndex,const DescriptorHandle& startHandle);
			void SetVertexBuffer();
			void SetIndexBuffer();
			void SetPrimitiveTopology();
			void DrawInstance();
		
		};
	}
		
}