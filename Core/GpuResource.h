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
			GpuResource(ID3D12Resource* RawResource) :resource_(RawResource) {}
			ID3D12Resource* RawResource() {
				return resource_.Get();
			}
			D3D12_RESOURCE_STATES Usage() { return usage_; };
			void Usage(D3D12_RESOURCE_STATES state) { usage_ = state; }
		protected:
			ComPtr<ID3D12Resource>resource_;
			D3D12_RESOURCE_STATES usage_;
			D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress_ = ADDRESS_UNKOWN;
		};
	}
		
}