#pragma once

#include<queue>

#include"Common/YiaMath.h"
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
			static GraphicContext& Begin(const wchar_t* Name = L"");
			void SetRootSignature(const RootSignature& rootSignature);
			void ParseRootSignature(const RootSignature& rootSignature);
			
			
			void BindDescriptorTable(int rootIndex,const DescriptorHandle& startHandle);
			void BindCpuDescriptor(int rootIndex, int offset, size_t num, const DescriptorHandle descriptorHandles[]);
			void BindConstBufferView(int rootIndex, UINT size, void* data, const char* Name);
			void BindGpuDescriptor();

			void BindTestConstBufferView(int rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address);

			void BindConstBufferView(int rootIndex, UINT size, void* data);
			
			void SetViewPortAndScissorRects(const D3D12_VIEWPORT* pViewPort, const D3D12_RECT* pAcissorRect_);
			void SetRenderTarget(const DescriptorHandle* renderTarget,const DescriptorHandle* depth);
			void ClearRenderTarget(DescriptorHandle renderTarget, const Math::Vec4f& colors);
			void ClearRenderTarget(DescriptorHandle renderTarget);
			void ClearDepthStencil(DescriptorHandle depth, bool clearDepth, float depthValue, bool clearStencil, UINT8 stencilValue = 0);
			void SetRenderTargets(UINT numRT,const DescriptorHandle RTS[],const DescriptorHandle* depth);
			void SetVertexBuffers(UINT slot, UINT bufferCount, const D3D12_VERTEX_BUFFER_VIEW* vertexView);
			void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* indexView);
			void ExecuteBundle(ID3D12GraphicsCommandList* bundle);
			void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology);
		
			void DrawInstance(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation = 0, UINT startInstanceLocation = 0);
			
			void DrawIndexInstance(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
			
		private:
			
		};
	}
		
}