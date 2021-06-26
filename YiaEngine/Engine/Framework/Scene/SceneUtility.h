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

#include"Scene/Object/MaterialObject.h"


namespace YiaEngine
{
	namespace Scene
	{
		//http://www.opengex.org/opengex-spec.pdf
		

		


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