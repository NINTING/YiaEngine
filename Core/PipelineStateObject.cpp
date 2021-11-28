#pragma once
#include"pch.h"
#include <d3d12.h>
#include <wrl/client.h>
#include <vector>
#include<cstring>
#include<memory>
#include"Common/Utility.h"

#include"Utility.h"
#include"PipelineStateObject.h"

namespace YiaEngine::Graphic
{
	std::map<size_t, ComPtr<ID3D12PipelineState>> PipelineStateObject::s_PipelineStatePool;
	PipelineStateObject PipelineStateObject::s_DefaultPSO(L"Default PSO");
	void PipelineStateObject::StaticPSOInit()
	{
		CD3DX12_RASTERIZER_DESC defalutRasterizerDesc(D3D12_DEFAULT);
		s_DefaultPSO.SetRasterizerState(defalutRasterizerDesc);
		s_DefaultPSO.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
		s_DefaultPSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		s_DefaultPSO.SetDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
		s_DefaultPSO.SetRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
		s_DefaultPSO.SetSampleDesc(1, 0);
		s_DefaultPSO.SetSampleMask(UINT_MAX);

	}
	PipelineStateObject::PipelineStateObject(const wchar_t* Name):name_(Name),signature_(nullptr)
	{
		ZeroMemory(&desc_, sizeof(desc_));
		desc_.SampleDesc.Count = 1;
	}

	
	
	void PipelineStateObject::SetInputLayout(UINT NumElements, const D3D12_INPUT_ELEMENT_DESC* pInputElementDescs)
	{
		desc_.InputLayout.NumElements = NumElements;
		if (NumElements > 0)
		{
			D3D12_INPUT_ELEMENT_DESC* NewElements = (D3D12_INPUT_ELEMENT_DESC*)malloc(sizeof(D3D12_INPUT_ELEMENT_DESC) * NumElements);
			memcpy(NewElements, pInputElementDescs, NumElements * sizeof(D3D12_INPUT_ELEMENT_DESC));
			input_layout_.reset((const D3D12_INPUT_ELEMENT_DESC*)NewElements);
			desc_.InputLayout.pInputElementDescs = input_layout_.get();
		}
		else
			input_layout_ = nullptr;
	}
	void PipelineStateObject::SetInputLayout(const D3D12_INPUT_LAYOUT_DESC& layout_desc)
	{
		desc_.InputLayout = layout_desc;

	}
	void PipelineStateObject::SetRootSignature(const RootSignature& signature)
	{
		signature_ = &signature;
		desc_.pRootSignature = signature.RawRootSignature();
	}

	void PipelineStateObject::SetShader(const Shader& shader)
	{
		for (size_t i = 0; i < Shader_Stage_Count; i++)
		{
			ShaderStage stage = (ShaderStage)(1 << i);

			if ((shader.Stages & stage) == stage)
			{
				const void* pointer = shader.ShaderBlob[i].Data;
				size_t size = shader.ShaderBlob[i].DataSize;
				switch (stage)
				{
				
				case YiaEngine::Graphic::Shader_Stage_Vertex:
					SetVertexShader(pointer, size);
					break;
				case YiaEngine::Graphic::Shader_Stage_Pixel:
					SetPixelShader(pointer, size);
					break;
				
				}
			}
		}
	}

	void PipelineStateObject::SetVertexShader(const void* buffer, size_t size)
	{
		desc_.VS.pShaderBytecode = (UINT8*)buffer;
		desc_.VS.BytecodeLength = size;
	}
	void PipelineStateObject::SetPixelShader(const void* buffer, size_t size)
	{
		desc_.PS.pShaderBytecode = (UINT8*)buffer;
		desc_.PS.BytecodeLength = size;
	}
	
	void PipelineStateObject::SetRasterizerState(D3D12_RASTERIZER_DESC state)
	{
		desc_.RasterizerState = state;
	}
	void PipelineStateObject::SetWireframeShowMode()
	{
		desc_.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	}
	void PipelineStateObject::SetSolideShowMode()
	{
		desc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	}
	
	void PipelineStateObject::SetBackCullMode()
	{
		desc_.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	}
	void PipelineStateObject::SetFrontCullMode()
	{
		desc_.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
	}
	void PipelineStateObject::SetNoneCullMode()
	{
		desc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	}
	void PipelineStateObject::SetBlendState(D3D12_BLEND_DESC desc)
	{
		desc_.BlendState = desc;
	}
	void PipelineStateObject::SetDepthStencilState(D3D12_DEPTH_STENCIL_DESC depth_stencil_desc)
	{
		
		desc_.DepthStencilState = depth_stencil_desc;
	}
	void PipelineStateObject::SetSampleMask(UINT mask)
	{
		desc_.SampleMask = mask;
	}
	void PipelineStateObject::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type)
	{
		desc_.PrimitiveTopologyType = type;
	}
	void PipelineStateObject::SetRenderTarget(DXGI_FORMAT rtv_format, DXGI_FORMAT depth_format)
	{
		SetRenderTargets(1, &rtv_format, depth_format);
	}
	void PipelineStateObject::SetRenderTargets(UINT count,DXGI_FORMAT* rtv_format,DXGI_FORMAT depth_format)
	{
		for (size_t i = 0; i < count; i++)
		{
			desc_.RTVFormats[i] = rtv_format[i];
		}
		for (size_t i = count; i < desc_.NumRenderTargets; i++)
		{
			desc_.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
		}
		desc_.NumRenderTargets  = count;
		desc_.DSVFormat = depth_format;
	}
	void PipelineStateObject::SetSampleDesc(UINT count,UINT quality)
	{
		desc_.SampleDesc.Count = count;
		desc_.SampleDesc.Quality = quality;
	}
	void PipelineStateObject::Finalize()
	{

		desc_.InputLayout.pInputElementDescs = nullptr;
		size_t hash = HashState(&desc_);
		hash = HashState(input_layout_.get(),desc_.InputLayout.NumElements,hash);
		desc_.InputLayout.pInputElementDescs = input_layout_.get();

		if (s_PipelineStatePool.find(hash) != s_PipelineStatePool.end())
		{
			pso_ = s_PipelineStatePool[hash];
		}
		else
		{
			ASSERT_SUCCEEDED(Graphic::g_Device->CreateGraphicsPipelineState(&desc_, IID_PPV_ARGS(&pso_)));
			s_PipelineStatePool[hash] = pso_;
			pso_->SetName(name_);
		}
	}
}