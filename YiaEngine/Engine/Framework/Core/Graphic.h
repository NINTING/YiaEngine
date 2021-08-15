#pragma once
#include<d3d12.h>
#include <wrl/client.h>

namespace YiaEngine
{
	namespace Graphic
	{
		extern Microsoft::WRL::ComPtr<ID3D12Device> g_device;
	}
}