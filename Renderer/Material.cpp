#include "pch.h"
#include "Material.h"
#include"Common/Utility.h"

namespace YiaEngine
{
	namespace Graphic
	{
		void FilterDuplicateShaderResource(const Shader& shader, std::vector<ShaderResource>& outResources);
		Material::Material(const char* name,std::shared_ptr<Shader>shader) :name_(name),pShader_(shader)
		{
			
			wchar_t* wname = new wchar_t[strlen(name) + 1];
			Char2Wchar(name, wname);


			std::vector<ShaderResource> outResources;
			FilterDuplicateShaderResource(*shader, outResources);

			resourceLayout_.CreateRootSignature(wname, outResources);
			delete wname;
			InitMaterialResource(outResources);
			AddInputLayout();
			AddVariable();
		}
		void FilterDuplicateShaderResource(const Shader& shader, std::vector<ShaderResource>& outResources)
		{
			std::map<const char*, int> resourceMap;
		
			UINT rootParamCount = 0;
			for (size_t i = 0; i < Shader_Stage_Count; i++)
			{
				if (shader.Stages & (1 << i))
				{
					const ShaderReflect& reflect = shader.Reflect[i];
					for (size_t j = 0; j < reflect.ResourcesSize; j++)
					{
						auto it = resourceMap.find(reflect.Resources[j].Name);
						if (it == resourceMap.end())
						{
							outResources.push_back(reflect.Resources[j]);
							resourceMap[reflect.Resources[j].Name] = outResources.size() - 1;
						}
						else
						{
							outResources[it->second].Stage |= reflect.Resources[j].Stage;
						}
					}
				}
			}
		}
		void Material::InitMaterial(const char* name, std::shared_ptr<Shader>shader)
		{
			name_ = name;
			pShader_ = shader;
			std::string rootName(name);
			rootName += "RootSignature";
			wchar_t* wname = new wchar_t[strlen(name) + 1];
			Char2Wchar(name, wname);


			std::vector<ShaderResource> outResources;
			FilterDuplicateShaderResource(*shader, outResources);

			resourceLayout_.CreateRootSignature(wname, outResources);
			delete wname;
			InitMaterialResource(outResources);
			AddInputLayout();
			AddVariable();
		}
		void Material::InitMaterialResource(const std::vector<ShaderResource>& resources)
		{
			UINT rootParamCount = 0;
			
			for (size_t i = 0; i < resources.size(); i++)
			{
				const ShaderResource& res = resources[i];
				dirtyFlags_.push_back(true);

				ResourceData resData;
				resData.Type = resources[i].Type;
				resData.Freq = resources[i].RegisterSpace;
				if (res.Type == ShaderResourceType::ShaderResourceType_ConstBuffer)
				{
					
					resData.Data = new uint8_t[resources[i].Size];
					resData.location = resourceLayout_.GetResourceLocation(i);
					resData.Size = resources[i].Size;
					
				
					resData.Freq = resources[i].RegisterSpace;
			

					//更新频率为 PerDraw将使用临时分配的CBV,而不主动分配
					if (resData.Freq!=ResourceFrequenceType::UPDATE_FREQ_PER_DRAW)
					{
						resData.BufferIndex = cbvList.size();
						cbvList.push_back(std::shared_ptr<GpuBuffer>(new GpuBuffer()));
						cbvList.back()->Create(resources[i].Name, resData.Size);
					}
					resourceDatas_.push_back(resData);
				}
				else if(res.Type == ShaderResourceType::ShaderResourceType_Texture)
				{
		
					textureMap[res.Name] = resourceDatas_.size();
					resData.Count = res.Count;
					resData.Data = nullptr;
					resData.location = resourceLayout_.GetResourceLocation(i);
					resourceDatas_.push_back(resData);
				}
				else if (res.Type == ShaderResourceType::ShaderResourceType_Sampler)
				{
			
					resourceDatas_.push_back(resData);
				}
				else
				{
					YIA_ERROR("尚未支持创建其他类型的RootParamter {0}", res.Name);
				}
			}
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
		void Material::AddTextureResource(){
		
		}

		void Material::SetMatrix(const char* name, const Math::Mat4x4f& mat)
		{
			ResourceData& data = resourceDatas_[varibalData_[name].ResourceIndex];
			dirtyFlags_[varibalData_[name].ResourceIndex] = true;
			memcpy(data.Data + varibalData_[name].Offset, (uint8_t*)&mat, sizeof(Math::Mat4x4f));
		}
		void Material::SetTexture(const char* name, const Texture& texture)
		{
			ResourceData& resourceData = resourceDatas_[textureMap[name]];
			resourceData.Data = (uint8_t*)(&texture);

			dirtyFlags_[varibalData_[name].ResourceIndex] = true;
		}

		ResourceData& Material::GetResourceData(int rootIndex)
		{
			return resourceDatas_[rootIndex];
		}
		GpuBuffer& Material::GetCbvData(int rootIndex)
		{
			assert(resourceDatas_[rootIndex].Type == ShaderResourceType::ShaderResourceType_ConstBuffer);
			return *cbvList[resourceDatas_[rootIndex].BufferIndex];
		}
		RootSignature& Material::GetRootSignature()
		{
			return resourceLayout_;
		}
		void Material::Finalize()
		{
			for (size_t i = 0; i < dirtyFlags_.size(); i++)
			{
				if (dirtyFlags_[i])
				{
					if (resourceDatas_[i].Type == ShaderResourceType::ShaderResourceType_ConstBuffer)
					{
						if (resourceDatas_[i].Freq!=ResourceFrequenceType::UPDATE_FREQ_PER_DRAW)
						{
							CommandContext::UpdateBufferData(*cbvList[resourceDatas_[i].BufferIndex], resourceDatas_[i].Data);
						}
					
					}
					dirtyFlags_[i] = false;
				}
			}
		}
	}
}