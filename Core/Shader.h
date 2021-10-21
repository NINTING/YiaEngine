#pragma once
#include"Graphic.h"
#include"Common/DataFormat.h"
namespace YiaEngine
{
	namespace Graphic
	{
		struct VertextAttribute
		{
			char* SemanticName;
			int SemanticIndex;
			int NameSize;
			DataFormat format;

			
		};
		struct VertexInputLayout
		{
			int AttributesCount;
			VertextAttribute* Attrs;
		};

		struct ShaderReflect
		{
			char* pNamePool;
			UINT PoolSize;
			VertexInputLayout VertexInput;
		};
		enum ShaderStage
		{
			Shader_Stage_None,
			Shader_Stage_Vertex,
			Shader_Stage_Pixel,
			Shader_Stage_Count,
		};
		struct Shader
		{
			ShaderStage Stage;
			ComPtr<IDxcBlob> ShaderBlob;
			ShaderReflect Reflect;
		};
	}
}
