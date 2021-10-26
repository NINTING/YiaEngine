#pragma once
#include"Graphic.h"
#include"Common/DataFormat.h"
#include"Common/Model/MeshObject.h"
namespace YiaEngine
{
	namespace Graphic
	{
		struct VertextInputAttribute
		{
			char* SemanticName;
			int SemanticIndex;
			int NameSize;
			DataFormate format;
			VertexAttributeEnum Attribute;

		};
		struct VertexInputLayout
		{
			int AttributesCount;
			VertextInputAttribute* Attrs;
		};

		struct ShaderReflect
		{
			char* pNamePool;
			UINT PoolSize;
			VertexInputLayout VertexInput;
		};
		enum ShaderStage
		{
			Shader_Stage_None = 0,

			Shader_Stage_Vertex = 1,

			Shader_Stage_Pixel = 2,

			Shader_Stage_Count = 3,
		};
		static inline ShaderStage operator | (ShaderStage a, ShaderStage b) { return (ShaderStage)((UINT)(a) | (UINT)(b)); }
		static inline ShaderStage operator & (ShaderStage a, ShaderStage b) { return (ShaderStage)((UINT)(a) & (UINT)(b)); }
		static inline ShaderStage operator &= (ShaderStage& a, ShaderStage b) { return a =a&b; }
		static inline ShaderStage operator |= (ShaderStage& a, ShaderStage b) { return a = a | b; }

		struct Shader
		{
			ShaderStage Stages;
			ComPtr<IDxcBlob> ShaderBlob[Shader_Stage_Count];
			ShaderReflect Reflect[Shader_Stage_Count];
		};
	}
}
