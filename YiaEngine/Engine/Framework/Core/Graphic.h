#pragma once
#include<d3d12.h>
#include <wrl/client.h>

#include"Utility.h"
using namespace Microsoft::WRL;
namespace YiaEngine
{
	namespace Graphic
	{
		
		extern ComPtr<ID3D12Device> g_device;
	}
}