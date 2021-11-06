#pragma once
#include"Graphic.h"
#include"Common/DataFormat.h"
#include"Common/Model/MeshObject.h"
namespace YiaEngine
{
	namespace Graphic
	{

		enum ResourceType
		{
			ConstBuffer = 0,
			Texture,
			Sample,
		};
		enum ShaderStage
		{
			Shader_Stage_None = 0,

			Shader_Stage_Vertex = 1,

			Shader_Stage_Pixel = 2,

			Shader_Stage_Count = 3,
		};
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
		struct ShaderResource
		{
			char* Name;
			UINT NameSize;
			int registerSpace;
			size_t size;
			ResourceType Type;
			ShaderStage Stage;
		};
		struct ShaderVariable
		{
			char* Name;
			UINT NameSize;
			size_t Size;
			size_t Offset;
			UINT ResourceIndex;
			
		};
		struct ShaderReflect
		{
			char* pNamePool;
			UINT PoolSize;
			VertexInputLayout VertexInput;

			ShaderResource* Resources;
			UINT ResourcesSize;

		ShaderVariable* Variables;
			UINT VariablesSize;
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
