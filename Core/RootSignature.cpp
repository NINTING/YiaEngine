#pragma once
#include"pch.h"
#include"Utility.h"
#include"Core/Graphic.h"
#include "RootSignature.h"
#include"Common/Logger.h"
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
	void RootSignature::CreateRootSignature(const wchar_t*name, const std::vector<ShaderResource>& resources)
	{
		//static sample存储
		//texture根签名的分配
		std::vector<UINT> descriptorTableRanges[ResourceFrequenceType::UPDATE_FREQ_COUNT];
		std::vector<UINT> rootConstRanges[ResourceFrequenceType::UPDATE_FREQ_COUNT];
		std::vector<UINT> samplerRanges[ResourceFrequenceType::UPDATE_FREQ_COUNT];
		int rootParamentCount = 0;
		UINT samplerCount = 0;
		for (size_t i = 0; i < resources.size(); i++)
		{
			const ShaderResource& res = resources[i];
			D3D12_SHADER_VISIBILITY visibility = ToDx12ShaderVisiblity(res.Stage);
			
			if (res.Type == ShaderResourceType::ShaderResourceType_ConstBuffer)
			{
				rootConstRanges[res.RegisterSpace].push_back(i);
				rootParamentCount++;
				
			}
			else if (res.Type == ShaderResourceType::ShaderResourceType_Sampler)
			{
				samplerRanges[res.RegisterSpace].push_back(i);
				samplerCount++;
			}
			else if (res.Type == ShaderResourceType::ShaderResourceType_Texture)
			{
				descriptorTableRanges[res.RegisterSpace].push_back(i);
			}
			else
			{
				YIA_ERROR("尚未支持创建其他类型的RootParamter {0}",res.Name);
			}
		}
		for (size_t i = 0; i < ResourceFrequenceType::UPDATE_FREQ_COUNT; i++)
		{
			if (descriptorTableRanges[i].size())
			{
				rootParamentCount++;
			}
		}
		
		Reset(rootParamentCount, samplerCount);
		UINT rootParamIndex = 0;
#pragma region RootConst
	
		for (int i = ResourceFrequenceType::UPDATE_FREQ_COUNT - 1; i >= 0 ; i--)
		{
			std::vector<UINT>& rootConstRange = rootConstRanges[i];

			for (int j = 0; j < rootConstRange.size(); j++)
			{
				const ShaderResource& res = resources[rootConstRange[j]];

				D3D12_SHADER_VISIBILITY visibility = ToDx12ShaderVisiblity(res.Stage);
				paraments_[rootParamIndex].InitAsConstBufferView(visibility, res.Reg, res.RegisterSpace);
				locationMap[rootConstRange[j]] = SignatureLocation(rootParamIndex, 0);
				++rootParamIndex;
			}
		}
		
#pragma endregion

		
#pragma region DescriptorTable
		for (int i = ResourceFrequenceType::UPDATE_FREQ_COUNT - 1; i >= 0; i--)
		{
			std::vector<UINT>& descriptorTableRange = descriptorTableRanges[i];
			if (descriptorTableRange.size())
			{
				std::sort(descriptorTableRange.begin(), descriptorTableRange.end(), [resources](UINT lhs, UINT rhs) {return resources[lhs].Reg > resources[rhs].Reg; });
				std::sort(descriptorTableRange.begin(), descriptorTableRange.end(), [resources](UINT lhs, UINT rhs) {return resources[lhs].Type > resources[rhs].Type;  });

				ShaderStage stages;

				for (size_t i = 0; i < descriptorTableRange.size(); i++)
				{
					const ShaderResource& res = resources[descriptorTableRange[i]];
					stages |= res.Stage;
				}
				paraments_[rootParamIndex].InitAsDescriptorTable(descriptorTableRange.size(), ToDx12ShaderVisiblity(stages));

				for (UINT i = 0; i < descriptorTableRange.size(); i++)
				{
					const ShaderResource& res = resources[descriptorTableRange[i]];
					paraments_[rootParamIndex].SetTableRange(i, res.Count, ToDx12RangeType(res.Type), res.Reg, res.RegisterSpace);

					locationMap[descriptorTableRange[i]] = SignatureLocation(rootParamIndex, i);
				}

				rootParamIndex++;
			}
			
			std::vector<UINT>& samplerRange = samplerRanges[i];
			for (int i = 0; i < samplerRange.size(); i++)
			{
				const ShaderResource& res = resources[samplerRange[i]];
				D3D12_SAMPLER_DESC sampler_desc;
				sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				sampler_desc.MinLOD = 0;
				sampler_desc.MaxLOD = 0;
				sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
				sampler_desc.MipLODBias = 0;
				sampler_desc.MaxAnisotropy = 0;

				InitStaticSampler(res.Reg, sampler_desc, ToDx12ShaderVisiblity(res.Stage));
			}
		}
		
#pragma endregion
		Finalize(name, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
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
	void RootParameter::InitAsConstBufferView(D3D12_SHADER_VISIBILITY visibility,UINT reg,UINT shaderSpace)
	{
		parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		parameter_.ShaderVisibility = visibility;
		parameter_.Descriptor.RegisterSpace = shaderSpace ;
		parameter_.Descriptor.ShaderRegister = reg;
	}
}