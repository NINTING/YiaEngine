#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <vector>
#include<memory>

#include"Graphic.h"
#include"RootSignature.h"
#include"Shader.h"
namespace YiaEngine::Graphic
{
	class PipelineStateObject
	{	
	public:
		static void StaticPSOInit();
		static PipelineStateObject s_DefaultPSO;
	public:
		PipelineStateObject(const wchar_t*name = L"unanemed PSO");
		void SetInputLayout(UINT NumElements, const D3D12_INPUT_ELEMENT_DESC* pInputElementDescs);
		void SetInputLayout(const D3D12_INPUT_LAYOUT_DESC& layout_desc);
		void SetRootSignature(const RootSignature& signature);
		void SetVertexShader(const Shader& shader);
		void SetVertexShader(const void* buffer, size_t size);
		void SetPixelShader(const void* buffer, size_t size);

		void SetPixelShader(const Shader& shader);
	
		void SetRasterizerState(D3D12_RASTERIZER_DESC desc);
		void SetBlendState(D3D12_BLEND_DESC desc);
		void SetDepthStencilState(D3D12_DEPTH_STENCIL_DESC depth_stencil_desc);
		void SetSampleMask(UINT mask);
		void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type);
		void SetRenderTarget(DXGI_FORMAT rtv_format, DXGI_FORMAT depth_format);

		void SetRenderTargets(UINT count, DXGI_FORMAT* rtv_format, DXGI_FORMAT depth_format);

		void SetSampleDesc(UINT count, UINT quality);

		ID3D12PipelineState* RawPipelineStateObject()const { return pso_.Get(); }
		void Finalize();

	private:
		const wchar_t* name_;
		const RootSignature* signature_;
		std::shared_ptr <const D3D12_INPUT_ELEMENT_DESC>input_layout_;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc_;
		Microsoft::WRL::ComPtr <ID3D12PipelineState> pso_;
	};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
}