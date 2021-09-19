//渲染基本设置



#include "imgui.h"
#include "ImGui/backend/imgui_impl_win32.h"
#include "ImGui/backend/imgui_impl_dx12.h"


#include"Dx12Sample.h"
#include"BmpParser.h"
#include"AssetLoder.h"
#include"SceneObject.h"

#include"Common/YiaTime.h"
#include"Common/Input.h"

#include"Core/Graphic.h"
#include"Core/RootSignature.h"

#include"Core/CommandContext.h"
#include"Core/CommandManager.h"
#include"Core/DescriptorHeap.h"
#include"Core/GraphicContext.h"
inline std::string  HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}




inline void ThrowIfFailed(HRESULT hr)
{ 
	if (FAILED(hr))
	{
	
		throw HrException(hr);
	}
}
UINT TextureWidth = 200;
UINT TextureHeight = 200;
UINT TexturePixelSize = 4;
std::vector<UINT8> GenerateTextureData()
{
	const UINT rowPitch = TextureWidth * TexturePixelSize;
	const UINT cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
	const UINT cellHeight = TextureWidth >> 3;    // The height of a cell in the checkerboard texture.
	const UINT textureSize = rowPitch * TextureHeight;

	std::vector<UINT8> data(textureSize);
	UINT8* pData = &data[0];

	for (UINT n = 0; n < textureSize; n += TexturePixelSize)
	{
		UINT x = n % rowPitch;
		UINT y = n / rowPitch;
		UINT i = x / cellPitch;
		UINT j = y / cellHeight;

		if (i % 2 == j % 2)
		{
			pData[n] = 0x00;        // R
			pData[n + 1] = 0xFF;    // G
			pData[n + 2] = 0xFF;    // B
			pData[n + 3] = 0xff;    // A
		}
		else
		{
			pData[n] = 0xff;        // R
			pData[n + 1] = 0xff;    // G
			pData[n + 2] = 0xff;    // B
			pData[n + 3] = 0xff;    // A
		}
	}

	return data;
}

 void GetHardwareAdapter(
	 IDXGIFactory1* pFactory,
	 IDXGIAdapter1** ppAdapter,
	 bool requestHighPerformanceAdapter)
 {
	 *ppAdapter = nullptr;

	 ComPtr<IDXGIAdapter1> adapter;

	 ComPtr<IDXGIFactory6> factory6;
	 if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
	 {
	

		 int adapterIndex = 0; // we'll start looking for directx 12  compatible graphics devices starting at index 0

		 bool adapterFound = false; // set this to true when a good one was found

		 // find first hardware gpu that supports d3d 12
		 while (factory6->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
		 {
			 DXGI_ADAPTER_DESC1 desc;
			 adapter->GetDesc1(&desc);

			 if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			 {
				 // we dont want a software device
				 adapterIndex++; // add this line here. Its not currently in the downloadable project
				 continue;
			 }

			 // we want a device that is compatible with direct3d 12 (feature level 11 or higher)
			 ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr));
			

			 adapterIndex++;
		 }

		 

	 }
	 else
	 {
		 for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
		 {
			 DXGI_ADAPTER_DESC1 desc;
			 adapter->GetDesc1(&desc);

			 if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			 {
				 // Don't select the Basic Render Driver adapter.
				 // If you want a software adapter, pass in "/warp" on the command line.
				 continue;
			 }

			 // Check to see whether the adapter supports Direct3D 12, but don't create the
			 // actual device yet.
			 if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			 {
				 break;
			 }
		 }
	 }

	 *ppAdapter = adapter.Detach();
 }


 void CompilerShader(const wchar_t*path, const char* entry,const char* target,ID3DBlob** Shader)
 {
#if defined(_DEBUG)
	 // Enable better shader debugging with the graphics debugging tools.
	 UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	 UINT compileFlags = 0;
#endif
	 ComPtr<ID3DBlob>error;
	 HRESULT hr = D3DCompileFromFile(path, nullptr, nullptr, entry, target, compileFlags, 0, Shader, &error);
	 if (error != nullptr)
	 {
		 OutputDebugStringA((char*)error->GetBufferPointer());
	 }

	 ThrowIfFailed(hr);

 }
void App::WaitForPreviousFrame(bool is_end_frame)
{
	current_frame_ =  (current_frame_+1) % (frames_count_);
	UINT64  fence =  frame_resouces_[current_frame_]->fence;
	ThrowIfFailed(g_CommandQueue->Signal(g_Fence.Get(), frame_resouces_[current_frame_]->fence));
	//ThrowIfFailed(g_CommandQueue->Signal(g_Fence.Get(), fence));
	frame_resouces_[current_frame_]->fence = ++g_fenceValue;

	if ( g_Fence->GetCompletedValue() < fence)
	{
		g_Fence->SetEventOnCompletion(fence, g_fenceEvent);
		WaitForSingleObject(g_fenceEvent, INFINITE);
	}

	if (!is_end_frame)
	{
		g_frameIndex = g_SwapChain->GetCurrentBackBufferIndex();
		current_cmd_alloc = frame_resouces_[current_frame_]->cmd_list_alloctor;
	}
}

void App::InitEngine()
{

	g_frameIndex = 0;
	g_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(g_width), static_cast<float>(g_height));
	g_scissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(g_width), static_cast<LONG>(g_height));
	g_rtvDescriptorSize = 0;


}



void App::LoadPipeline(HWND hwnd)
{

	
#if defined DEBUG
	{
		ComPtr<ID3D12Debug>debugger;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugger))))
		{
			debugger->EnableDebugLayer();
		}
	}
