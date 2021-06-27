#ifndef SCENE_MATERIAL_OBJECT_H
#define SCENE_MATERIAL_OBJECT_H


#include<cas>

#include "BaseSceneObject.h"
#include"TextureObject.h"
#include"SceneEnum.h"

namespace YiaEngine
{
	namespace Scene {
		//一个混合参数(value,texture)
		//对于一个材质属性diffuse
		//既可以是color或者texture
		//the VariatParam have (value,texture)
		//for a material attribute diffuse
		//it can be color or(and) texture

		struct MaterialParam
		{
			virtual ~MaterialParam()
			{
			}
		};
		
		
		template<typename T>
		struct VariantParam : MaterialParam
		{
		public:
			VariantParam() = default;
			VariantParam(const T& value) :Value(value) {}
			
			VariantParam(const VariantParam&) = default;
			VariantParam(VariantParam&&) = default;

			
			virtual ~VariantParam() {}

			T Value;	
			//	T value_;
			//	std::shared_ptr<TextureObject>texture_;
		};
		template<>
		struct VariantParam<TextureObject> : MaterialParam
		{
			VariantParam() = default;
			VariantParam(const std::shared_ptr<TextureObject>& texture) :Texture(texture) {}
			VariantParam(std::shared_ptr<TextureObject>&& texture) :Texture(std::move(texture)) {}
			VariantParam(const VariantParam&) = default;
			VariantParam(VariantParam&&) = default;
			
			virtual ~VariantParam() {}

			std::shared_ptr<TextureObject>Texture;
		};

		using ColorParam = VariantParam<Color>;
		using FloatParam = VariantParam<float>;
		using BoolParam  = VariantParam<bool>;
		using TextureParam = VariantParam <TextureObject>;

		using Value = float;
		
		class MaterialObject :public BaseSceneObject
		{
		public:
			virtual ~MaterialObject()
			{
			}
			
			template<typename T>
			T GetParam(std::string param_name)
			{
				if (params_.find(param_name) == params_.end())
					return T{};
				return *std::dynamic_pointer_cast<T>(params_[param_name]);
			}
			template<typename T>
			void SetParam(std::string param_name,const T& param)
			{
				params_[param_name] = 
					std::static_pointer_cast<MaterialParam>(std::make_shared<T>(param));
			}
			template<typename T>
			void SetParam(std::string param_name,const std::shared_ptr<T>& param_ptr)
			{
				params_[param_name] =std::static_pointer_cast<MaterialParam>(param_ptr);
			}
			void SetName(const std::string& material_name)
			{
				name_= material_name;
			}
			std::string Name()
			{
				return name_;
			}
		protected:
			MaterialObject() :BaseSceneObject(SceneObjectType::kMaterialObject) {};
			std::string name_;
			bool two_side_;
			std::unordered_map<std::string,std::shared_ptr<MaterialParam>> params_;
		};
		class PBRMaterial :public MaterialObject
		{
		public:
			PBRMaterial()
			{
				params_["Diffuse"]				= std::make_shared<ColorParam>();
				params_["Diffuse_Tex"]			= std::make_shared<TextureParam>();
				params_["Specular"]				= std::make_shared<ColorParam>();
				params_["Specular_power"]		= std::make_shared<FloatParam>();
				params_["Emission"]				= std::make_shared<ColorParam>();
				params_["Opacity"]				= std::make_shared<ColorParam>();
				params_["Transparency"]			= std::make_shared<ColorParam>();
				params_["Norma_map"]			= std::make_shared<TextureParam>();
				params_["Height_map"]			= std::make_shared<TextureParam>();
				params_["Height_scale"]			= std::make_shared<FloatParam>();
				params_["Occlution"]			= std::make_shared<TextureParam>();
				params_["Roughness"]			= std::make_shared<FloatParam>();
				params_["Metalness"]			= std::make_shared<FloatParam>();
				params_["Clearcoat"]			= std::make_shared<ColorParam>();
				params_["Clearcoat_roughness"]	= std::make_shared<FloatParam>();
				params_["Sheen"]				= std::make_shared<ColorParam>();
				params_["Sheen_roughness"]		= std::make_shared<FloatParam>();
				params_["Ior"]					= std::make_shared<ColorParam>();
			}
			PBRMaterial(const PBRMaterial&) = default;
			//PBRMaterial(const PBRMaterial&&) = default;
			PBRMaterial& operator=(const PBRMaterial&) = default;
			PBRMaterial& operator=(PBRMaterial&&) = default;
			
			

			virtual ~PBRMaterial(){}

		};
	}//Scene
}//YiaEngine

#endif