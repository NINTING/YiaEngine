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
			kUint16_1 = 1,		//2
			kUint32_1 = 2,		//4
			kFloat_1 = 3,		//4
			kUint16_2 = 4,		//4
			kUint16_3 = 5,		//6
			kUint16_4 = 6,		//8
			kDouble_1 = 7,		//8
			kUint32_2 = 8,		//8
			kFloat_2 = 9,		//8
			kUint32_3 = 10,		//12
			kFloat_3 = 11,		//12
			kDouble_2 = 12,		//16
			kUint32_4 = 13,		//16
			kFloat_4 = 14,		//16
			kDouble_3 = 15,		//24
			kDouble_4 = 16,		//32
		};

		

		size_t DataTypeStride(DataType data_type);

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