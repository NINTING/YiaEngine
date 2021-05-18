//#pragma once
//#pragma comment(lib,"d3dcompiler.lib")
//#pragma comment(lib,"D3D12.lib")
//#pragma comment(lib,"dxgi.lib")
//
//
//#include <windows.h>
//#include <windowsx.h>
//#include <d3d12.h>
//#include <DXGI1_4.h>
//#include <wrl/client.h>
//#include <d3dcompiler.h>
//#include <DirectXMath.h>
//#include <DirectXPackedVector.h>
//#include <DirectXColors.h>
//#include <dxgi1_6.h>
//#include<stdexcept>
//#include <string>
//#include "d3dx12.h"
//
//using namespace DirectX;
//using namespace Microsoft::WRL;
//using Microsoft::WRL::ComPtr;
//
//
////��Ⱦ��������
//
//
//inline std::string  HrToString(HRESULT hr);
//
//
//
//struct Vertex
//{
//	XMFLOAT3 position;
//	XMFLOAT4 color;
//};
//
//
//class HrException :public std::runtime_error
//{
//public:
//	HrException(HRESULT hr) :runtime_error(HrToString(hr)), m_hr(hr) {}
//	HRESULT Error()const {return m_hr ;}
//private:
//	const HRESULT m_hr;
//};
//inline void ThrowIfFailed(HRESULT hr);
//class DeferApp {
//
//public:
//
//	//void GetHardwareAdapter(IDXGIFactory* pFactory, IDXGIAdapter** ppAdapter);
//
//	void WaitForPreviousFrame();
//
//
//	void InitEngine();
//
//	void LoadPipeline(HWND hwnd);
//
//
//	void LoadAsset();
//
//	void Render();
//
//
//	void PopulateCommandList();
//
//	void Destroy();
//
//	void Update();
//
//	int g_width =512;
//	int g_height = 512;
//
//	CD3DX12_VIEWPORT  g_viewport;
//	CD3DX12_RECT g_scissorRect;
//
//
//	ComPtr<ID3D12Device> g_device;
//	ComPtr<ID3D12CommandQueue>g_CommandQueue;
//	ComPtr< IDXGISwapChain3> g_SwapChain;
//	ComPtr<ID3D12DescriptorHeap>g_RTVHeap;
//	ComPtr<ID3D12CommandAllocator>g_commandAllocator;
//	ComPtr<ID3D12RootSignature>g_rootSignature;
//	ComPtr<ID3D12PipelineState>g_pipelineState;
//	ComPtr<ID3D12GraphicsCommandList>g_commandList;
//	ComPtr<ID3D12Resource>g_vertexBuffer;
//	ComPtr<ID3D12Resource> g_renderTargets[2];
//	UINT g_rtvDescriptorSize;
//	//ͬ��
//	ComPtr<ID3D12Fence> g_Fence;
//	D3D12_VERTEX_BUFFER_VIEW g_VertexBufferView;
//	UINT g_frameIndex = 0;
//	HANDLE g_fenceEvent;
//	UINT64 g_fenceValue = 0;
//
//};
//
