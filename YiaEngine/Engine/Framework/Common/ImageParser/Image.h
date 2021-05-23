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
		uint32_t bitCount;	//����Ԫ����ռbit
		uint32_t pitch;		//һ�е�bit��
		size_t data_size;	//��bit��
	};
}