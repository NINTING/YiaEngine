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
			uint32_t PerPixelByte;	//单个元素所占bit
			uint32_t Pitch;		//一行的bit数
			std::shared_ptr<uint8_t> pData;
			size_t Size;	//总bit数

		};

	}
}