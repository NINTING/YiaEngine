#pragma once

#include "Image.h"
#include"Buffer.h"
namespace YiaEngine
{
	class ImageParser
	{
	public:
		virtual ImageData Parser(const Buffer& buffer) = 0;
	};
}