#pragma once

#include"Graphic.h"
#include"GpuResource.h"
#include"DescriptorHeap.h"
namespace YiaEngine
{
	class Image;
	namespace Graphic
	{

		class Texture :public GpuResource
		{
		public:
			Texture() = default;
			void InitializeByImage(Image* image);
			void CreateTex2D();
			DescriptorHandle CpuHandle()const { return handle; };
		private:
			UINT width_;
			UINT height_;
			DescriptorHandle handle;
			
		};
	}
}