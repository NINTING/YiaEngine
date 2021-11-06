#pragma once
#include"pch.h"
#include"Utility.h"
#include"Core/Graphic.h"
#include "RootSignature.h"
namespace YiaEngine::Graphic
{
	void RootSignature::Reset(int root_parament_count, int static_sampler_count)
	{
		if (root_parament_count > 0)
			paraments_.reset(new RootParameter[root_parament_count]);
		else
			paraments_ = nullptr;
		paramenterCount_ = root_parament_count;

		if (static_sampler_count > 0)
			samplers_.reset(new D3D12_STATIC_SAMPLER_DESC[static_sampler_count]);
		else
			samplers_ = nullptr;
		static_sampler_count_ = static_sampler_count;
		uninit_static_sampler_count_ = 0;
	}
	void RootSignature::CreateRootSignature(Shader& shader)
	{

	}
	void RootSignature::InitStaticSampler(int registerid, D3D12_SAMPLER_DESC desc, D3D12_SHADER_VISIBILITY visibility)
	{
		//ASSERT(m_NumInitializedStaticSamplers < m_NumSamplers);
		D3D12_STATIC_SAMPLER_DESC& StaticSamplerDesc = samplers_[uninit_static_sampler_count_++];

		StaticSamplerDesc.Filter = desc.Filter;
		StaticSamplerDesc.AddressU = desc.AddressU;
		StaticSamplerDesc.AddressV = desc.AddressV;
		StaticSamplerDesc.AddressW = desc.AddressW;
		StaticSamplerDesc.MipLODBias = desc.MipLODBias;
		StaticSamplerDesc.MaxAnisotropy = desc.MaxAnisotropy;
		StaticSamplerDesc.ComparisonFunc = desc.ComparisonFunc;
		StaticSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		StaticSamplerDesc.MinLOD = desc.MinLOD;
		StaticSamplerDesc.MaxLOD = desc.MaxLOD;
		StaticSamplerDesc.ShaderRegister = registerid;
		StaticSamplerDesc.RegisterSpace = 0;
		StaticSamplerDesc.ShaderVisibility = visibility;
	}
	void RootSignature::Finalize(const wchar_t* Name, D3D12_ROOT_SIGNATURE_FLAGS flag)
	{
		
		for (size_t i = 0; i < paramenterCount_; i++)
		{
			descriptorTableSizeArray_[i] = paraments_[i].DescriptorTableSize();
		}
		
		D3D12_ROOT_SIGNATURE_DESC RootDesc;
		RootDesc.Flags = flag;
		RootDesc.NumParameters = paramenterCount_;
		RootDesc.NumStaticSamplers = static_sampler_count_;
		RootDesc.pParameters = (const D3D12_ROOT_PARAMETER*)paraments_.get();
		RootDesc.pStaticSamplers = (const D3D12_STATIC_SAMPLER_DESC*)samplers_.get();

		ComPtr<ID3DBlob> pOutBlob, pErrorBlob;

		ASSERT_SUCCEEDED(D3D12SerializeRootSignature(&RootDesc, D3D_ROOT_SIGNATURE_VERSION_1,pOutBlob.GetAddressOf(), pErrorBlob.GetAddressOf()));

		ASSERT_SUCCEEDED(Graphic::g_Device->CreateRootSignature(1, pOutBlob->GetBufferPointer(), pOutBlob->GetBufferSize(),IID_PPV_ARGS(&rootSignature_)));

		rootSignature_->SetName(Name);
	}
	void RootParameter::InitAsConstBufferView(D3D12_SHADER_VISIBILITY visibility,UINT registerSpace,UINT shaderSpace)
	{
		parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		parameter_.ShaderVisibility = visibility;
		parameter_.Descriptor.RegisterSpace = registerSpace;
		parameter_.Descriptor.ShaderRegister = shaderSpace;
	}
}