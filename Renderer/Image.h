#pragma once

#include"pch.h"
namespace YiaEngine
{
	namespace Graphic
	{

		struct ImageData
		{
			uint32_t Width;
			uint32_t Height;
			uint32_t PerPixelByte;	//����Ԫ����ռbit
			uint32_t Pitch;		//һ�е�bit��
			std::shared_ptr<uint8_t> pData;
			size_t Size;	//��bit��

		};

	}
}