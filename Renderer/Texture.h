#pragma once
#include"Core/YiaGraphic.h"
#include"Image.h"
namespace YiaEngine
{
	namespace Graphic
	{
		class Texture :public GpuTexture
		{
		public:
			Texture() = default;
			void InitializeByImage(ImageData& image, DXGI_FORMAT format);
			void SetDebugName(const wchar_t* name);
			void CreateView();
			void CreateTex2D();
			virtual  DescriptorHandle SrvCpuHandle() const { return srvHandle_; };
		private:
			
			DescriptorHandle srvHandle_;

			

		};
	}
}


