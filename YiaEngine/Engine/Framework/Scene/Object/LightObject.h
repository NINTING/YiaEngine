#pragma once

#include"BaseSceneObject.h"
#include"SceneUtility.h"
#include"SceneEnum.h"
namespace YiaEngine
{
	namespace Scene
	{
		enum class LightType
		{
			kPointLight,
			kSpotLight,
			kDirectionLight,
		};

		class LightObject :public BaseSceneObject
		{
		protected:
			LightObject() :BaseSceneObject(SceneObjectType::kLightObject) {}

			bool shadow_;
			float instansity_;
			//LightType type_;
			Color color_;
			AttenType atten_type_;
			float near_clip_;
			float far_clip_;
		};
		class SpotLight :public LightObject
		{
		public:
			SpotLight() :LightObject() {}
		private:
			float inner_angle_;
			float out_angle_;
		};

		class PointLight :public LightObject
		{
		public:
			PointLight() :LightObject() {}
		};

		class DirectionLight :public LightObject
		{
			DirectionLight() :LightObject() {}
		};
	}
}
