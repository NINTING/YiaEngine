#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <vector>

#include<memory>
#include"Shader.h"
namespace YiaEngine::Graphic
{



	class RootParameter
	{
	public:
		RootParameter()
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
		void InitAsConstBufferView(D3D12_SHADER_VISIBILITY visibility, UINT registerSpace, UINT shaderSpace = 0);
		//void InitAsConst()
		void SetTableRange(UINT index, UINT descriptor_count, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT ragister,UINT space = 0)
		{
			auto range_ptr =  const_cast<D3D12_DESCRIPTOR_RANGE*>(&parameter_.DescriptorTable.pDescriptorRanges[index]);
				
			range_ptr->BaseShaderRegister = ragister;
			range_ptr->NumDescriptors = descriptor_count;
			range_ptr->RangeType = type;
			range_ptr->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
			range_ptr->RegisterSpace = space;
		}
		bool IsDescriptorTable() const
		{
			return parameter_.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		}
		UINT DescriptorTableSize()const
		{
			if (!IsDescriptorTable())
				return 0;
			size_t rangeNum = parameter_.DescriptorTable.NumDescriptorRanges;
			auto ranges = parameter_.DescriptorTable.pDescriptorRanges;
			UINT ret = 0;
			for (size_t i = 0; i < rangeNum; i++)
			{
				ret += ranges[i].NumDescriptors;
			}
			return ret;
		}
		~RootParameter()
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

	struct SignatureLocation
	{
		SignatureLocation() = default;
		SignatureLocation(UINT rootIndex, UINT offset) :RootIndex(rootIndex), Offset(offset){}
		UINT RootIndex;
		UINT Offset;
	};
	/*
		64 DWORD ¿Õ¼ä
		Root Constants			= 1 DWORD * numConstant
		Root Descriptor Table	= 1DWORD
		Root Descriptor			= 2DWORD
	*/
	class RootSignature
	{
	public:
		//const ID3D12RootSignature* root_signature() const { return root_signature_.Get(); }const ID3D12RootSignature* root_signature() const { return root_signature_.Get(); }
		//ID3D12RootSignature* RootSignaturePtr()  { return rootSignature_.Get(); }
		ID3D12RootSignature* RawRootSignature() const { return rootSignature_.Get(); }
		void Finalize(const wchar_t* Name, D3D12_ROOT_SIGNATURE_FLAGS flag);
		void InitStaticSampler(int registerid, D3D12_SAMPLER_DESC desc, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
		void Reset(int root_parament_count,int static_sampler_count);
		void CreateRootSignature(const wchar_t* name, Shader& shader, int staticSamplerCount);
		
	public:
		const RootParameter& operator[](int index) const { return paraments_[index]; }
		size_t ParamenterCount()const { return paramenterCount_; }
		RootParameter& operator[](int index) { return paraments_[index]; }
		SignatureLocation GetResourceLocation(int i) { return locationMap[i]; }
	private:

		size_t descriptorTableSizeArray_[16];
		UINT paramenterCount_;
		UINT static_sampler_count_;
		size_t uninit_static_sampler_count_;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
		std::unique_ptr<D3D12_STATIC_SAMPLER_DESC[]>samplers_;
		std::unique_ptr<RootParameter[]>paraments_;
		std::map<int, SignatureLocation> locationMap;
	};
}