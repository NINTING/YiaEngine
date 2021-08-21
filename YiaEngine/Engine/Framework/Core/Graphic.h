#pragma once
#include<d3d12.h>
#include <wrl/client.h>

namespace YiaEngine
{
	namespace Graphic
	{
		using namespace Microsoft::WRL;
		extern ComPtr<ID3D12Device> g_device;
	}
}