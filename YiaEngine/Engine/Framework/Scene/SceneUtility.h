#pragma once
#ifndef FRAMEWORLK_SCENE_OBJECT_H_
#define FRAMEWORLK_SCENE_OBJECT_H_

#define WINDOWS_PLATFORM

#include<memory>
#include<string>
#include<vector>
#include<sstream>

#include<assert.h>

//#include"MagicEnum.h"
#include "crossguid/guid.hpp"

#include "Common/MacroHelp.h"
#include "Common/ImageParser/Image.h"




namespace YiaEngine
{
	namespace Scene
	{
		//http://www.opengex.org/opengex-spec.pdf
		

		/*template<>
		struct YiaEngine::Meta::EnumMeta<MeshPrimitive>
		{
			typedef MeshPrimitive type;
			static constexpr std::string_view type_name{};
			static constexpr size_t count = enum_count<MeshPrimitive>();
			static constexpr std::array<MeshPrimitive, count>values{};
			static constexpr std::array<std::string_view, count> names{};
			static constexpr Core::CategoryTag tag = Core::CategoryTag::kEnumClassTag;
		};*/

		//BaseSceneObject提供Guid，所有SceneObject类型都表示一个持久化的Asset


		enum class AttenType
		{
			kNone,
			kLinearAtten,
		};
		class AttenCurve {
			static float Atten(AttenType type, float instance, float distance)
			{
				if (type == AttenType::kLinearAtten)
					return LinearAtten(instance, distance);
				return instance;
			}

			static float LinearAtten(float instance, float distance)
			{
				return instance * 1.f / distance;
			}
		};
	}

}	//YiaEngine
#endif //FRAMEWORLK_SCENE_OBJECT_H_