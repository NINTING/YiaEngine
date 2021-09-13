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
			void CreateResource(UINT width,UINT height, DXGI_FORMAT format, const D3D12_CLEAR_VALUE* clearValue,UINT arraySize,UINT sampleCount,UINT numMip);
			void CreateView(DXGI_FORMAT format,UINT arraySize,UINT numMips);
			DescriptorHandle RtvCpuHandle() { return rtvHandle_; };
			DescriptorHandle SrvCpuHandle() {return srvHandle_; };
		protected:
			UINT width_;
			UINT height_;
			DXGI_FORMAT format_;
			UINT numMips_;
			UINT arraySize_;
			DescriptorHandle srvHandle_;
			DescriptorHandle rtvHandle_;
		};
	}
}