#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <vector>

#include<memory>

#include"RootSignature.h"
namespace YiaEngine::Graphic
{
	class PipelineStateObject
	{
	public:
		void SetInputLayout(const D3D12_INPUT_LAYOUT_DESC& layout_desc);
		void SetRootSignature(const RootSignature& signature);
		void SetVertexShader(UINT8* buffer, size_t size);
		void SetPixelShader(UINT8* buffer, size_t size);
		void Finalize();

	private:
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc_;
		Microsoft::WRL::ComPtr <ID3D12PipelineState> pso_;
	};
}