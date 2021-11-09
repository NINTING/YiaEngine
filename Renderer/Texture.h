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
			void InitializeByImage(ImageData& image, DXGI_FORMAT format);
			void SetDebugName(const wchar_t* name);
			void CreateView();
			void CreateTex2D();
			DescriptorHandle CpuHandle()const { return srvHandle; };
		private:
			UINT width_;
			UINT height_;
			DescriptorHandle srvHandle;


			

		};
	}
}


