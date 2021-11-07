#include "pch.h"
#include "Material.h"
#include"Common/Utility.h"

namespace YiaEngine
{
	namespace Graphic
	{
		Material::Material(const char* name, Shader* shader) :name_(name),pShader_(shader)
		{
			
			wchar_t* wname = new wchar_t[strlen(name) + 1];
			Char2Wchar(name, wname);
			resourceLayout_.CreateRootSignature(wname, *shader, 0);
			delete wname;
			InitMaterialResource();
			AddInputLayout();
			AddVariable();
		}
		void Material::InitMaterial(const char* name, Shader* shader)
		{
			name_ = name;
			pShader_ = shader;
			std::string rootName(name);
			rootName += "RootSignature";
			wchar_t* wname = new wchar_t[rootName.size()+1];
			Char2Wchar(rootName.c_str(), wname);
			resourceLayout_.CreateRootSignature(wname, *shader, 0);
			delete wname;

			InitMaterialResource();
			AddInputLayout();
			AddVariable();
		}
		void Material::InitMaterialResource()
		{
			std::map<const char*, int> resourceMap;
			std::vector<ShaderResource>resources;
			UINT rootParamCount = 0;
			for (size_t i = 0; i < Shader_Stage_Count; i++)
			{
				if (pShader_->Stages & (1 << i))
				{
					ShaderReflect& reflect = pShader_->Reflect[i];
					for (size_t j = 0; j < reflect.ResourcesSize; j++)
					{
						auto it = resourceMap.find(reflect.Resources[i].Name);
						if (it == resourceMap.end())
						{
							resources.push_back(reflect.Resources[i]);
							resourceMap[reflect.Resources[i].Name] = resources.size();
							for (size_t varIndex = 0; varIndex < reflect.VariablesSize; varIndex++)
							{
								reflect.Variables[varIndex].ResourceIndex = resources.size() - 1;
							}
						}
						else
						{
							resources[it->second].Stage |= reflect.Resources[i].Stage;
						}
					}
				}
			}
			for (size_t i = 0; i < resources.size(); i++)
			{
				ShaderResource& res = resources[i];
				if (res.Type == ShaderResourceType::ConstBuffer)
				{
					ResourceData res;
					res.BufferData = new uint8_t[resources[i].Size];
					res.Size = resources[i].Size;
					res.Type = resources[i].Type;
					resourceDatas_.push_back(res);
				}
				else
				{
					YIA_ERROR("尚未支持创建其他类型的RootParamter {0}", res.Name);
				}
			}
		}
		void Material::SetMatrix(const char* name,const Math::Mat4x4f& mat)
		{
			ResourceData& data = resourceDatas_[varibalData_[name].ResourceIndex];
			memcpy(data.BufferData + varibalData_[name].Offset, (uint8_t*)&mat, sizeof(Math::Mat4x4f));
		}
		void Material::AddInputLayout()
		{
			D3D12_INPUT_LAYOUT_DESC desc;

			desc.NumElements = pShader_->Reflect[0].VertexInput.AttributesCount;
			auto& vertexInput = pShader_->Reflect[0].VertexInput;
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
		}
		void Material::AddVariable()
		{
			for (size_t i = 0; i < Shader_Stage_Count; i++)
			{
				ShaderReflect& reflect = pShader_->Reflect[i];

				for (size_t j = 0; j < reflect.VariablesSize; j++)
				{
					std::string varName = reflect.Variables[j].Name;

					if (varibalData_.find(varName)!= varibalData_.end())
					{
						YIA_ERROR("同名变量{0}",varName);
					}
					else
						varibalData_[varName] = reflect.Variables[j];
				} 
			}
		}

		ResourceData& Material::GetResourceData(int rootIndex)
		{
			return resourceDatas_[rootIndex];
		}
	}
}