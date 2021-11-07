#include "pch.h"
#include "Shader.h"
#include "Utility.h"
#include<d3d12shader.h>
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
			return stage > 1 ? D3D12_SHADER_VISIBILITY_ALL : ret;
		}

	}
}