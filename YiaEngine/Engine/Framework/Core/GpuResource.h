#pragma once

#include"Graphic.h"

namespace YiaEngine::Graphic
{
	
	class GpuResource
	{

	protected:
		ComPtr<ID3D12Resource>resource_;
		D3D12_RESOURCE_STATES usage_;
	};
		
}