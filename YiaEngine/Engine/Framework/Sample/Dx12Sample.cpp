//渲染基本设置




#include"Dx12Sample.h"
#include"BmpParser.h"
#include"AssetLoder.h"
#include"SceneObject.h"



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



void App::WaitForPreviousFrame()
{
	//float fence = g_fenceValue;
	


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

	
	g_frameIndex = g_SwapChain->GetCurrentBackBufferIndex();

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
		IID_PPV_ARGS(&g_device)
	));

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(g_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&g_CommandQueue)));


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
	ThrowIfFailed(factory->CreateSwapChain(g_CommandQueue.Get(), &swapChainDesc, &swapChain));

	ThrowIfFailed(swapChain.As(&g_SwapChain));
	
	//控制窗口切换(alt-enter)
	ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

	g_frameIndex = g_SwapChain->GetCurrentBackBufferIndex();

	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.NumDescriptors = 2;
		ThrowIfFailed(g_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_RTVHeap)));

		g_rtvDescriptorSize = g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_RTVHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < 2; i++)
		{
			ThrowIfFailed(g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&g_renderTargets[i])));
			g_device->CreateRenderTargetView(g_renderTargets[i].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, g_rtvDescriptorSize);
		}
	
	}
	std::wstring c_name;
	
	for (int i = 0; i < frames_count_; i++)
	{
		c_name = L"frame_command";
		c_name.append(std::to_wstring(i));
		
		frame_resouces_.push_back( std::make_unique<FrameResource>(g_device.Get(),c_name.c_str()));
	}

	ThrowIfFailed(g_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocator)));

	ThrowIfFailed(g_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_Fence)));
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

		if (FAILED(g_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
		{
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}

		CD3DX12_STATIC_SAMPLER_DESC linearClamp(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
		//CD3DX12_STATIC_SAMPLER_DESC linearClamp(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
	

		CD3DX12_DESCRIPTOR_RANGE1 rootRange[2];
		rootRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		rootRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		/*CD3DX12_ROOT_PARAMETER para[1];
		para[0].InitAsDescriptorTable(1, rootPara,D3D12_SHADER_VISIBILITY_PIXEL);*/


		CD3DX12_ROOT_PARAMETER1  rootParameters[2];
		rootParameters[0].InitAsDescriptorTable(1, &rootRange[0], D3D12_SHADER_VISIBILITY_PIXEL);
		rootParameters[1].InitAsDescriptorTable(1, &rootRange[1], D3D12_SHADER_VISIBILITY_ALL);
	
			

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc;

		rootDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &linearClamp, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
				ComPtr<ID3DBlob> error;
		ComPtr<ID3DBlob> signature;

		ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootDesc, featureData.HighestVersion, &signature, &error));
		ThrowIfFailed(g_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&g_rootSignature)));

		/*CD3DX12_ROOT_SIGNATURE_DESC rootDesc;
		rootDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		ComPtr<ID3DBlob> error;
		ComPtr<ID3DBlob> signature;

		ThrowIfFailed(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
		ThrowIfFailed(g_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&g_rootSignature)));
	*/
	}
	
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		ComPtr<ID3DBlob>error;
		//GetCurrentDirectory();F:\YiaEngineRepos\YiaEngine\Engine\Framework\Shader\Shader.hlsl
		auto hr = D3DCompileFromFile(L"E:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/DeferShader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader,&error);
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
		ThrowIfFailed(hr);

		/*ThrowIfFailed(D3DCompileFromFile(L"F:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/DeferShader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(L"F:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/DeferShader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));*/


		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1,0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }

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

		CD3DX12_RASTERIZER_DESC wirframe_rasterized_desc(D3D12_FILL_MODE_WIREFRAME, D3D12_CULL_MODE_NONE, 
			false, 0, 0, 0, true, false, true, 0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
		CD3DX12_RASTERIZER_DESC defalut_rasterized_desc(D3D12_DEFAULT);
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = inputLayoutDesc;
		psoDesc.pRootSignature = g_rootSignature.Get();
		psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
		psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
		psoDesc.RasterizerState = defalut_rasterized_desc;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1; 
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;

		ThrowIfFailed(g_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_pipelineState)));
			
	}
	ThrowIfFailed(g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_commandAllocator.Get(), g_pipelineState.Get(), IID_PPV_ARGS(&g_commandList)));
	g_commandList->SetName(L"G_CommandList");
	//ThrowIfFailed(g_commandList->Close());

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

		/*const UINT vertexBufferSize = sizeof(triangleVertices);
		CD3DX12_HEAP_PROPERTIES heapProerties(D3D12_HEAP_TYPE_UPLOAD);
		auto desc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		ThrowIfFailed(g_device->CreateCommittedResource(
			&heapProerties,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&g_vertexBuffer)));
		UINT8* pVertexBegin;
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(g_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexBegin)));
		memcpy(pVertexBegin, triangleVertices, sizeof(triangleVertices));
		g_vertexBuffer->Unmap(0, nullptr);
		g_VertexBufferView.BufferLocation = g_vertexBuffer->GetGPUVirtualAddress();
		g_VertexBufferView.StrideInBytes = sizeof(Vertex_Pos_Uv);
		g_VertexBufferView.SizeInBytes = vertexBufferSize;*/
		auto positiones = mesh->GetPosition();
		auto uvs = mesh->GetUVs();
		BindVertexAttribute(positiones.data(), positiones.data_size(), positiones.stride(), 0);
		BindVertexAttribute(uvs.data(), uvs.data_size(), uvs.stride(), 1);

		auto indexarray = mesh->GetIndexArray(0);
		BindIndexBuffer(indexarray.data(), indexarray.data_size());
		

		//{
		//	Vec3f d
		//}

		D3D12_DESCRIPTOR_HEAP_DESC srv_cbvHeapdesc = {};
		srv_cbvHeapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srv_cbvHeapdesc.NumDescriptors = 2;
		srv_cbvHeapdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(g_device->CreateDescriptorHeap(&srv_cbvHeapdesc, IID_PPV_ARGS(&g_SRVCBVHeap)));

		using Camera = YiaEngine::Scene::CameraNode;
		std::unique_ptr<Camera> camera = std::unique_ptr<Camera>(new Camera(YiaEngine::Vec3f(190,50,0)));
		camera->set_front(YiaEngine::Vec3f(-1, 0,0));
		VPConstBuffer vp_cbuffer;
		vp_cbuffer.VPMat = camera->ViewProjMatrix();
		{
			auto tmp_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			auto tmp_buffer_desc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(VPConstBuffer));
			ThrowIfFailed(g_device->CreateCommittedResource(&tmp_properties,
				D3D12_HEAP_FLAG_NONE,
				&tmp_buffer_desc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr, IID_PPV_ARGS(&g_cbv))
			);

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
			cbv_desc.BufferLocation = g_cbv->GetGPUVirtualAddress();
			cbv_desc.SizeInBytes = sizeof(VPConstBuffer);
			g_device->CreateConstantBufferView(&cbv_desc, g_SRVCBVHeap->GetCPUDescriptorHandleForHeapStart());
			
			CD3DX12_RANGE range(0, 0);
			UINT8* buffer_begin;
			ThrowIfFailed(g_cbv->Map(0, &range, reinterpret_cast<void**>(&buffer_begin)));
			memcpy(buffer_begin, &vp_cbuffer, sizeof(VPConstBuffer));
		}


		YiaEngine::BmpParser bmpParser;
		/*Image ima = bmpParser.Parser(assetLoder.OpenAndReadBinary("bb.bmp"));
		Image* image =& ima;*/
		auto image  = YiaEngine::AssetLoder::Instance()->LoadImageFile("Fox/Texture.png");
		

		ComPtr<ID3D12GraphicsCommandList> tmpList;
		

		/*ThrowIfFailed(g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_commandAllocator.Get(), g_pipelineState.Get(), IID_PPV_ARGS(&tmpList)));

		ThrowIfFailed(tmpList->Close());*/


