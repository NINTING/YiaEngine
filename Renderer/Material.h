#pragma once

#include"Core/YiaGraphic.h"

#include"Common/YiaMath.h"

namespace YiaEngine
{
	namespace Graphic
	{

		struct ResourceData
		{
			ShaderResourceType Type;
			uint8_t* BufferData;
			UINT Size;
		};

		class Material
		{
		public:
			Material()=default;
			Material(const char* name, Shader* shader);
			void InitMaterial(const char* name, Shader* shader);
			void InitMaterialResource();
			
			Shader& GetShader() { return *pShader_; };
			int GetResourceCount() { return  resourceDatas_.size(); };
			ResourceData& GetResourceData(int rootIndex);
		public:
			void SetMatrix(const char* name, const Math::Mat4x4f& mat);

		private:

			void AddInputLayout();

			void AddVariable();

			

		private:
			Shader* pShader_;

			RootSignature resourceLayout_;

			std::string name_;
			std::map<std::string,ShaderVariable> varibalData_;
			std::vector <ResourceData> resourceDatas_;
		};
	}
}


