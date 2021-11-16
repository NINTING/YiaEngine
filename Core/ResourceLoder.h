#pragma once
#include"Graphic.h"
#include"Shader.h"
namespace YiaEngine
{
	namespace Graphic
	{
		

		struct ShaderStageLoadDesc
		{
			const wchar_t* FileName;
			ShaderStage stage = Shader_Stage_None;
			const wchar_t* target;
		};
		struct ShaderLoadDesc
		{
			ShaderStageLoadDesc StageLoadDesc[Shader_Stage_Count];
		};
		void LoadShader(const ShaderLoadDesc& desc,Shader& shader);
		void CompilerShaderFromDXC(const wchar_t* path, const wchar_t* entry, const wchar_t* target, ID3D12ShaderReflection** ppOutReflect, ShaderBinaryData* pOut);

	}
}