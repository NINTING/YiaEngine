#pragma once

#include"Core/YiaGraphic.h"

#include"Common/YiaMath.h"
#include"Texture.h"
namespace YiaEngine
{
	namespace Graphic
	{

		struct ResourceData
		{
			ShaderResourceType Type;
			uint8_t* Data;
			SignatureLocation location;
			UINT Count;
			UINT Size;
			UINT BufferIndex;
			ResourceFrequenceType Freq;
		};

		class Material
		{
		public:
			Material()=default;
			Material(const char* name, std::shared_ptr<Shader>shader);
			void InitMaterial(const char* name, std::shared_ptr<Shader>shader);
			void InitMaterialResource(const std::vector<ShaderResource>& resources);
			
			Shader& GetShader() { return *pShader_; };
			int GetResourceCount() { return  resourceDatas_.size(); };
			ResourceData& GetResourceData(int rootIndex);
			RootSignature& GetRootSignature();
			GpuBuffer& GetCbvData(int rootIndex);
			void Finalize();
		public:
			void SetMatrix(const char* name, const Math::Mat4x4f& mat);

			void SetTexture(const char* name, const Texture& texture);

		private:

			void AddInputLayout();

			void AddVariable();

			void AddTextureResource();

			


		private:
			std::shared_ptr<Shader> pShader_;

			RootSignature resourceLayout_;
			
			std::string name_;
			std::map<std::string,ShaderVariable> varibalData_;
			std::vector <ResourceData> resourceDatas_;
			std::vector<bool> dirtyFlags_;
			std::vector<std::shared_ptr< GpuBuffer>> cbvList;
			std::map<std::string, UINT>textureMap;
		};
	}
}


