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
			VariantParam(const T& value) :value_(value) {}
			
			VariantParam(const VariantParam&) = default;
			VariantParam(VariantParam&&) = default;

			VariantParam(std::shared_ptr<TextureObject>&& texture) :texture_(std::move(texture)) {}
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
			BoolParam GetBoolParam(std::string param_name)
			{
				return *std::dynamic_pointer_cast<BoolParam>(param_[param_name]);
			}
		protected:
			MaterialObject() :BaseSceneObject(SceneObjectType::kMaterialObject) {};
			std::string name_;
			bool two_side_;
			std::unordered_map<std::string,std::shared_ptr<MaterialParam>> param_;
		};
		class PBRMaterial :public MaterialObject
		{
		public:
			PBRMaterial()
			{

			}
			PBRMaterial(const PBRMaterial&) = default;
			//PBRMaterial(const PBRMaterial&&) = default;
			PBRMaterial& operator=(const PBRMaterial&) = default;
			PBRMaterial& operator=(PBRMaterial&&) = default;
			
			

			virtual PBRMaterial(){}

			ColorParam Diffuse;
			ColorParam Specular;
			ValueParam Specular_power;
			ColorParam Emission;
			ValueParam Opacity;
			ColorParam Transparency;
			TextureObject Norma_map;
			TextureObject Height_map;
			ValueParam Height_scale;
			TextureObject Occlution;
			ValueParam Roughness;
			ValueParam Metalness;
			//清漆材质
			ColorParam Clearcoat;
			ValueParam Clearcoat_roughness;
			ColorParam Sheen;
			ValueParam Sheen_roughness;
			Value Ior;
		};
	}//Scene
}//YiaEngine

#endif