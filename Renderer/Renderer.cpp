#include "pch.h"
#include "Renderer.h"
#include "Common/YiaMath.h"
namespace YiaEngine
{
	namespace Graphic
	{
		void Renderer::Begin()
		{
			graphicContext = &Graphic::GraphicContext::Begin();
			useDefalutViewport = useDefalutScissorRect = true;
		}
		void Renderer::End()
		{
			graphicContext->End();
		}
		void Renderer::SetRenderTarget(RenderBuffer* renderTarget) 
		{
			renderTarget_ = renderTarget; 
			graphicContext->SetRenderTarget(renderTarget_->RtvCpuHandlePtr(), nullptr);
		}
		void Renderer::SetCamera(Camera& camera)
		{
			pCamera_ = &camera;
		}
		;
		void Renderer::ClearRenderTarget(const Math::Vec4f& clearColor)
		{
			graphicContext->ClearRenderTarget(renderTarget_->RtvCpuHandle(),clearColor);
		}
		void Renderer::ClearRenderTarget()
		{
			graphicContext->ClearRenderTarget(renderTarget_->RtvCpuHandle());
		}
		void Renderer::SetViewport(const CD3DX12_VIEWPORT& viewport)
		{ 
			viewport_ = viewport; 
			useDefalutViewport = false; 
		};
		void Renderer::SetScissorRect(const CD3DX12_RECT& scissorRect) {
			scissorRect_ = scissorRect; 
			useDefalutScissorRect = false; 
		};
		void Renderer::SetRootSignature(const RootSignature& signature) {
			graphicContext->SetRootSignature(signature);
		};
		void Renderer::SetConstBufferView(int rootIndex,int size,void* data) {
			graphicContext->BindConstBufferView(rootIndex, size,data);
		};
		void Renderer::SetTestConstBufferView(int rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address) {
			graphicContext->BindTestConstBufferView(rootIndex, address);
		};
		void Renderer::DrawMesh(const Mesh& mesh, PipelineStateObject& pso, Shader& shader)
		{
			YIA_ASSERT(renderTarget_);
		
			if (useDefalutViewport)
			{
				viewport_ = {};
				viewport_.Width = renderTarget_->Size().x();
				viewport_.Height = renderTarget_->Size().y();
			}
			if (useDefalutScissorRect)
			{
				scissorRect_ = {};
				scissorRect_.right = renderTarget_->Size().x();
				scissorRect_.bottom = renderTarget_->Size().y();
			}

			D3D12_VERTEX_BUFFER_VIEW vbo[15];
		
			__declspec(align(16)) struct TransformMat
			{
				Math::Mat4x4f ObjMat;
				Math::Mat4x4f WorldMat;
				Math::Mat4x4f ViewMat;
				Math::Mat4x4f ProjMat;
			} transfromMat;

			for (size_t i = 0; i < shader.Reflect->VertexInput.AttributesCount; i++)
			{
				vbo[i] = mesh.GetVertexBuffer(shader.Reflect->VertexInput.Attrs[i].Attribute);
			} 
			
			transfromMat.ObjMat = Math::Mat4x4f::Identity();
			transfromMat.WorldMat = Math::Mat4x4f::Identity();
			
			if (pCamera_ != nullptr)
			{
				transfromMat.ViewMat = pCamera_->ViewMatrix();
				transfromMat.ProjMat = pCamera_->ProjectionMatrix();
			}
			
			D3D12_INDEX_BUFFER_VIEW ibo = mesh.GetIndexBuffer();
			graphicContext->SetPipelineState(pso);
		//	graphicContext->BindGpuDescriptor();
			graphicContext->BindConstBufferView(0,sizeof(transfromMat),&transfromMat,"TransformCBV");
			graphicContext->SetVertexBuffers(0, shader.Reflect->VertexInput.AttributesCount, vbo);
			graphicContext->SetIndexBuffer(&ibo);
			graphicContext->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			graphicContext->SetViewPortAndScissorRects(&viewport_, &scissorRect_);
			graphicContext->TransitionBarrier(*renderTarget_, D3D12_RESOURCE_STATE_RENDER_TARGET);
			graphicContext->DrawIndexInstance(mesh.GetIndexCount(), 1, 0, 0, 0);
			graphicContext->TransitionBarrier(*renderTarget_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			

			useDefalutScissorRect = useDefalutViewport = true;
		}
		Camera& Renderer::GetCurrentCamera()
		{
			return  *pCamera_;
		}
	}
}