#pragma once

#include "Image.h"

namespace YiaEngine
{
	class ImageParser
	{
	public:
		virtual ImageData Parser(const Buffer& buffer) = 0;
	};
}