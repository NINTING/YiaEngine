#pragma once

#include"Graphic.h"
#include"CommandManager.h"
namespace YiaEngine
{
	namespace Graphic
	{
		Microsoft::WRL::ComPtr<ID3D12Device> g_Device = nullptr;
		CommandManager g_commandManager;
	}
}