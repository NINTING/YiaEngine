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
			const wchar_t* EntryPoint;
			const wchar_t* target;
		};
		struct ShaderLoadDesc
		{
			ShaderStageLoadDesc StageLoadDesc[Shader_Stage_Count];
		};
		void LoadShader(const ShaderLoadDesc& desc,Shader* shader);
		ComPtr<IDxcBlob> CompilerShaderFromDXC(const wchar_t* path, const wchar_t* entry, const wchar_t* target, ComPtr< ID3D12ShaderReflection > pReflection);

	}
}