//		ThrowIfFailed(g_commandAllocator->Reset());
//		ThrowIfFailed(g_commandList->Reset(g_commandAllocator.Get(), g_pipelineState.Get()));
		/*D3D12_DESCRIPTOR_HEAP_DESC srvHeapdesc = {};
		srvHeapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapdesc.NumDescriptors = 1;
		srvHeapdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(g_device->CreateDescriptorHeap(&srvHeapdesc, IID_PPV_ARGS(&g_SRVHeap)));*/

		LoadTextureBuffer(image, g_SRVCBVHeap.Get(),1,&g_texture);

		//std::vector<UINT8> texture	= GenerateTextureData();

		//D3D12_RESOURCE_DESC desc;
		//auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM,image->width,image->height);
		////auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, TextureWidth, TextureHeight);

		//auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		//ThrowIfFailed(g_device->CreateCommittedResource(
		//	&heap_properties,
		//	D3D12_HEAP_FLAG_NONE,
		//	&desc,
		//	D3D12_RESOURCE_STATE_COPY_DEST,
		//	nullptr,
		//	IID_PPV_ARGS(&g_texture)));
		//ComPtr<ID3D12Resource> tempraryUpload = nullptr;

		//UINT64 textureUploadBufferSize, textureUploadBufferSize2;
		//g_device->GetCopyableFootprints(&desc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);
		//
		//textureUploadBufferSize2 = GetRequiredIntermediateSize(g_texture.Get(), 0, 1);
		//auto heap_properties1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		//auto resource_desc = CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize);

		//ThrowIfFailed(g_device->CreateCommittedResource(
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
		//g_device->CreateShaderResourceView(g_texture.Get(), &srvDesc, g_SRVHeap->GetCPUDescriptorHandleForHeapStart());


		
	}
	{
	ThrowIfFailed(g_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&g_BundleAllocator)));
	g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, g_BundleAllocator.Get(), g_pipelineState.Get(), IID_PPV_ARGS(&g_BundleList));
	
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
		
		// Create an event handle to use for frame synchronization.
		
	}
}