#endif // DEBUG
	ComPtr<IDXGIFactory4> factory;
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));
	ComPtr<IDXGIAdapter1> hardwareAdapter;

	GetHardwareAdapter(factory.Get(), &hardwareAdapter,false);
	ThrowIfFailed(D3D12CreateDevice(
		hardwareAdapter.Get(),
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(& Graphic::g_Device)
	)); 
	Graphic::g_commandManager.Create(Graphic::g_Device.Get());
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed( Graphic::g_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&g_CommandQueue)));


	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Width = g_width;
	swapChainDesc.BufferDesc.Height = g_height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;

	ComPtr<IDXGISwapChain> swapChain;
	ThrowIfFailed(factory->CreateSwapChain(Graphic::g_commandManager.GetQueue(D3D12_COMMAND_LIST_TYPE_DIRECT).RawCommandQueue(), &swapChainDesc, &swapChain));
	
	ThrowIfFailed(swapChain.As(&g_SwapChain));
	
	//控制窗口切换(alt-enter)
	ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

	g_frameIndex = g_SwapChain->GetCurrentBackBufferIndex();

	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.NumDescriptors = 3;
		ThrowIfFailed( Graphic::g_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_RTVHeap)));
		
		//rtv_heap_.Create(L"RTV_Heap", rtvHeapDesc,  Graphic::g_device.Get());
		//rtv_heap_.Create(L"RTV_Heap", D3D12_DESCRIPTOR_HEAP_TYPE_RTV,3, Graphic::g_device.Get());
		g_rtvDescriptorSize =  Graphic::g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	{
		//分配交换链的2个rtv
	/*	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_RTVHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < 2; i++)
		{
			ThrowIfFailed(g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&g_renderTargets[i])));
			 Graphic::g_device->CreateRenderTargetView(g_renderTargets[i].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, g_rtvDescriptorSize);
		}*/
		//rtv_heap_.Alloc();
		
		swap_rtv_handle_[0] = descriptor_allcator_[D3D12_DESCRIPTOR_HEAP_TYPE_RTV].Alloc(1);
		swap_rtv_handle_[1] = descriptor_allcator_[D3D12_DESCRIPTOR_HEAP_TYPE_RTV].Alloc(1);
		for (UINT i = 0; i < 2; i++)
		{
			ID3D12Resource* swapChainResource;
			ThrowIfFailed(g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&swapChainResource)));
			
			Graphic::g_SwapRenderTarget[i].CreateFromSwapChian(L"Screen Render Target", swapChainResource);
		}
	
	}
	{
		//ImGui Resource
		/*D3D12_DESCRIPTOR_HEAP_DESC srvHeapdesc = {};
		srvHeapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapdesc.NumDescriptors = 10;
		srvHeapdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed( Graphic::g_device->CreateDescriptorHeap(&srvHeapdesc, IID_PPV_ARGS(&g_ImGui_SrvCbvHeap)));
		
		g_ImGui_SrvCbvHeap->SetName(L"UI_SRV");*/
		Graphic::g_GpuCommonDescriptoHeap.CreateShaderVisibleType(L"Common_SRV_CBV_UAV", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1024);
		gui_srv_heap.CreateShaderVisibleType(L"UI_SRV_CBV_UAV", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10);
		{		
			auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024, 1, 6, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
			auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			ThrowIfFailed( Graphic::g_Device->CreateCommittedResource(
				&heap_properties,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				nullptr,
				IID_PPV_ARGS(&g_scene_tex)));

			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = desc.Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
		//	UINT srv_desc_size =  Graphic::g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//	UINT rtv_size =  Graphic::g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		//	CD3DX12_CPU_DESCRIPTOR_HANDLE desc_handle(g_ImGui_SrvCbvHeap->GetCPUDescriptorHandleForHeapStart(), 1, srv_desc_size);
			 Graphic::g_Device->CreateShaderResourceView(g_scene_tex.Get(), &srvDesc, gui_srv_heap.Alloc());
			//Graphic::ResourceAllocator
			//场景RTV视图
		//	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_RTVHeap->GetCPUDescriptorHandleForHeapStart(), 2, g_rtvDescriptorSize);
			 
			scene_rtv_handle =  descriptor_allcator_[D3D12_DESCRIPTOR_HEAP_TYPE_RTV].Alloc(1);
			Graphic::g_Device->CreateRenderTargetView(g_scene_tex.Get(), nullptr, scene_rtv_handle);
			g_scene_tex->SetName(L"SCENE");
			
		}
		{
			sceneTarget.Create(1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM);
		}
		/*auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, 300, 300, 1, 6, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		ThrowIfFailed( Graphic::g_device->CreateCommittedResource(
			&heap_properties,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			IID_PPV_ARGS(&g_scene_tex)));

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		UINT srv_desc_size =  Graphic::g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE desc_1_handle(g_ImGui_SrvCbvHeap->GetCPUDescriptorHandleForHeapStart());
		CD3DX12_CPU_DESCRIPTOR_HANDLE desc_handle(g_ImGui_SrvCbvHeap->GetCPUDescriptorHandleForHeapStart(), 1, srv_desc_size);
		 Graphic::g_device->CreateShaderResourceView(g_scene_tex.Get(), &srvDesc, desc_handle);*/
		

	}
	std::wstring c_name;
	
	for (int i = 0; i < frames_count_; i++)
	{
		c_name = L"frame_command";
		c_name.append(std::to_wstring(i));
		
		frame_resouces_.push_back( std::make_unique<FrameResource>( Graphic::g_Device.Get(),c_name.c_str()));
	}

	ThrowIfFailed( Graphic::g_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocator)));

	ThrowIfFailed( Graphic::g_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_Fence)));
	g_fenceValue = 1;
	g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (g_fenceEvent == nullptr)
	{
		ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}
}

struct VPConstBuffer
{
	YiaEngine::Mat4x4f VPMat;
	float padding[48];
};

