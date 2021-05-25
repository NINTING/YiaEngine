#pragma once

#include "Image.h"
#include"Buffer.h"
namespace YiaEngine
{
	class ImageParser
	{
	public:
		virtual Image Parser(const Buffer& buffer) = 0;
	};
}