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
			for (size_t i = 0; i < numRt_; i++)
			{
				graphicContext->TransitionBarrier(*renderTarget_[i], D3D12_RESOURCE_STATE_COMMON);
				renderTarget_[i] = nullptr;
			}
			
			graphicContext->TransitionBarrier(*depthTarget_, D3D12_RESOURCE_STATE_COMMON);
			
			depthTarget_ = nullptr;
			graphicContext->End();
		}
		void Renderer::SetRenderTarget(RenderBuffer* renderTarget, DepthBuffer* depthTarget)
		{
			TransitionRenderTarget(0, renderTarget);
			TransitionDepthTarget(depthTarget);
			numRt_ = 1;
			pso_.SetRenderTarget(renderTarget->GetFormat(), depthTarget->GetFormat());
			graphicContext->SetRenderTarget(renderTarget_[0]->RtvCpuHandlePtr(), depthTarget_->GetDepthStencilHandlePtr());
		}
		void Renderer::SetRenderTargets(int numRt, RenderBuffer* renderTarget[], DepthBuffer* depthTarget)
		{
			numRt_ = numRt;
			DescriptorHandle rtHandle[8];
			DXGI_FORMAT formats[8];
			for (size_t i = 0; i < numRt; i++)
			{
				TransitionRenderTarget(i, renderTarget[i]);
				rtHandle[i] = renderTarget[i]->RtvCpuHandle();
				formats[i] = renderTarget[i]->GetFormat();
			}
			for (int i = numRt_;i<8;i++)
			{
				TransitionRenderTarget(i,nullptr);
			}

			TransitionDepthTarget(depthTarget);
			pso_.SetRenderTargets(numRt_, formats, depthTarget->GetFormat());
			graphicContext->SetRenderTargets(numRt, rtHandle, depthTarget_->GetDepthStencilHandlePtr());
		}
		void Renderer::TransitionRenderTarget(int i, RenderBuffer* Rt)
		{
			if (renderTarget_[i] != Rt)
			{
				if (renderTarget_[i])
				{
					graphicContext->TransitionBarrier(*renderTarget_[i], D3D12_RESOURCE_STATE_COMMON);
				}
				if (Rt)
				{
					graphicContext->TransitionBarrier(*Rt, D3D12_RESOURCE_STATE_RENDER_TARGET);
					renderTarget_[i] = Rt;
				}
			}
		}
		void Renderer::TransitionDepthTarget(DepthBuffer* depthBuffer)
		{
			
				if (depthTarget_ != depthBuffer)
				{
					
					if (depthTarget_)
					{
						graphicContext->TransitionBarrier(*depthTarget_, D3D12_RESOURCE_STATE_DEPTH_READ);
					}
					if (depthBuffer)
					{

						graphicContext->TransitionBarrier(*depthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE);
						depthTarget_ = depthBuffer;
					}
				}
			
		}
		
		void Renderer::SetCamera(Camera& camera)
		{
			pCamera_ = &camera;
		}
		;
		void Renderer::ClearRenderTarget(const Math::Vec4f& clearColor)
		{
			for (size_t i = 0; i < numRt_; i++)
			{
				graphicContext->ClearRenderTarget(renderTarget_[i]->RtvCpuHandle(), clearColor);
		
			}
			
		
		}
		void Renderer::ClearRenderTarget()
		{
			for (size_t i = 0; i < numRt_; i++)
			{
				graphicContext->ClearRenderTarget(renderTarget_[i]->RtvCpuHandle());

			}
		}
		void Renderer::ClearDepthStencil()
		{
			graphicContext->ClearDepthStencil(depthTarget_->GetDepthStencilHandle(),true,1,false);
		}
		

		void Renderer::SetDepthStencilState(bool depthEnable, D3D12_COMPARISON_FUNC comFunc /*= D3D12_COMPARISON_FUNC_LESS*/, D3D12_DEPTH_WRITE_MASK depthMask /*= D3D12_DEPTH_WRITE_MASK_ALL*/,
			bool stencilEnable /*= false*/, UINT8 stencilWriteMask /*= D3D12_DEFAULT_STENCIL_WRITE_MASK*/, UINT8 stencilReadMask /*= D3D12_DEFAULT_STENCIL_READ_MASK*/, 
			D3D12_STENCIL_OP frontStencilFailOp /*= D3D12_STENCIL_OP_KEEP*/, 
			D3D12_STENCIL_OP frontStencilDepthFailOp /*= D3D12_STENCIL_OP_KEEP*/, 
			D3D12_STENCIL_OP frontStencilPassOp /*= D3D12_STENCIL_OP_KEEP*/,
			D3D12_COMPARISON_FUNC frontStencilFunc /*= D3D12_COMPARISON_FUNC_ALWAYS*/, 
			D3D12_STENCIL_OP backStencilFailOp /*= D3D12_STENCIL_OP_KEEP*/, 
			D3D12_STENCIL_OP backStencilDepthFailOp /*= D3D12_STENCIL_OP_KEEP*/, 
			D3D12_STENCIL_OP backStencilPassOp /*= D3D12_STENCIL_OP_KEEP*/, 
			D3D12_COMPARISON_FUNC backStencilFunc /*= D3D12_COMPARISON_FUNC_ALWAYS*/)
		{
			D3D12_DEPTH_STENCIL_DESC desc;
			desc.DepthEnable = depthEnable;
			desc.DepthFunc = comFunc;
			desc.DepthWriteMask = depthMask;
			desc.StencilEnable = stencilEnable;
			desc.StencilReadMask = stencilReadMask;
			desc.StencilWriteMask = stencilWriteMask;
			desc.FrontFace.StencilDepthFailOp = frontStencilDepthFailOp;
			desc.FrontFace.StencilPassOp = frontStencilPassOp;
			desc.FrontFace.StencilFailOp = frontStencilFailOp;
			desc.FrontFace.StencilFunc = frontStencilFunc;

			desc.BackFace.StencilDepthFailOp = backStencilDepthFailOp;
			desc.BackFace.StencilPassOp = backStencilPassOp;
			desc.BackFace.StencilFailOp = backStencilFailOp;
			desc.BackFace.StencilFunc = backStencilFunc;
			pso_.SetDepthStencilState(desc);
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
		//void Renderer::SetConstBufferView(int rootIndex,int size,void* data) {
		//	graphicContext->BindConstBufferView(rootIndex, size,data);
		//};
		void Renderer::SetTestConstBufferView(int rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address) {
			graphicContext->BindTestConstBufferView(rootIndex, address);
		};
		void Renderer::DrawMesh(const Mesh& mesh, Material& material)
		{
			YIA_ASSERT(renderTarget_);
			material.Finalize();
			if (useDefalutViewport)
			{
				Viewport viewport = pCamera_->GetViewport();

				viewport_ = {};
				viewport_.Width = viewport.Width;
				viewport_.Height = viewport.Height;
			}
			if (useDefalutScissorRect)
			{
				scissorRect_ = {};
				scissorRect_.right = renderTarget_[0]->Size().x();
				scissorRect_.bottom = renderTarget_[0]->Size().y();
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
					if (data.Freq==ResourceFrequenceType::UPDATE_FREQ_PER_DRAW)
					{
						graphicContext->BindDynamicConstBufferView(data.location.RootIndex, data.Size, data.Data);
					}
					else
						graphicContext->BindConstBufferView(data.location.RootIndex,material.GetCbvData(i).GetGpuAddress());
				}
				else if (data.Type == ShaderResourceType::ShaderResourceType_Texture)
				{
					GpuTexture* texData = (GpuTexture*)(data.Data);
					auto handle = texData->SrvCpuHandle();
					//graphicContext->TransitionBarrier(handle,)
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
			//graphicContext->TransitionBarrier(*renderTarget_, D3D12_RESOURCE_STATE_RENDER_TARGET);
			graphicContext->DrawIndexInstance(mesh.GetIndexCount(), 1, 0, 0, 0);
			//graphicContext->TransitionBarrier(*renderTarget_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);


			useDefalutScissorRect = useDefalutViewport = true;
		}

		void Renderer::DrawMesh(MeshRenderer& meshRenderer)
		{
			DrawMesh(*meshRenderer.pMesh, *meshRenderer.pMaterial);
		}

		
		Camera& Renderer::GetCurrentCamera()
		{
			return  *pCamera_;
		}
	}
}