#pragma once
#include"Core/YiaGraphic.h"
#include"Image.h"
namespace YiaEngine
{
	namespace Graphic
	{
		class Texture :public GpuResource
		{
		public:
			Texture() = default;
			void InitializeByImage(ImageData* image);
			void InitializeByImage(ImageData& image);
			void CreateTex2D();
			DescriptorHandle CpuHandle()const { return handle; };
		private:
			UINT width_;
			UINT height_;
			DescriptorHandle handle;


			

		};
	}
}