void App::LoadAsset()
{
	{
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

		// This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

		if (FAILED( Graphic::g_Device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
		{
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}

		CD3DX12_STATIC_SAMPLER_DESC linearClamp(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

		D3D12_SAMPLER_DESC sampler_desc;
		sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = 0;
		sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler_desc.MipLODBias = 0;
		sampler_desc.MaxAnisotropy = 0;

		//CD3DX12_STATIC_SAMPLER_DESC linearClamp(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
	

		CD3DX12_DESCRIPTOR_RANGE rootRange[2]; 
		rootRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		rootRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		/*CD3DX12_ROOT_PARAMETER para[1];
		para[0].InitAsDescriptorTable(1, rootPara,D3D12_SHADER_VISIBILITY_PIXEL);*/


		//CD3DX12_ROOT_PARAMETER  rootParameters[2];
		//rootParameters[0].InitAsDescriptorTable(1, &rootRange[0], D3D12_SHADER_VISIBILITY_ALL);
		//rootParameters[1].InitAsDescriptorTable(1, &rootRange[1], D3D12_SHADER_VISIBILITY_ALL);
		//
	/*	Graphic::RootParameter rootParameters[2];
		rootParameters[0].InitAsDescriptorTable(1,D3D12_SHADER_VISIBILITY_ALL);
		rootParameters[1].InitAsDescriptorTable(1, D3D12_SHADER_VISIBILITY_ALL);
		rootParameters[0].SetTableRange(0,1,D3D12_DESCRIPTOR_RANGE_TYPE_SRV,0);
		rootParameters[1].SetTableRange(0,1,D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 0);*/

		//rootSignature.AllocRootParamenter()
		
		rootSignature.Reset(2, 1);
		rootSignature.InitStaticSampler(0, sampler_desc);
		rootSignature[Graphic::DescriptorType::SRV].InitAsDescriptorTable(1);
		rootSignature[Graphic::DescriptorType::SRV].SetTableRange(0, 10, D3D12_DESCRIPTOR_RANGE_TYPE_SRV,0);
		rootSignature[Graphic::DescriptorType::CBV].InitAsDescriptorTable(1);
		rootSignature[Graphic::DescriptorType::CBV].SetTableRange(0, 10, D3D12_DESCRIPTOR_RANGE_TYPE_CBV,0);
		rootSignature.Finalize(L"sampleRootSignature", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		//CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc;

		//rootDesc.Init_1_0(_countof(rootParameters), (const D3D12_ROOT_PARAMETER *)rootParameters, 1, &linearClamp, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		//		ComPtr<ID3DBlob> error;
		//ComPtr<ID3DBlob> signature;

		//ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootDesc, featureData.HighestVersion, &signature, &error));
		//ThrowIfFailed( Graphic::g_Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&g_rootSignature)));

		/*CD3DX12_ROOT_SIGNATURE_DESC rootDesc;
		rootDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		ComPtr<ID3DBlob> error;
		ComPtr<ID3DBlob> signature;

		ThrowIfFailed(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
		ThrowIfFailed( Graphic::g_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&g_rootSignature)));
	*/
	}
	
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

		ComPtr<ID3DBlob> light_vertexShader;
		ComPtr<ID3DBlob> light_pixelShader;

#if defined(_DEBUG)
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		ComPtr<ID3DBlob>error;
		//GetCurrentDirectory();F:\YiaEngineRepos\YiaEngine\Engine\Framework\Shader\Shader.hlsl
	/*	auto hr = D3DCompileFromFile(L"E:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/DeferShader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader,&error);
		if (error != nullptr)
		{
			OutputDebugStringA((char*)error->GetBufferPointer());
		}
		ThrowIfFailed(hr);
		hr = D3DCompileFromFile(L"E:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/DeferShader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, &error);
		if (error != nullptr)
		{
			OutputDebugStringA((char*)error->GetBufferPointer());
		}
		ThrowIfFailed(hr);*/

		CompilerShader(L"E:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/DeferShader.hlsl", "VSMain", "vs_5_0", &vertexShader);
		CompilerShader(L"E:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/DeferShader.hlsl", "PSMain", "ps_5_0", &pixelShader);


		CompilerShader(L"E:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/SampleLightShader.hlsl", "VSMain", "vs_5_0", &light_vertexShader);
		CompilerShader(L"E:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/SampleLightShader.hlsl", "PSMain", "ps_5_0", &light_pixelShader);

		/*ThrowIfFailed(D3DCompileFromFile(L"F:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/DeferShader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(L"F:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/DeferShader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));*/


		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1,0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }

		};
		D3D12_INPUT_ELEMENT_DESC inputElementDescs_normal[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1,0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2,0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		/*D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};*/
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
		inputLayoutDesc.NumElements = _countof(inputElementDescs);
		inputLayoutDesc.pInputElementDescs = inputElementDescs;

		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_normal = {};
		inputLayoutDesc_normal.NumElements = _countof(inputElementDescs_normal);
		inputLayoutDesc_normal.pInputElementDescs = inputElementDescs_normal;

		CD3DX12_RASTERIZER_DESC wirframe_rasterized_desc(D3D12_FILL_MODE_WIREFRAME, D3D12_CULL_MODE_NONE, 
			false, 0, 0, 0, true, false, true, 0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
		CD3DX12_RASTERIZER_DESC defalut_rasterized_desc(D3D12_DEFAULT);
	
		
		pso.SetRasterizerState(defalut_rasterized_desc);
		pso.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
		pso.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);                
		pso.SetDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
		pso.SetInputLayout(_countof(inputElementDescs), inputElementDescs);
		pso.SetPixelShader(reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize());
		pso.SetVertexShader(reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize());
		pso.SetRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
		pso.SetSampleDesc(1, 0);
		pso.SetSampleMask(UINT_MAX);
		pso.SetRootSignature(rootSignature);
		pso.Finalize();

		lightpso = pso;
		lightpso.SetVertexShader(light_vertexShader->GetBufferPointer(), light_vertexShader->GetBufferSize());
		lightpso.SetPixelShader(light_pixelShader->GetBufferPointer(), light_pixelShader->GetBufferSize());
		lightpso.SetInputLayout(_countof(inputElementDescs_normal), inputElementDescs_normal);

	}

	{

	}
	ThrowIfFailed( Graphic::g_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_commandAllocator.Get(), pso.RawPipelineStateObject(), IID_PPV_ARGS(&g_commandList)));
	g_commandList->SetName(L"G_CommandList");
	ThrowIfFailed(g_commandList->Close());

	{
		
		g_mode = YiaEngine::AssetLoder::Instance()->LoadModel("Fox/Fox.gltf");

		Vertex_Pos_Uv triangleVertices[] =
		{
			{ { -1.0f, 1.f , 0.0f },{0,0} },
			{ { 1.0f, 1.f , 0.0f }, {1,0} },
			{ { -1.f, -1.f , 0.0f }, {0,1}},

			{ { -1.f, -1.f , 0.0f }, {0,1}},
			{ { 1.f, 1.f , 0.0f }, {1,0}},
			{ { 1.f, -1.f , 0.0f }, {1,1}}
		};
		YiaEngine::Vec3f pos[] = {
			{ -1.0f, 1.f , 0.0f },
			{ 1.0f, 1.f , 0.0f },
			{ -1.f, -1.f , 0.0f },
			{ -1.f, -1.f , 0.0f },
			{ 1.f, 1.f , 0.0f },
			{ 1.f, -1.f , 0.0f }
		};
		YiaEngine::Vec2f uv[] =
		{
			{0,0},
			{1,0},
			{0,1},
			{0,1},
			{1,0},
			{1,1}
		};


		auto mesh = g_mode->Object()->GetMeshObject(0);
		
	//	meshe->Vertices

	
		auto positiones = mesh->GetPosition();
		auto uvs = mesh->GetUVs();
		auto normals = mesh->GetNormal();
	

		
		BindVertexAttribute(positiones.data(), positiones.data_size(), positiones.stride(), 0);

		BindVertexAttribute(uvs.data(), uvs.data_size(), uvs.stride(), 1);
		//BindVertexAttribute(normals.data(), normals.data_size(), normals.stride(), 2);
		auto indexarray = mesh->GetIndexArray(0);
		BindIndexBuffer(indexarray.data(), indexarray.data_size());
		UINT dateSize =  positiones.data_size() + uvs.data_size() + indexarray.data_size();

	/*	Graphic::CommandContext* initContext = Graphic::CommandContext::Begin(L"initMesh");
		Graphic::AllocateBuffer uploadBuffer = initContext->GetAllocateUploadBuffer(dateSize);
		UINT8* begin;
		UINT8* dest = begin = (UINT8*)uploadBuffer.CpuAddress;
		dest = AlignUp(dest, sizeof(float));
		size_t posOffset = dest - begin;
		memcpy(dest, (UINT8*)positiones.data(), positiones.data_size());
		dest += positiones.data_size();

		dest = AlignUp(dest, sizeof(float));
		size_t uvsOffset = dest - begin;
		memcpy(dest, uvs.data(), uvs.data_size());
		dest += uvs.data_size();
		
		dest = AlignUp(dest, sizeof(float));
		size_t indexOffset = dest - begin;
		memcpy(dest, (UINT8*)indexarray.data(), indexarray.data_size());
		
		Graphic::GpuBuffer meshBuffer;
		meshBuffer.Create(dateSize,1);
		initContext->TransitionBarrier(meshBuffer, D3D12_RESOURCE_STATE_COPY_DEST);
		ASSERT_SUCCEEDED(UpdateSubresources<1>(initContext->RawCommandList(), meshBuffer.RawResource(), uploadBuffer.Buffer.RawResource(), 0, 0, 1, nullptr));
		initContext->TransitionBarrier(meshBuffer, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		
		initContext->End();

		mesh->TMP_SetMeshData(meshBuffer);
		mesh->TMP_SetVertexBufferView(0, meshBuffer.VertexBufferView(posOffset,positiones.stride(),positiones.data_size()));
		mesh->TMP_SetVertexBufferView(1, meshBuffer.VertexBufferView(uvsOffset, uvs.stride(), uvs.data_size()));
		mesh->TMP_SetIndexBufferView(meshBuffer.IndexBufferView(indexOffset, sizeof(int), indexarray.data_size()));*/
		
		//{
		//	Vec3f d
		//}

		//D3D12_DESCRIPTOR_HEAP_DESC srv_cbvHeapdesc = {};
		//srv_cbvHeapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		//srv_cbvHeapdesc.NumDescriptors = 2;
		//srv_cbvHeapdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		//ThrowIfFailed( Graphic::g_device->CreateDescriptorHeap(&srv_cbvHeapdesc, IID_PPV_ARGS(&g_SRVCBVHeap)));
		srv_cbv_heap_.CreateShaderVisibleType(L"Shader_Var_Heap", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10);

		{
	
			
		}
	
		editor_camera =std::unique_ptr<Scene::CameraNode>(new Scene::CameraNode(YiaEngine::Vec3f(190,50,0))) ;
		editor_camera->set_front(YiaEngine::Vec3f(-1, 0,0));
		VPConstBuffer vp_cbuffer;
		vp_cbuffer.VPMat = editor_camera->ViewProjMatrix();
		{
			auto tmp_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			auto tmp_buffer_desc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(VPConstBuffer));
			ThrowIfFailed( Graphic::g_Device->CreateCommittedResource(&tmp_properties,
				D3D12_HEAP_FLAG_NONE,
				&tmp_buffer_desc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr, IID_PPV_ARGS(&g_cbv))
			);

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
			cbv_desc.BufferLocation = g_cbv->GetGPUVirtualAddress();
			cbv_desc.SizeInBytes = sizeof(VPConstBuffer);
		//	 Graphic::g_device->CreateConstantBufferView(&cbv_desc, g_SRVCBVHeap->GetCPUDescriptorHandleForHeapStart());
			
			Graphic::DescriptorHandle cb_handle = srv_cbv_heap_.Alloc(1);


			cpu_cbv_handle = Graphic::CpuDescriptorAllocator::AllocateDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			Graphic::g_Device->CreateConstantBufferView(&cbv_desc, cpu_cbv_handle);

			//gpuCbvHandle_ = viewDescriptorAllocator.CopyToGpuDescriptor(1, cpu_cbv_handle.GetCpuAddress());


			CD3DX12_RANGE range(0, 0);
			UINT8* buffer_begin;
			ThrowIfFailed(g_cbv->Map(0, &range, reinterpret_cast<void**>(&buffer_begin)));
			memcpy(buffer_begin, &vp_cbuffer, sizeof(VPConstBuffer));
		}

		{
		
			scene_window_ = std::unique_ptr<SceneWindow>(new SceneWindow("Scene"));
		}

		YiaEngine::BmpParser bmpParser;
		/*Image ima = bmpParser.Parser(assetLoder.OpenAndReadBinary("bb.bmp"));
		Image* image =& ima;*/
		auto image  = YiaEngine::AssetLoder::Instance()->LoadImageFile("Fox/Texture.png");
		

		ComPtr<ID3D12GraphicsCommandList> tmpList;
		

		/*ThrowIfFailed( Graphic::g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_commandAllocator.Get(), g_pipelineState.Get(), IID_PPV_ARGS(&tmpList)));

		ThrowIfFailed(tmpList->Close());*/


//		ThrowIfFailed(g_commandAllocator->Reset());
//		ThrowIfFailed(g_commandList->Reset(g_commandAllocator.Get(), g_pipelineState.Get()));
		

		LoadTextureBuffer(image, srv_cbv_heap_,1,&g_texture);
		AlbedoTex.InitializeByImage(image.get());
		//LoadTextureBuffer(image, g_ImGui_SrvCbvHeap.Get(), 2, &g_texture);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = g_texture->GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = g_texture->GetDesc().MipLevels;
		UINT srv_desc_size =  Graphic::g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CD3DX12_CPU_DESCRIPTOR_HANDLE desc_handle(g_ImGui_SrvCbvHeap->GetCPUDescriptorHandleForHeapStart(), 2, srv_desc_size);
		// Graphic::g_device->CreateShaderResourceView(g_texture.Get(), &srvDesc, gui_srv_heap.Alloc());
		//std::vector<UINT8> texture	= GenerateTextureData();

		//D3D12_RESOURCE_DESC desc;
		//auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM,image->width,image->height);
		////auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, TextureWidth, TextureHeight);

		//auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		//ThrowIfFailed( Graphic::g_device->CreateCommittedResource(
		//	&heap_properties,
		//	D3D12_HEAP_FLAG_NONE,
		//	&desc,
		//	D3D12_RESOURCE_STATE_COPY_DEST,
		//	nullptr,
		//	IID_PPV_ARGS(&g_texture)));
		//ComPtr<ID3D12Resource> tempraryUpload = nullptr;

		//UINT64 textureUploadBufferSize, textureUploadBufferSize2;
		// Graphic::g_device->GetCopyableFootprints(&desc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);
		//
		//textureUploadBufferSize2 = GetRequiredIntermediateSize(g_texture.Get(), 0, 1);
		//auto heap_properties1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		//auto resource_desc = CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize);

		//ThrowIfFailed( Graphic::g_device->CreateCommittedResource(
		//	&heap_properties1,
		//	D3D12_HEAP_FLAG_NONE,
		//	&resource_desc,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&tempraryUpload)));

		////D3D12_SUBRESOURCE_DATA initData = { &texture[0], TextureWidth * 4,TextureWidth * 4 * TextureHeight };
		//D3D12_SUBRESOURCE_DATA initData = { image->pData, image->pitch,image->data_size };
		//UpdateSubresources<1>(g_commandList.Get(), g_texture.Get(), tempraryUpload.Get(), 0, 0, 1, &initData);
		//
		//auto barrise = CD3DX12_RESOURCE_BARRIER::Transition(
		//	g_texture.Get(),
		//	D3D12_RESOURCE_STATE_COPY_DEST,
		//	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		//g_commandList->ResourceBarrier(1, &barrise);
		//
		//// now we create a shader resource view (descriptor that points to the texture and describes it)
		//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//srvDesc.Format = desc.Format;
		//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		//srvDesc.Texture2D.MipLevels = 1;
		// Graphic::g_device->CreateShaderResourceView(g_texture.Get(), &srvDesc, g_SRVHeap->GetCPUDescriptorHandleForHeapStart());


		
	}
	{
	ThrowIfFailed( Graphic::g_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&g_BundleAllocator)));
	 Graphic::g_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, g_BundleAllocator.Get(), pso.RawPipelineStateObject(), IID_PPV_ARGS(&g_BundleList));
	
	//g_BundleList->SetPipelineState(g_pipelineState.Get());
	g_BundleList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//g_commandList->IASetIndexBuffer();
	g_BundleList->IASetVertexBuffers(0, 2, &g_VertexBufferView[0]);
	g_BundleList->IASetIndexBuffer(&g_IndexBufferView);
	auto mesh = g_mode->Object()->GetMeshObject(0);
	auto indexarray = mesh->GetIndexArray(0);

	g_BundleList->DrawIndexedInstanced(indexarray.count(), 1, 0, 0, 0);
	g_BundleList->Close();
	}
	
	{
		sceneGpuHandle_ = Graphic::g_GpuCommonDescriptoHeap.Alloc(1);
		UINT destSize[] = { 1 };
		UINT srcSize[] = { 1 };
		Graphic::g_Device->CopyDescriptors(
			1,
			sceneGpuHandle_.GetCpuAddress(),
			destSize,
			1,
			sceneTarget.SrvCpuHandle().GetCpuAddress(),
			srcSize,
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);
	
	}

	{
		
		// Create an event handle to use for frame synchronization.
		
	}
}

