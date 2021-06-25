#pragma once





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
		template<typename T>
		class VariantParam
		{

		public:
			VariantParam() = default;
			VariantParam(const T& value) :value_(value) {}
			VariantParam(const std::shared_ptr<TextureObject>& texture) :texture_(texture) {}
			VariantParam(const VariantParam&) = default;
			VariantParam(VariantParam&&) = default;

			VariantParam(std::shared_ptr<TextureObject>&& texture) :texture_(std::move(texture)) {}

			/*const T value() { return value_; }
			void set_value(const T& value) { value_ = value; }

			const std::shared_ptr<TextureObject> texture() { return texture_; }
			void set_texture(std::shared_ptr<TextureObject>& texture) { texture_ = texture; }*/


		private:
			T value_;
			std::shared_ptr<TextureObject>texture_;
			//	T value_;
			//	std::shared_ptr<TextureObject>texture_;
		};
		using ColorParam = VariantParam<Color>;
		using ValueParam = VariantParam<float>;
		using Value = float;

		class MaterialObject :public BaseSceneObject
		{

		protected:
			MaterialObject() :BaseSceneObject(SceneObjectType::kMaterialObject) {};
			std::string name_;
			bool two_side_;
		};
		class PBRMaterial :public MaterialObject
		{
		public:
			PBRMaterial() = default;
			PBRMaterial(const PBRMaterial&) = default;
			//PBRMaterial(const PBRMaterial&&) = default;
			PBRMaterial& operator=(const PBRMaterial&) = default;
			PBRMaterial& operator=(PBRMaterial&&) = default;
		private:
			ColorParam diffuse_;
			ColorParam specular_;
			ValueParam specular_power_;
			ColorParam emission_;
			ValueParam opacity_;
			ColorParam transparency_;
			TextureObject norma_map_;
			TextureObject height_map_;
			ValueParam height_scale_;
			TextureObject occlution_;
			ValueParam roughness_;
			ValueParam metalness_;
			//清漆材质
			ColorParam clearcoat_;
			ValueParam clearcoat_roughness_;
			ColorParam sheen_;
			ValueParam sheen_roughness_;
			Value ior_;
		};
	}//Scene
}//YiaEngine