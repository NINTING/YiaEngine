#pragma once

#include"Graphic.h"
#include"GpuResource.h"
#include"DescriptorHeap.h"
namespace YiaEngine
{
	namespace Graphic
	{

		class RenderBuffer :public GpuResource
		{
		public:
			void Create(UINT width, UINT height, DXGI_FORMAT format,  UINT numMip = 1, UINT arraySize = 1, UINT sampleCount = 1);

			void CreateFromSwapChian(const wchar_t* name,ID3D12Resource* resource);
			DescriptorHandle RtvCpuHandle()const { return rtvHandle_; };
			DescriptorHandle SrvCpuHandle()const { return srvHandle_; };
			
			const DescriptorHandle* RtvCpuHandlePtr()const { return &rtvHandle_; };
			const DescriptorHandle* SrvCpuHandlePtr()const { return &srvHandle_; };

		private:
			void CreateResource(UINT width, UINT height, DXGI_FORMAT format, const D3D12_CLEAR_VALUE* clearValue, UINT arraySize, UINT sampleCount, UINT numMip);
			void CreateView(DXGI_FORMAT format, UINT arraySize, UINT numMips);
		protected:
			UINT64 width_;
			UINT64 height_;
			DXGI_FORMAT format_;
			UINT numMips_;
			UINT arraySize_;
			DescriptorHandle srvHandle_;
			DescriptorHandle rtvHandle_;
		};
	}
}