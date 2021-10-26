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
		}
		void Renderer::End()
		{
			graphicContext->End();
		}
		void Renderer::SetRenderTarget(RenderBuffer* renderTarget) 
		{
			renderTarget_ = renderTarget; 
			graphicContext->SetRenderTarget(renderTarget_->RtvCpuHandlePtr(), nullptr);
		};
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
		
			for (size_t i = 0; i < shader.Reflect->VertexInput.AttributesCount; i++)
			{
				vbo[i] = mesh.GetVertexBuffer(shader.Reflect->VertexInput.Attrs[i].Attribute);
			} 

			D3D12_INDEX_BUFFER_VIEW ibo = mesh.GetIndexBuffer();
			graphicContext->SetPipelineState(pso);
			graphicContext->BindGpuDescriptor();
			graphicContext->SetVertexBuffers(0, shader.Reflect->VertexInput.AttributesCount, vbo);
			graphicContext->SetIndexBuffer(&ibo);
			graphicContext->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			graphicContext->SetViewPortAndScissorRects(&viewport_, &scissorRect_);
			graphicContext->TransitionBarrier(*renderTarget_, D3D12_RESOURCE_STATE_RENDER_TARGET);
			graphicContext->DrawIndexInstance(6, 1, 0, 0, 0);
			graphicContext->TransitionBarrier(*renderTarget_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			

			useDefalutScissorRect = useDefalutViewport = true;
		}
	}
}