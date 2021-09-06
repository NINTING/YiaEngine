#pragma once
#include<d3d12.h>
#include<d3dx12.h>
#include <wrl/client.h>

#include"Utility.h"
using namespace Microsoft::WRL;




namespace YiaEngine
{

	namespace Graphic
	{
		class CommandManager;
		
		extern CommandManager g_commandManager;
		extern ComPtr<ID3D12Device> g_Device;
	}
}