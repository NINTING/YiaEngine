#pragma once

#include"Core/YiaGraphic.h"
#include"Component/MeshObject.h"
#include"Common/Camera.h"
#include"Material.h"
#include"Component/MeshRenderer.h"
namespace YiaEngine
{
	namespace Graphic
	{
		class Renderer
		{
		public:
			void Begin();
			void BeginPass();
			void End();
			void SetRenderTargets(int numRt, RenderBuffer* renderTarget[], DepthBuffer* depthTarget = nullptr);
			void SetRenderTarget(RenderBuffer* renderTarget, DepthBuffer* depthTarget = nullptr);
			void SetCamera(Camera& camera);
			void ClearRenderTarget(const Math::Vec4f& clearColor);
			void ClearRenderTarget();
			void ClearDepthStencil();
			void SetDepthStencilState(bool depthEnable, D3D12_COMPARISON_FUNC comFunc =  D3D12_COMPARISON_FUNC_LESS, D3D12_DEPTH_WRITE_MASK depthMask = D3D12_DEPTH_WRITE_MASK_ALL, 
				bool stencilEnable = false,
				UINT8 stencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK, UINT8 stencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK,
				D3D12_STENCIL_OP frontStencilFailOp = D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP frontStencilDepthFailOp = D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP frontStencilPassOp = D3D12_STENCIL_OP_KEEP,
				D3D12_COMPARISON_FUNC frontStencilFunc = D3D12_COMPARISON_FUNC_ALWAYS,
				D3D12_STENCIL_OP backStencilFailOp =  D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP backStencilDepthFailOp = D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP backStencilPassOp =  D3D12_STENCIL_OP_KEEP,
				D3D12_COMPARISON_FUNC backStencilFunc = D3D12_COMPARISON_FUNC_ALWAYS);

			void SetViewport(const CD3DX12_VIEWPORT& viewport);
			void SetScissorRect(const CD3DX12_RECT& scissorRect);
			void SetRootSignature(const RootSignature& signature);
			void SetConstBufferView(int rootIndex, int size, void* data);
			void SetConstBufferView(int rootIndex, void* data);
			void SetTestConstBufferView(int rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address);
			void DrawMesh(const Mesh& mesh,  Material& shader);
			void DrawMesh(MeshRenderer&meshRenderer);
			Camera& GetCurrentCamera();
		private:
			void TransitionDepthTarget(DepthBuffer* Rt);
			void TransitionRenderTarget(int i, RenderBuffer* Rt);
		private:
			int numRt_;
			RenderBuffer* renderTarget_[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
			DepthBuffer* depthTarget_;
			PipelineStateObject pso_;
			bool useDefalutViewport;
			bool useDefalutScissorRect;
			CD3DX12_VIEWPORT viewport_;
			CD3DX12_RECT scissorRect_;
			Graphic::GraphicContext* graphicContext;
			Camera* pCamera_;
		};
	}
}