void App::Render()
{
	//PopulateSceneCommandList();

	// Execute the command list.
	/*ID3D12CommandList* ppCommandLists[] = { g_commandList.Get() };
	g_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);*/

	// Present the frame.
	ThrowIfFailed(g_SwapChain->Present(1, 0));

//	WaitForPreviousFrame();
}

void App::SwapPresent()
{
	ThrowIfFailed(g_SwapChain->Present(1, 0));
	g_frameIndex = g_SwapChain->GetCurrentBackBufferIndex();
}

void App::ExecuteCommand()
{
	
	
	// Present the frame.
	
}

//void App::PopulateUICommandList()
//{
////	PopulateUI();
//
//	current_cmd_alloc = frame_resouces_[current_frame_]->cmd_list_alloctor;
//	ThrowIfFailed(current_cmd_alloc->Reset());
//	ThrowIfFailed(g_commandList->Reset(current_cmd_alloc.Get(), NULL));
//
//	ID3D12DescriptorHeap* descriptorHeaps[] = { Graphic::g_GpuCommonDescriptoHeap.RawHeap() };
//	g_commandList->SetDescriptorHeaps(1, descriptorHeaps);
//
//	auto  barrier = CD3DX12_RESOURCE_BARRIER::Transition(Graphic::g_SwapRenderTarget[g_frameIndex].RawResource(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
//	g_commandList->ResourceBarrier(1, &barrier);
//
//	//提交RTV
//	//CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_RTVHeap->GetCPUDescriptorHandleForHeapStart(), g_frameIndex, g_rtvDescriptorSize);
//	g_commandList->OMSetRenderTargets(1, Graphic::g_SwapRenderTarget[g_frameIndex].RtvCpuHandle().GetCpuAddress(), false, nullptr);
//	ClearRenderTarget(Graphic::g_SwapRenderTarget[g_frameIndex].RtvCpuHandle());
//
//	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_commandList.Get());
//	 
//	barrier = CD3DX12_RESOURCE_BARRIER::Transition(Graphic::g_SwapRenderTarget[g_frameIndex].RawResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
//	g_commandList->ResourceBarrier(1, &barrier);
//	ThrowIfFailed(g_commandList->Close());
// ID3D12CommandList* ppCommandLists[] = { g_commandList.Get() };
//g_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
//}
void App::PopulateUICommandList()
{
	 PopulateUI();

	///*current_cmd_alloc = frame_resouces_[current_frame_]->cmd_list_alloctor;
	//ThrowIfFailed(current_cmd_alloc->Reset());
	//ThrowIfFailed(g_commandList->Reset(current_cmd_alloc.Get(), NULL));*/
	//Graphic::GraphicContext& UiContext = Graphic::GraphicContext::Begin();

	//ID3D12DescriptorHeap* descriptorHeaps[] = { Graphic::g_GpuCommonDescriptoHeap.RawHeap() };
	//UiContext.RawCommandList()->SetDescriptorHeaps(1, descriptorHeaps);

	//auto  barrier = CD3DX12_RESOURCE_BARRIER::Transition(Graphic::g_SwapRenderTarget[g_frameIndex].RawResource(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//UiContext.RawCommandList()->ResourceBarrier(1, &barrier);

	////提交RTV
	////CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_RTVHeap->GetCPUDescriptorHandleForHeapStart(), g_frameIndex, g_rtvDescriptorSize);
	//UiContext.RawCommandList()->OMSetRenderTargets(1, Graphic::g_SwapRenderTarget[g_frameIndex].RtvCpuHandle().GetCpuAddress(), false, nullptr);
	////ClearRenderTarget(Graphic::g_SwapRenderTarget[g_frameIndex].RtvCpuHandle());

	//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), UiContext.RawCommandList());

	//barrier = CD3DX12_RESOURCE_BARRIER::Transition(Graphic::g_SwapRenderTarget[g_frameIndex].RawResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	//UiContext.RawCommandList()->ResourceBarrier(1, &barrier);
	//UiContext.End();
	//ThrowIfFailed(g_commandList->Close());
	
	//ID3D12CommandList* ppCommandLists[] = { g_commandList.Get() };
	//g_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}
void App::PopulateScene()
{
	Graphic::GraphicContext& drawSceneContext = Graphic::GraphicContext::Begin();
	drawSceneContext.SetRootSignature(rootSignature);
	drawSceneContext.SetPipelineState(pso);
	auto texHandle = AlbedoTex.CpuHandle();

	drawSceneContext.BindCpuDescriptor(0, 0, 1, &texHandle);
	drawSceneContext.BindCpuDescriptor(1, 0, 1,&cpu_cbv_handle);
	drawSceneContext.BindGpuDescriptor();
	drawSceneContext.SetViewPortAndScissorRects(&g_viewport, &g_scissorRect);
	Graphic::GpuResource scene(g_scene_tex.Get());
	//drawSceneContext.TransitionBarrier(scene, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	drawSceneContext.TransitionBarrier(sceneTarget, D3D12_RESOURCE_STATE_RENDER_TARGET);
	drawSceneContext.SetRenderTarget(sceneTarget.RtvCpuHandlePtr(), nullptr);
	//drawSceneContext.SetRenderTarget(scene_rtv_handle.GetCpuAddress(), nullptr);
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	drawSceneContext.ClearRenderTarget(sceneTarget.RtvCpuHandle(), clearColor);
	//drawSceneContext.ClearRenderTarget(scene_rtv_handle, clearColor);
	drawSceneContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto mesh = g_mode->Object()->GetMeshObject(0);
	drawSceneContext.SetIndexBuffer(&g_IndexBufferView);
	//drawSceneContext.SetIndexBuffer(mesh->IndexView());
	drawSceneContext.SetVertexBuffers(0, 2,&g_VertexBufferView[0]);
	//drawSceneContext.SetVertexBuffers(0, 2, mesh->VertexView(0));

	
	auto indexarray = mesh->GetIndexArray(0);

	drawSceneContext.DrawIndexInstance(indexarray.count(), 1, 0, 0, 0); 

	//drawSceneContext.ExecuteBundle(g_BundleList.Get());
	drawSceneContext.TransitionBarrier(sceneTarget,  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	//drawSceneContext.TransitionBarrier(scene, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	drawSceneContext.End();
}

void App::PopulateUI()
{
	Graphic::GraphicContext& UiContext = Graphic::GraphicContext::Begin();
	UiContext.SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,Graphic::g_GpuCommonDescriptoHeap.RawHeap());
	////
	UiContext.TransitionBarrier(Graphic::g_SwapRenderTarget[g_frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET);
	UiContext.SetRenderTarget(Graphic::g_SwapRenderTarget[g_frameIndex].RtvCpuHandlePtr(), nullptr);
	UiContext.ClearRenderTarget(Graphic::g_SwapRenderTarget[g_frameIndex].RtvCpuHandle());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), UiContext.RawCommandList());

	UiContext.TransitionBarrier(Graphic::g_SwapRenderTarget[g_frameIndex], D3D12_RESOURCE_STATE_PRESENT);
	//barrier = CD3DX12_RESOURCE_BARRIER::Transition(Graphic::g_SwapRenderTarget[g_frameIndex].RawResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	//UiContext.RawCommandList()->ResourceBarrier(1, &barrier);
	UiContext.End();
}

