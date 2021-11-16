#include "pch.h"
#include "Shader.h"
#include "Common/Utility.h"
#include<d3d12shader.h>
#include"ResourceLoder.h"
namespace YiaEngine
{
	namespace Graphic
	{
		D3D12_SHADER_VISIBILITY ToDx12ShaderVisiblity(ShaderStage stage)
		{
			int stageCount = 0;
			D3D12_SHADER_VISIBILITY ret;
			if (stage & ShaderStage::Shader_Stage_Vertex)
			{
				ret = D3D12_SHADER_VISIBILITY_VERTEX;
				stageCount++;
			}

			if (stage & ShaderStage::Shader_Stage_Pixel)
			{
				ret = D3D12_SHADER_VISIBILITY_PIXEL;
				stageCount++;
			}
			return stageCount > 1 ? D3D12_SHADER_VISIBILITY_ALL : ret;
		}

		D3D12_DESCRIPTOR_RANGE_TYPE ToDx12RangeType(ShaderResourceType stage)
		{
			switch (stage)
			{
			case YiaEngine::Graphic::ShaderResourceType_ConstBuffer:
				return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
			case YiaEngine::Graphic::ShaderResourceType_Texture:
				return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			}
		}

		std::string ShaderLibrary::Prefix = "../Asset/Shader/";
		std::string ShaderLibrary::CompletePrefix = "D:/Yia/YiaEngine/Asset/Shader/";
		std::map<std::string, std::shared_ptr<Shader>> ShaderLibrary::shaderLib_;

		bool ShaderLibrary::LoadShader(const char* name, std::shared_ptr<Shader>& outShader)
		{

			auto it = shaderLib_.find(name);
			if (it != shaderLib_.end())
			{
				outShader = it->second;
			}
			else
			{
				shaderLib_[name] = std::shared_ptr<Shader>(new Shader());
				Graphic::ShaderLoadDesc loadDesc = {};

				std::string filename = name;
#ifdef USE_PIX
				filename = CompletePrefix + name;

#else
				filename = Prefix + name;
#endif
				auto sw = String2WString(filename);

				loadDesc.StageLoadDesc[0] = { sw.c_str(),Graphic::Shader_Stage_Vertex,L"vs_6_0" };
				loadDesc.StageLoadDesc[1] = { sw.c_str(),Graphic::Shader_Stage_Pixel,L"ps_6_0" };

				Graphic::LoadShader(loadDesc, *shaderLib_[name]);
				outShader = shaderLib_[name];
			}
			return true;
		}
	}
}