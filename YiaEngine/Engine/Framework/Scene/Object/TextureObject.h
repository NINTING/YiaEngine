#pragma once
#include"BaseSceneObject.h"

#include "Common/ImageParser/Image.h"

namespace YiaEngine
{
	namespace Scene
	{
		enum class AddressingMode
		{
			kRepeat,
			kClamp,
			kBorder,
			kMirror_Reapt,
			kMirror_Clamp,
			kMirror_Border
		};

		class TextureObject :public BaseSceneObject {

		private:
			std::string name_;
			std::shared_ptr<Image>image_;
			//指定使用网格哪一个纹理坐标集
			uint32_t texcoord_ = 0;

			//应用于纹理坐标的转换
			//包含以下几种矩阵类型
			//Transform,Translation,Rotation,Scale
			std::vector<Mat4x4f> transform_;

			AddressingMode x_address_;
			AddressingMode y_address_;
			AddressingMode z_address_;
			//当时Border模式下，border的颜色
			Color border_;
		public:
			TextureObject() :
				BaseSceneObject(SceneObjectType::kTextureObject),
				image_(nullptr),
				x_address_(AddressingMode::kRepeat),
				y_address_(AddressingMode::kRepeat),
				z_address_(AddressingMode::kRepeat),
				border_(Color::Zero()) {}
			TextureObject(TextureObject&) = default;
			TextureObject(TextureObject&&) = default;
			TextureObject(std::string name) :BaseSceneObject(SceneObjectType::kTextureObject), name_(name) {}
			TextureObject(uint32_t texcoord, const std::shared_ptr<Image>& image) :
				BaseSceneObject(SceneObjectType::kTextureObject),
				texcoord_(texcoord),
				image_(image) {}
			TextureObject(uint32_t texcoord, const std::shared_ptr<Image>&& image) :
				BaseSceneObject(SceneObjectType::kTextureObject),
				texcoord_(texcoord),
				image_(std::move(image)) {}

		};
	}//Scene
}//YiaEngine
