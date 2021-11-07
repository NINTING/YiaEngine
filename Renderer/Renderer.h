#pragma once

#include"Core/YiaGraphic.h"

#include"Common/Model/MeshObject.h"
#include"Common/Camera.h"
#include"Material.h"
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
			void SetRenderTarget(RenderBuffer* renderTarget);
			void SetRenderTarget(RenderBuffer* renderTarget, DepthBuffer* depthTarget);
			void SetCamera(Camera& camera);
			void ClearRenderTarget(const Math::Vec4f& clearColor);
			void ClearRenderTarget();
			void ClearDepthStencil();
			void SetViewport(const CD3DX12_VIEWPORT& viewport);
			void SetScissorRect(const CD3DX12_RECT& scissorRect);
			void SetRootSignature(const RootSignature& signature);
			void SetConstBufferView(int rootIndex, int size, void* data);
			void SetConstBufferView(int rootIndex, void* data);
			void SetTestConstBufferView(int rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address);
			void DrawMesh(const Mesh& mesh, PipelineStateObject& pso, Material& shader);
			void DrawMesh(const Mesh& mesh, PipelineStateObject& pso, Shader& shader);
			Camera& GetCurrentCamera();
		private:
			RenderBuffer* renderTarget_;
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
