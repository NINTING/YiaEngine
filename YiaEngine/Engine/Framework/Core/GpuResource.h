#pragma once

#include"Graphic.h"

namespace YiaEngine
{
	namespace Graphic
	{
		class GpuResource
		{
		public:
			ID3D12Resource* resource() {
				return resource_.Get();
			}
		protected:
			ComPtr<ID3D12Resource>resource_;
			D3D12_RESOURCE_STATES usage_;
		};
	}
		
}