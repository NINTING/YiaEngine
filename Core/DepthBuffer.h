#pragma once

#include"Graphic.h"
#include"GpuResource.h"
#include"DescriptorHeap.h"
#include"GpuTexture.h"

#include"Common/YiaMath.h"
namespace YiaEngine
{
	namespace Graphic
	{

		class DepthBuffer :public GpuTexture
		{
		public:
			DepthBuffer();
			void Create(const wchar_t* Name, UINT width, UINT Height, DXGI_FORMAT format);
			//~DepthBuffe() { Destroy(); }
			void CreateFromSwapChian(const wchar_t* Name, ID3D12Resource* resource);


			virtual  DescriptorHandle SrvCpuHandle() const { return depthSrvHandle_; };

			 DescriptorHandle GetDepthStencilHandle() { return dsvHandle_[0]; };
			const DescriptorHandle* GetDepthStencilHandlePtr()const { return &dsvHandle_[0]; };
			const DescriptorHandle* GetDepthReadHandlePtr()const { return &dsvHandle_[1]; };
			const DescriptorHandle* GetStencilReadHandlePtr()const { return &dsvHandle_[2]; };
			const DescriptorHandle* GetDepthStencilReadHandlePtr()const { return &dsvHandle_[3]; };
			const DescriptorHandle* GetDepthSrvCpuHandlePtr()const { return &depthSrvHandle_; };
			const DescriptorHandle* GetStencilSrvCpuHandlePtr()const { return &depthSrvHandle_; };
												  
		private:
			
			void CreateView(DXGI_FORMAT format);
			DXGI_FORMAT GetDepthFormat(DXGI_FORMAT defaultFormat);
		protected:
			
			DescriptorHandle dsvHandle_[4];
			DescriptorHandle stencilHandle_;
			DescriptorHandle depthSrvHandle_;
			DescriptorHandle stencilSrvHandle_;
		};
	}
}