void App::PopulateSceneCommandList()
{

}
void App::ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_)
{
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	g_commandList->ClearRenderTargetView(rtvHandle_, clearColor, 0, nullptr);
}
void App::LoadTextureBuffer(const std::shared_ptr<YiaEngine::Image>& image, Graphic::DescriptorHeap descriptor_heap, UINT offset, ID3D12Resource** texture_buffer)
{

	//D3D12_RESOURCE_DESC desc;
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, image->width, image->height);
	//auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, TextureWidth, TextureHeight);

	auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	Graphic::GpuResource target;
	ThrowIfFailed(Graphic::g_Device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(texture_buffer)));

	Graphic::GpuResource texture;

	auto copy_command = Graphic::CommandContext::Begin();


	ComPtr<ID3D12Resource> tempraryUpload = nullptr;

	UINT64 textureUploadBufferSize;
	textureUploadBufferSize = GetRequiredIntermediateSize(*texture_buffer, 0, 1);
	auto uplaod_buffer = copy_command->GetAllocateUploadBuffer(textureUploadBufferSize);





	//loadTextureContext->Begin();
	//D3D12_SUBRESOURCE_DATA initData = { &texture[0], TextureWidth * 4,TextureWidth * 4 * TextureHeight };
	D3D12_SUBRESOURCE_DATA initData = { image->pData, image->pitch,image->data_size };
	//UpdateSubresources<1>(copy_command->command_list(), *texture_buffer, tempraryUpload.Get(), 0, 0, 1, &initData);
	ASSERT_SUCCEEDED(UpdateSubresources<1>(copy_command->RawCommandList(), *texture_buffer, uplaod_buffer.Buffer.RawResource(), 0, 0, 1, &initData));
	//UpdateSubresources<1>(copy_command->command_list(), *texture_buffer, tempraryUpload.Get(), 0, 0, 1, &initData);

	auto barrise = CD3DX12_RESOURCE_BARRIER::Transition(
		*texture_buffer,
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	copy_command->RawCommandList()->ResourceBarrier(1, &barrise);


	copy_command->End();
	// now we create a shader resource view (descriptor that points to the texture and describes it)
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	//UINT srv_desc_size =   Graphic::g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//CD3DX12_CPU_DESCRIPTOR_HANDLE desc_handle(descriptor_heap.Alloc(1), 1, srv_desc_size);
	cpu_texture_handle = Graphic::CpuDescriptorAllocator::AllocateDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	Graphic::g_Device->CreateShaderResourceView(*texture_buffer, &srvDesc, cpu_texture_handle);
	/*D3D12_CPU_DESCRIPTOR_HANDLE cpuHandles[] = { cpu_texture_handle };
	gpuTextureHandle_ = viewDescriptorAllocator.CopyToGpuDescriptor(1, cpuHandles);*/


	/*ThrowIfFailed(g_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { g_commandList.Get() };

	g_CommandQueue->ExecuteCommandLists(1, ppCommandLists);

	WaitForPreviousFrame();*/

	//loadTextureContext->End();
}

