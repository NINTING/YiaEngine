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
	


		class GraphicContext :public CommandContext
		{
		public:
			static GraphicContext& Begin(const wchar_t* name = L"");
			void SetRootSignature(const RootSignature& rootSignature);
			void ParseRootSignature(const RootSignature& rootSignature);
			
			
			void BindDescriptorTable(int rootIndex,const DescriptorHandle& startHandle);
			void BindCpuDescriptor(int rootIndex, int offset, int num, const DescriptorHandle descriptorHandles[]);
			void BindGpuDescriptor();
			void SetViewPortAndScissorRects(const D3D12_VIEWPORT* pViewPort, const D3D12_RECT* pAcissorRect_);
			void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE* renderTarget, D3D12_CPU_DESCRIPTOR_HANDLE* depth);
			void ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE renderTarget,const float colors[]);
			void ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE renderTarget);
			void SetRenderTargets(UINT numRT,D3D12_CPU_DESCRIPTOR_HANDLE RTS[], D3D12_CPU_DESCRIPTOR_HANDLE*depth);
			void SetVertexBuffers(UINT slot, UINT bufferCount, const D3D12_VERTEX_BUFFER_VIEW* vertexView);
			void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* indexView);
			void ExecuteBundle(ID3D12GraphicsCommandList* bundle);
			void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology);
			void DrawInstance();
			void DrawIndexInstance(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
			void DrawIndexInstance();
		private:
			
		};
	}
		
}