#pragma once
#ifndef FRAMEWORK_SCENE_ENUM_H
#define FRAMEWORK_SCENE_ENUM_H

#include "Engine/Meta/Meta.h"

namespace YiaEngine
{
	namespace Scene
	{
		enum class DataType : int
		{
			kUint16_1 = 1,
			kUint32_1 = 2,
			kFloat_1 = 3,
			kDouble_1 = 4,
			kUint16_2 = 5,
			kUint32_2 = 6,
			kFloat_2 = 7,
			kDouble_2 = 8,
			kUint16_3 = 9,
			kUint32_3 = 10,
			kFloat_3 = 11,
			kDouble_3 = 12,
			kUint16_4 = 13,
			kUint32_4 = 14,
			kFloat_4 = 15,
			kDouble_4 = 16,
		};

		

		static int getDataTypeForCount(DataType value)
		{
			return  (static_cast<int>(value) - 1) / 4 + 1;
		}


		enum class SceneObjectType
		{
			kGemometryObject,
			kMeshObject,
			kTextureObject,
			kMaterialObject,
			kLightObject,
			kCameraObject
		};
	}
	META_ENUM(Scene::DataType);
}

#endif //FRAMEWORK_SCENE_ENUM_H