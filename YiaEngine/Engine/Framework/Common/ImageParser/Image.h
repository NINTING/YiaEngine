#pragma once

#include<stdint.h>
#include"GeomMath.h"
namespace YiaEngine
{
	
	struct Image
	{
		uint32_t width;
		uint32_t height;
		uint32_t bitCount;	//单个元素所占bit
		uint32_t pitch;		//一行的bit数
		void* pData;
		size_t data_size;	//总bit数
	};

	
}