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

		class RenderBuffer :public GpuTexture
		{
		public:
			void Create(const wchar_t* Name,UINT width, UINT Height, DXGI_FORMAT format,  UINT numMip = 1, UINT arraySize = 1, UINT sampleCount = 1);
			~RenderBuffer() { Destroy(); }
			void CreateFromSwapChian(const wchar_t* Name,ID3D12Resource* resource);
			DescriptorHandle RtvCpuHandle()const { return rtvHandle_; };
			virtual DescriptorHandle SrvCpuHandle()const { return srvHandle_; };
			
			const DescriptorHandle* RtvCpuHandlePtr()const { return &rtvHandle_; };
			const DescriptorHandle* SrvCpuHandlePtr()const { return &srvHandle_; };
			Math::Vec2f Size() { return Math::Vec2f{ width_,height_ }; };
		private:
			void CreateResource(UINT width, UINT Height, DXGI_FORMAT format, const D3D12_CLEAR_VALUE* clearValue, UINT arraySize, UINT sampleCount, UINT numMip);
			void CreateView(DXGI_FORMAT format, UINT arraySize, UINT numMips);
		protected:
			
			DescriptorHandle rtvHandle_;
			DescriptorHandle srvHandle_;
		};
	}
}