void App::Destroy()
{
	WaitForPreviousFrame();
	CloseHandle(g_fenceEvent);
}

void App::UpdateGPUResource()
{
	VPConstBuffer vp_cbuffer;
	vp_cbuffer.VPMat = editor_camera->ViewProjMatrix();
	{
		CD3DX12_RANGE range(0, 0);
		UINT8* buffer_begin;
		ThrowIfFailed(g_cbv->Map(0, &range, reinterpret_cast<void**>(&buffer_begin)));
		memcpy(buffer_begin, &vp_cbuffer, sizeof(VPConstBuffer));
	}
}
void App::Update()
{
	static Vec2f last_drag_delta = 0;
	if (YiaEngine::Input::IsKeyDown(YiaEngine::KeyCodeEnum::W))
	{
		editor_camera->MoveForward(Time::DetalTime*20);
	}
	if (YiaEngine::Input::IsKeyDown(YiaEngine::KeyCodeEnum::S))
	{
		editor_camera->MoveForward(-Time::DetalTime * 20);
	}
	if (YiaEngine::Input::IsKeyDown(YiaEngine::KeyCodeEnum::A))
	{
		editor_camera->Strafe(-Time::DetalTime * 20);
	}
	if (YiaEngine::Input::IsKeyDown(YiaEngine::KeyCodeEnum::D))
	{
		editor_camera->Strafe(Time::DetalTime * 20);
	}
	Vec2f mouse_delta = Input::MouseDragDelta();
	//mouse_delta = mouse_delta - last_drag_delta;
//	last_drag_delta = 0;
	if (!mouse_delta.IsZero()) {
		/*mouse_delta.x /= g_width;
		mouse_delta.x *= 3.14;
		mouse_delta.y /= g_height;
		mouse_delta.y *= 3.14;*/
		//mouse_delta.x = std::fmaxf(mouse_delta.x, 10.f);
		mouse_delta.x =std::fmaxf(-30.f, std::fminf(mouse_delta.x, 30));
		mouse_delta.y = std::fmaxf(-30.f, std::fminf(mouse_delta.y, 30));
		//float delta_radians_x = Angle2Rad(mouse_delta.x * 0.1);
		float delta_radians_x = Angle2Rad(mouse_delta.x*0.01);
		float delta_radians_y = Angle2Rad(mouse_delta.y * 0.01);
		
		
		editor_camera->RotationEulerDebug(0, delta_radians_x, delta_radians_y);
	//	last_drag_delta = mouse_delta;
	}

	/*
	* if(Input.IsKeyDown(KeyCodeEnum.W))
	*	editor_camera->MoveForward(Time.DetalTime)
	* if(!Input.MouseDetal.IsZero())
	*	editor_camera->Rotation(Input.MouseDetal);
	*/
	UpdateGPUResource();
}

