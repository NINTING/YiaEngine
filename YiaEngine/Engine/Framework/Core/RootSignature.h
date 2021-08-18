#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <vector>

#include<memory>
namespace YiaEngine::Graphic
{
	class RootParament
	{
	public:
		RootParament()
		{
			parameter_.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)-1;
		}
		void InitAsDescriptorTable(int range_count, D3D12_SHADER_VISIBILITY visblity = D3D12_SHADER_VISIBILITY_ALL)
		{
			parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			parameter_.ShaderVisibility = visblity;
			parameter_.DescriptorTable.NumDescriptorRanges = range_count;
			parameter_.DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[range_count];
		}
		void SetTableRange(UINT index, UINT descriptor_count, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT ragister,UINT space = 0)
		{
			auto range_ptr =  const_cast<D3D12_DESCRIPTOR_RANGE*>(&parameter_.DescriptorTable.pDescriptorRanges[index]);
				
			range_ptr->BaseShaderRegister = ragister;
			range_ptr->NumDescriptors = descriptor_count;
			range_ptr->RangeType = type;
			range_ptr->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
			range_ptr->RegisterSpace = space;
		}
		~RootParament()
		{
			Clear();
		}
		void Clear()
		{
			if (parameter_.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
				delete[] parameter_.DescriptorTable.pDescriptorRanges;
			parameter_.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)-1;
		}
		const D3D12_ROOT_PARAMETER& operator ()()const { return parameter_; }
	private:
		D3D12_ROOT_PARAMETER parameter_;
	};


	class RootSignature
	{
		public:
		void Reset(int root_parament_count,int static_sampler_count);
		void InitStaticSampler(int registerid, D3D12_SAMPLER_DESC desc, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
		ID3D12RootSignature* root_signature() { return root_signature_.Get(); }
		void Finalize(const wchar_t* name, D3D12_ROOT_SIGNATURE_FLAGS flag);
	public:
		RootParament& operator[](int index) { return paraments_[index]; }
		const RootParament& operator[](int index) const { return paraments_[index]; }

	private:

		std::unique_ptr<RootParament[]>paraments_;
		std::unique_ptr<D3D12_STATIC_SAMPLER_DESC[]>samplers_;
		int static_sampler_count_;
		int paramenter_count_;
		int uninit_static_sampler_count_;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> root_signature_;
	};
}