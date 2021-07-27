#pragma once
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"D3D12.lib")
#pragma comment(lib,"dxgi.lib")


#include <windows.h>
#include <windowsx.h>
#include <initguid.h>
#include <d3d12.h>
#include <DXGI1_4.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <dxgi1_6.h>
#include<stdexcept>
#include <string>
#include "d3dx12.h"

#include"Image.h"
#include"SceneNode.h"
#include "Editor/SceneWindow.h"
using namespace DirectX;
using namespace Microsoft::WRL;
using Microsoft::WRL::ComPtr;



using namespace YiaEngine;


//渲染基本设置


inline std::string  HrToString(HRESULT hr);


struct FrameResource;
struct Vertex_Pos_Color_Uv
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT2 uv;
};

struct Vertex_Pos_Uv
{
	XMFLOAT3 position;

	XMFLOAT2 uv;
};

class HrException :public std::runtime_error
{
public:
	HrException(HRESULT hr) :runtime_error(HrToString(hr)), m_hr(hr) {}
	HRESULT Error()const {return m_hr ;}
private:
	const HRESULT m_hr;
};
inline void ThrowIfFailed(HRESULT hr);
class App {

public:
	
	//void GetHardwareAdapter(IDXGIFactory* pFactory, IDXGIAdapter** ppAdapter);

	void WaitForPreviousFrame(bool is_end_frame = false);


	void InitEngine();

	void LoadPipeline(HWND hwnd);
	void UpdateGPUResource();
	
	void LoadAsset();

	void Render();

	void DrawUI();
	void PopulateSceneCommandList();

	void PopulateUICommandList();

	void SwapPresent();
	void ClearRenderTarget(CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle);

	void LoadTextureBuffer(const std::shared_ptr<YiaEngine::Image>& image,
		ID3D12DescriptorHeap* descriptor_heap,UINT offset, ID3D12Resource** texture_buffer);
	void Destroy();

	void Update();

	void BindVertexAttribute(void* data, size_t data_size, size_t stride, int index);

	void BindIndexBuffer(void* data, size_t data_size);
	void ExecuteCommand();
	int g_width =1024;
	int g_height = 1024;

	static ComPtr<ID3D12Device> GetCurrentDevice();
	

	CD3DX12_VIEWPORT  g_viewport;
	CD3DX12_RECT g_scissorRect;


	ComPtr<ID3D12Device> g_device;
	ComPtr<ID3D12CommandQueue>g_CommandQueue;
	ComPtr< IDXGISwapChain3> g_SwapChain;
	ComPtr<ID3D12DescriptorHeap>g_RTVHeap;
	ComPtr<ID3D12CommandAllocator>g_commandAllocator;
	ComPtr<ID3D12CommandAllocator>g_currentCommandAllocator;
	ComPtr<ID3D12CommandAllocator>g_BundleAllocator;
	ComPtr<ID3D12RootSignature>g_rootSignature;
	ComPtr<ID3D12PipelineState>g_pipelineState;
	ComPtr<ID3D12GraphicsCommandList>g_commandList;
	ComPtr<ID3D12GraphicsCommandList>g_BundleList;
	ComPtr<ID3D12Resource>g_vertexBuffer[2];
	
	ComPtr<ID3D12Resource>g_indexBuffer;
	ComPtr<ID3D12Resource> g_renderTargets[2];
	ComPtr<ID3D12Resource>g_texture;
	//ComPtr<ID3D12Resource> tempraryUpload;
	ComPtr<ID3D12DescriptorHeap>g_ImGui_SrvCbvHeap;
	ComPtr<ID3D12Resource> g_cbv;
	ComPtr<ID3D12DescriptorHeap> g_SRVCBVHeap;
	
	UINT g_rtvDescriptorSize;

	bool firstLoadAsset;
	
	std::unique_ptr<YiaEngine::Scene::GeometryNode> g_mode;
	//同步
	ComPtr<ID3D12Fence> g_Fence;
	D3D12_VERTEX_BUFFER_VIEW g_VertexBufferView[10];

	D3D12_INDEX_BUFFER_VIEW g_IndexBufferView;
	UINT g_frameIndex = 0;
	HANDLE g_fenceEvent;
	UINT64 g_fenceValue = 0;
	
	std::vector<std::unique_ptr<FrameResource>>frame_resouces_;
	int frames_count_ = 3;
	int current_frame_ = 0;
	ComPtr<ID3D12CommandAllocator>current_cmd_alloc;

	//scene
	std::unique_ptr<YiaEngine::Scene::CameraNode> editor_camera;
	ComPtr<ID3D12Resource> g_scene_tex;


	//GUI
	std::unique_ptr<SceneWindow> scene_window_;
};

struct FrameResource
{
	FrameResource(ID3D12Device* device,const WCHAR*name) {
		fence = 0;
		ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmd_list_alloctor)));
		cmd_list_alloctor->SetName(name);
	}
	uint32_t fence;
	ComPtr<ID3D12CommandAllocator>cmd_list_alloctor;
};

