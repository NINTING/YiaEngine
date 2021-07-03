#pragma once

#include<stdint.h>
#include"GeomMath.h"
namespace YiaEngine
{
	
	struct Image
	{
		uint32_t width;
		uint32_t height;
		R8G8B8A8U* pData;
		uint32_t bitCount;	//单个元素所占bit
		uint32_t pitch;		//一行的bit数
		size_t data_size;	//总bit数
	};
}