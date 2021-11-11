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
	void RootSignature::CreateRootSignature(const wchar_t*name, Shader& shader,int staticSamplerCount)
	{
		std::map<const char*, int> resourceMap;
		std::vector<ShaderResource>resources;
		UINT rootParamCount = 0;
		for (size_t i = 0; i < Shader_Stage_Count; i++)
		{
			if (shader.Stages & (1 << i))
			{
				ShaderReflect& reflect = shader.Reflect[i];
				for (size_t j = 0; j < reflect.ResourcesSize; j++)
				{
					auto it = resourceMap.find(reflect.Resources[i].Name);
					if (it == resourceMap.end())
					{
						resources.push_back(reflect.Resources[i]);
						resourceMap[reflect.Resources[i].Name] = resources.size() - 1;
					}
					else
					{
						 resources[it->second].Stage |= reflect.Resources[i].Stage;
					}
				}

			}

		}


		//static sample�洢
		//texture��ǩ���ķ���
		std::vector<int> descriptorTableRange;
		std::vector<int> rootConstRange;
		int rootParamentCount = 0;
		for (size_t i = 0; i < resources.size(); i++)
		{
			ShaderResource& res = resources[i];
			D3D12_SHADER_VISIBILITY visibility = ToDx12ShaderVisiblity(res.Stage);
			
			if (res.Type == ShaderResourceType::ShaderResourceType_ConstBuffer)
			{
				rootConstRange.push_back(i);
				rootParamentCount++;
				
			}
			else if (res.Type == ShaderResourceType::ShaderResourceType_Sampler)
			{
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

				InitStaticSampler(res.Reg, sampler_desc, visibility);
			}
			else if (res.Type == ShaderResourceType::ShaderResourceType_Texture)
			{
				descriptorTableRange.push_back(i);
				
			}
			else
			{
				YIA_ERROR("��δ֧�ִ����������͵�RootParamter {0}",res.Name);
			}
		}
		if (descriptorTableRange.size())
		{
			rootParamentCount++;
		}
		Reset(rootParamentCount, staticSamplerCount);
		int rootParamIndex = 0;
#pragma region RootConst
		for (rootParamIndex = 0; rootParamIndex < rootConstRange.size(); rootParamIndex++)
		{
			ShaderResource& res = resources[rootConstRange[rootParamIndex]];
			

			D3D12_SHADER_VISIBILITY visibility = ToDx12ShaderVisiblity(res.Stage);
			paraments_[rootParamIndex].InitAsConstBufferView(visibility, res.Reg);

			locationMap[rootConstRange[rootParamIndex]] = { rootParamIndex, 0 };
		}
#pragma endregion

		
#pragma region DescriptorTable
		if (descriptorTableRange.size())
		{
			std::sort(descriptorTableRange.begin(), descriptorTableRange.end(), [](const ShaderResource& lhs, const ShaderResource& rhs) {return lhs.Reg > rhs.Reg; });
			std::sort(descriptorTableRange.begin(), descriptorTableRange.end(), [](const ShaderResource& lhs, const ShaderResource& rhs) {return lhs.Type > rhs.Type; });
			
			ShaderStage stages;
			
			for (size_t i = 0; i < descriptorTableRange.size(); i++)
			{
				ShaderResource& res = resources[descriptorTableRange[i]];
				stages |= res.Stage;
			}
			paraments_[rootParamIndex].InitAsDescriptorTable(descriptorTableRange.size(),ToDx12ShaderVisiblity(stages));

			for (size_t i = 0; i < descriptorTableRange.size(); i++)
			{
				ShaderResource& res = resources[descriptorTableRange[i]];
				paraments_[rootParamIndex].SetTableRange(i, res.Count, ToDx12RangeType(res.Type),res.Reg);

				locationMap[descriptorTableRange[i]] = { rootParamIndex,i };
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
	void RootParameter::InitAsConstBufferView(D3D12_SHADER_VISIBILITY visibility,UINT registerSpace,UINT shaderSpace)
	{
		parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		parameter_.ShaderVisibility = visibility;
		parameter_.Descriptor.RegisterSpace = registerSpace;
		parameter_.Descriptor.ShaderRegister = shaderSpace;
	}
}