void App::Render()
{
	PopulateCommandList();

	// Execute the command list.
	ID3D12CommandList* ppCommandLists[] = { g_commandList.Get() };
	g_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame.
	ThrowIfFailed(g_SwapChain->Present(1, 0));

	WaitForPreviousFrame();
}

void App::PopulateCommandList()
{
	current_cmd_alloc = frame_resouces_[current_frame_]->cmd_list_alloctor;
	

	//ThrowIfFailed(current_cmd_alloc->Reset());
	ThrowIfFailed(current_cmd_alloc->Reset());
	ThrowIfFailed(g_commandList->Reset(current_cmd_alloc.Get(), g_pipelineState.Get()));
	
	g_commandList->SetGraphicsRootSignature(g_rootSignature.Get());
	

	// set the descriptor heap
//	ID3D12DescriptorHeap* descriptorHeaps[] = { g_CBVHeap.Get() };/*
	ID3D12DescriptorHeap* descriptorHeaps[] = { g_SRVCBVHeap.Get() };
	g_commandList->SetDescriptorHeaps(1, descriptorHeaps);

	//CD3DX12_GPU_DESCRIPTOR_HANDLE tex();
	//tex.Offset(0, 0);
	UINT srv_desc_size = g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE srv_gpu_handle(g_SRVCBVHeap->GetGPUDescriptorHandleForHeapStart(), 1, srv_desc_size);
	g_commandList->SetGraphicsRootDescriptorTable(1, g_SRVCBVHeap->GetGPUDescriptorHandleForHeapStart());
	g_commandList->SetGraphicsRootDescriptorTable(0, srv_gpu_handle);
	

	g_commandList->RSSetViewports(1, &g_viewport);
	g_commandList->RSSetScissorRects(1, &g_scissorRect);


	auto  barrier = CD3DX12_RESOURCE_BARRIER::Transition(g_renderTargets[g_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	g_commandList->ResourceBarrier(1, &barrier);
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_RTVHeap->GetCPUDescriptorHandleForHeapStart(), g_frameIndex, g_rtvDescriptorSize);
	g_commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	g_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	//g_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		
	////g_commandList->IASetIndexBuffer();
	//g_commandList->IASetVertexBuffers(0, 2, &g_VertexBufferView[0]);
	//g_commandList->IASetIndexBuffer(&g_IndexBufferView);
	//auto mesh = g_mode->Object()->GetMeshObject(0);
	//auto indexarray = mesh->GetIndexArray(0);

	//g_commandList->DrawIndexedInstanced(indexarray.count(), 1, 0, 0, 0);
	// 	   
	//g_commandList->DrawInstanced(6, 1, 0, 0);
	g_commandList->ExecuteBundle(g_BundleList.Get());
	barrier = CD3DX12_RESOURCE_BARRIER::Transition(g_renderTargets[g_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	g_commandList->ResourceBarrier(1, &barrier);

	ThrowIfFailed(g_commandList->Close());
}

void App::LoadTextureBuffer(const std::shared_ptr<YiaEngine::Image>& image,
	ID3D12DescriptorHeap* descriptor_heap,UINT offset, ID3D12Resource** texture_buffer)
{

	//D3D12_RESOURCE_DESC desc;
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, image->width, image->height);
	//auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, TextureWidth, TextureHeight);

	auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	ThrowIfFailed(g_device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(texture_buffer)));
	ComPtr<ID3D12Resource> tempraryUpload = nullptr;

	UINT64 textureUploadBufferSize, textureUploadBufferSize2;
	g_device->GetCopyableFootprints(&desc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);

	textureUploadBufferSize2 = GetRequiredIntermediateSize(*texture_buffer, 0, 1);
	auto heap_properties1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resource_desc = CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize);

	ThrowIfFailed(g_device->CreateCommittedResource(
		&heap_properties1,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tempraryUpload)));

	//D3D12_SUBRESOURCE_DATA initData = { &texture[0], TextureWidth * 4,TextureWidth * 4 * TextureHeight };
	D3D12_SUBRESOURCE_DATA initData = { image->pData, image->pitch,image->data_size };
	UpdateSubresources<1>(g_commandList.Get(), *texture_buffer, tempraryUpload.Get(), 0, 0, 1, &initData);

	auto barrise = CD3DX12_RESOURCE_BARRIER::Transition(
		*texture_buffer,
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	g_commandList->ResourceBarrier(1, &barrise);

	

	// now we create a shader resource view (descriptor that points to the texture and describes it)
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	UINT srv_desc_size =  g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE desc_handle(descriptor_heap->GetCPUDescriptorHandleForHeapStart(), 1, srv_desc_size);
	g_device->CreateShaderResourceView(*texture_buffer, &srvDesc, desc_handle);

	ThrowIfFailed(g_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { g_commandList.Get() };

	g_CommandQueue->ExecuteCommandLists(1, ppCommandLists);

	WaitForPreviousFrame();
}

void App::Destroy()
{
	WaitForPreviousFrame();
	CloseHandle(g_fenceEvent);
}

void App::Update()
{
	

}

void App::BindVertexAttribute(void* data, size_t data_size,size_t stride ,int index)
{
	const UINT vertexBufferSize = data_size;
	CD3DX12_HEAP_PROPERTIES heapProerties(D3D12_HEAP_TYPE_UPLOAD);
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
	ThrowIfFailed(g_device->CreateCommittedResource(
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
	ThrowIfFailed(g_device->CreateCommittedResource(
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
