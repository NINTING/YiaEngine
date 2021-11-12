#pragma once
#include"Graphic.h"
#include"Common/DataFormat.h"
#include"Common/Model/MeshObject.h"
namespace YiaEngine
{
	namespace Graphic
	{

		enum ShaderResourceType
		{
			ShaderResourceType_ConstBuffer = 0,
			ShaderResourceType_Texture = 2,
			ShaderResourceType_Sampler = 3,
		};
		enum ShaderStage
		{
			Shader_Stage_None = 0,

			Shader_Stage_Vertex = 1,

			Shader_Stage_Pixel = 2,

			Shader_Stage_Count = Shader_Stage_Pixel,
		};
		enum TextureDimension
		{
			TEXTURE_DIM_UNKNOWN = 0,
			TEXTURE_DIM_BUFFER = 1,
			TEXTURE_DIM_TEXTURE1D = 2,
			TEXTURE_DIM_TEXTURE1DARRAY = 3,
			TEXTURE_DIM_TEXTURE2D = 4,
			TEXTURE_DIM_TEXTURE2DARRAY = 5,
			TEXTURE_DIM_TEXTURE2DMS = 6,
			TEXTURE_DIM_TEXTURE2DMSARRAY = 7,
			TEXTURE_DIM_TEXTURE3D = 8,
			TEXTURE_DIM_TEXTURECUBE = 9,
			TEXTURE_DIM_TEXTURECUBEARRAY = 10,
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
			int RegisterSpace;
			int Reg;
			TextureDimension Dimension;
			size_t Size;
			int Count;
			ShaderResourceType Type;
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

		D3D12_SHADER_VISIBILITY ToDx12ShaderVisiblity(ShaderStage stage);
		D3D12_DESCRIPTOR_RANGE_TYPE ToDx12RangeType(ShaderResourceType stage);
	}
}