void App::BindVertexAttribute(void* data, size_t data_size,size_t stride ,int index)
{
	const UINT vertexBufferSize = data_size;
	CD3DX12_HEAP_PROPERTIES heapProerties(D3D12_HEAP_TYPE_UPLOAD);
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
	ThrowIfFailed( Graphic::g_Device->CreateCommittedResource(
		&heapProerties,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&g_vertexBuffer[index])));
	float* pVertexBegin;
	CD3DX12_RANGE readRange(0, 0);
	ThrowIfFailed(g_vertexBuffer[index]->Map(0, &readRange, reinterpret_cast<void**>(&pVertexBegin)));
	memcpy(pVertexBegin, (UINT8*)data, vertexBufferSize);
	g_vertexBuffer[index]->Unmap(0, nullptr);
	g_VertexBufferView[index].BufferLocation = g_vertexBuffer[index]->GetGPUVirtualAddress();
	g_VertexBufferView[index].StrideInBytes = stride;
	g_VertexBufferView[index].SizeInBytes = vertexBufferSize;


}

void App::BindIndexBuffer(void* data, size_t data_size)
{

	const UINT indexBufferSize = data_size;
	CD3DX12_HEAP_PROPERTIES heapProerties(D3D12_HEAP_TYPE_UPLOAD);
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
	ThrowIfFailed( Graphic::g_Device->CreateCommittedResource(
		&heapProerties,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&g_indexBuffer)));
	float* pIndexBegin;
	CD3DX12_RANGE readRange(0, 0);
	ThrowIfFailed(g_indexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexBegin)));
	memcpy(pIndexBegin, (UINT8*)data, indexBufferSize);
	g_indexBuffer->Unmap(0, nullptr);
	g_IndexBufferView.BufferLocation = g_indexBuffer->GetGPUVirtualAddress();
	g_IndexBufferView.SizeInBytes = indexBufferSize;
	g_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
}

