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
			void Create(UINT width, UINT height, DXGI_FORMAT format,  UINT numMip = 1, UINT arraySize = 1, UINT sampleCount = 1);
			void CreateResource(UINT width,UINT height, DXGI_FORMAT format, const D3D12_CLEAR_VALUE* clearValue,UINT arraySize,UINT sampleCount,UINT numMip);
			void CreateView(DXGI_FORMAT format,UINT arraySize,UINT numMips);
		protected:
			DescriptorHandle srvHandle;
			DescriptorHandle rtvHandle;
		};
	}
}