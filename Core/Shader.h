#pragma once
#include"Graphic.h"

namespace YiaEngine
{
	namespace Graphic
	{
		struct VertextAttribute
		{

		};
		struct VertexInputLayout
		{
			int AttributesCount;
			VertextAttribute Attrs[15];
		};



		class ShaderReflect
		{
			VertexInput;
		};
		class Shader
		{
		public:
			
			static void CompilerShader(const wchar_t* path,  Shader& Shader);
			static Shader s_DefaultShader;
		public:
			const UINT8* VsBufferPointer() const{ return (UINT8*)vsBlob_->GetBufferPointer(); };
			UINT VsBufferSize() const { return vsBlob_->GetBufferSize(); };
			const UINT8* PsBufferPointer()const { return (UINT8*)psBlob_->GetBufferPointer(); };
			UINT PsBufferSize() const { return psBlob_->GetBufferSize(); };
		private:
			static void CompilerShader(const wchar_t* path, const char* entry, const char* target, ID3DBlob** Shader);
			
			static bool  CompilerShaderFromDXC(const wchar_t* path, const wchar_t* entry, const wchar_t* target);


		private:
			ComPtr<ID3DBlob> vsBlob_;
			ComPtr<ID3DBlob> psBlob_;
		};
	}
}
