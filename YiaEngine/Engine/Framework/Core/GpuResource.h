#pragma once

#include"Graphic.h"

namespace YiaEngine
{
	namespace Graphic
	{
		class GpuResource
		{
		public:
			GpuResource() = default;
			GpuResource(ID3D12Resource* resource) :resource_(resource) {}
			ID3D12Resource* resource() {
				return resource_.Get();
			}
		protected:
			ComPtr<ID3D12Resource>resource_;
			D3D12_RESOURCE_STATES usage_;
			D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress_ = -1;
		};
	}
		
}