#pragma once
#include"pch.h"
#include <d3d12.h>
#include <wrl/client.h>
#include <vector>
#include<cstring>
#include<memory>


#include"Utility.h"
#include"PipelineStateObject.h"

namespace YiaEngine::Graphic
{
	PipelineStateObject::PipelineStateObject(const wchar_t* name):name_(name),signature_(nullptr)
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
	void PipelineStateObject::SetVertexShader(void* buffer, size_t size)
	{
		desc_.VS.pShaderBytecode = (UINT8*)buffer;
		desc_.VS.BytecodeLength = size;
	}
	void PipelineStateObject::SetPixelShader(void* buffer, size_t size)
	{
		desc_.PS.pShaderBytecode = (UINT8*)buffer;
		desc_.PS.BytecodeLength = size;
	}
	void PipelineStateObject::SetRasterizerState(D3D12_RASTERIZER_DESC state)
	{
		desc_.RasterizerState = state;
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
	void PipelineStateObject::SetRenderTargets(size_t count,DXGI_FORMAT* rtv_format,DXGI_FORMAT depth_format)
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
		ASSERT_SUCCEEDED(Graphic::g_Device->CreateGraphicsPipelineState(&desc_, IID_PPV_ARGS(&pso_)));
		pso_->SetName(name_);
	}
}