//void App::BindVertexAttribute(void* data, size_t data_size, int index)
//{
//}



void App::DrawUI()
{
	static bool initUI = true;
	
	

	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y + 20), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(600, 650), ImGuiCond_Always);
	ImGuiWindowFlags window_flag_ = ImGuiWindowFlags_None;
	window_flag_ |= ImGuiWindowFlags_NoMove;
	bool open = true;
	ImGui::Begin("Scene", &open, window_flag_);


	ImGui::Image((ImTextureID)sceneGpuHandle_.GetGpuAddress()->ptr, Vec2f(600, 600));
	
	ImGuiWindow * window = ImGui::GetCurrentWindow();
	/*window->DrawList->AddCallback([](const ImDrawList* parent_list, const ImDrawCmd* cmd) {
		UINT pSrcDescriptorRangeSizes[1];
		UINT pDestDescriptorRangeSizes[1] = { 1 };
		
		pSrcDescriptorRangeSizes[1] = 1;
		
		Graphic::g_Device->CopyDescriptors(
			1,
			cmd->TextureId,
			pDestDescriptorRangeSizes,
			count,
			cpuHandle,
			pSrcDescriptorRangeSizes,
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);
		}, (ImTextureID)sceneTarget.SrvCpuHandle().GetCpuAddress());*/

	
	ImGui::End();


//	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x+650, main_viewport->WorkPos.y + 20), ImGuiCond_Always);
//	ImGui::SetNextWindowSize(ImVec2(300, 350), ImGuiCond_Always);
//	window_flag_ = ImGuiWindowFlags_None;
////	window_flag_ |= ImGuiWindowFlags_NoMove;
//	ImGui::Begin("Scene2", &open, window_flag_);
//
//	ImGui::Image((ImTextureID)sceneTarget.SrvCpuHandle().GetCpuAddress()->ptr, Vec2f(600, 600));
//	
//	ImGui::End();
	//scene_window_->TEMP_SetShowRT(sceneTarget.SrvCpuHandle());
	//scene_window_->Show();
	//ImGui::Begin("desc_handle2");

	//ImGui::Image((ImTextureID)desc_handle2.ptr, Vec2f(300, 300));
	//ImGui::End();
	
	ImGui::Render();
	
	//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_commandList.Get());

}

