#pragma once

#include"Graphic.h"
#include"GpuResource.h"

namespace YiaEngine
{
	class Image;
	namespace Graphic
	{

		class Texture :GpuResource
		{
			Texture();
			void InitializeByImage(Image* image);
		private:
			UINT width_;
			UINT height_;
			
		};
	}
}