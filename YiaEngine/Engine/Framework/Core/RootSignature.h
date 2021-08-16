#pragma once
#include<d3d12.h>

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

	};
}