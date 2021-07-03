#pragma once

#include<stdint.h>
#include"GeomMath.h"
namespace YiaEngine
{
	
	struct Image
	{
		uint32_t width;
		uint32_t height;
		uint32_t bitCount;	//����Ԫ����ռbit
		uint32_t pitch;		//һ�е�bit��
		void* pData;
		size_t data_size;	//��bit��
	};

	
}