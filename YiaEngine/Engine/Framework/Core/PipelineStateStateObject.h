#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <vector>

#include<memory>
namespace YiaEngine::Graphic
{
	class PipleStateObject
	{
		void Set
		void Finalize();

	private:
		Microsoft::WRL::ComPtr <ID3D12PipelineState> pso_;
	};
}