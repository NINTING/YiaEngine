#include "pch.h"
#include "Renderer.h"
#include "Common/YiaMath.h"
#include"Material.h"
namespace YiaEngine
{
	namespace Graphic
	{
		void Renderer::Begin()
		{
			graphicContext = &Graphic::GraphicContext::Begin();
			pso_ = Graphic::PipelineStateObject::s_DefaultPSO;
			useDefalutViewport = useDefalutScissorRect = true;
		}
		void Renderer::BeginPass()
		{
			//pso_ = 
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
		void Renderer::SetRenderTarget(RenderBuffer* renderTarget,DepthBuffer* depthTarget)
		{
			renderTarget_ = renderTarget;
			depthTarget_ = depthTarget;
			graphicContext->SetRenderTarget(renderTarget_->RtvCpuHandlePtr(), depthTarget_->GetDepthStencilHandlePtr());
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
		void Renderer::ClearDepthStencil()
		{
			graphicContext->ClearDepthStencil(depthTarget_->GetDepthStencilHandle(),true,1,false);
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
		void Renderer::DrawMesh(const Mesh& mesh, Material& material)
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

			
			Shader& shader = material.GetShader();

			


#pragma region RootSignature
			SetRootSignature(material.GetRootSignature());


			bool bindGpuDescriptor = false;

			
			//	graphicContext->BindGpuDescriptor();
			for (size_t i = 0; i < material.GetResourceCount(); i++)
			{
				ResourceData& data = material.GetResourceData(i);
				if (data.Type == ShaderResourceType::ShaderResourceType_ConstBuffer)
				{
					graphicContext->BindConstBufferView(data.location.RootIndex, data.Size, data.Data);
				}
				else if (data.Type == ShaderResourceType::ShaderResourceType_Texture)
				{
					Texture* texData = (Texture*)(data.Data);
					auto handle = texData->CpuHandle();

					graphicContext->BindCpuDescriptor(data.location.RootIndex, data.location.Offset, data.Count, &handle);
					bindGpuDescriptor = true;
				}
				else if (data.Type == ShaderResourceType::ShaderResourceType_Sampler)
				{
				}
				else
				{
					YIA_ERROR("未知Shader 资源处理{0}", i);
				}
			}
			if (bindGpuDescriptor)
				graphicContext->BindGpuDescriptor();
#pragma endregion

#pragma region PSO
			D3D12_INPUT_LAYOUT_DESC desc;
			auto& vertexInput = shader.Reflect[0].VertexInput;

			desc.NumElements = shader.Reflect[0].VertexInput.AttributesCount;
			D3D12_INPUT_ELEMENT_DESC elements[8];// = new D3D12_INPUT_ELEMENT_DESC[desc.NumElements];
			for (size_t i = 0; i < desc.NumElements; i++)
			{
				elements[i].SemanticName = vertexInput.Attrs[i].SemanticName;
				elements[i].SemanticIndex = vertexInput.Attrs[i].SemanticIndex;
				elements[i].Format = DataFormatToDX12(vertexInput.Attrs[i].format);
				elements[i].AlignedByteOffset = 0;
				elements[i].InputSlot = i;
				elements[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
				elements[i].InstanceDataStepRate = 0;
			}

		
			pso_.SetRootSignature(material.GetRootSignature());

			pso_.SetInputLayout(desc.NumElements, elements);
			//delete elements;

			pso_.SetShader(shader);
		
			//pso.SetRenderTarget();
			pso_.Finalize();

			graphicContext->SetPipelineState(pso_);

#pragma endregion

			D3D12_INDEX_BUFFER_VIEW ibo = mesh.GetIndexBuffer();

			for (size_t i = 0; i < shader.Reflect->VertexInput.AttributesCount; i++)
			{
				vbo[i] = mesh.GetVertexBuffer(shader.Reflect->VertexInput.Attrs[i].Attribute);
			}

			

			
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