//渲染基本设置
#include"Dx12Sample.h"
#include"BmpParser.h"

#include"AssetLoder.h"




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
UINT TexturePixelSize = 32;
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
			pData[n + 1] = 0x00;    // G
			pData[n + 2] = 0x00;    // B
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
		 //for (
			// UINT adapterIndex = 0;
			// DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
			//	 adapterIndex,
			//	 requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
			//	 IID_PPV_ARGS(&adapter));
			// ++adapterIndex)
		 //{
			// DXGI_ADAPTER_DESC1 desc;
			// adapter->GetDesc1(&desc);

			// if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			// {
			//	 // Don't select the Basic Render Driver adapter.
			//	 // If you want a software adapter, pass in "/warp" on the command line.
			//	 continue;
			// }

			// // Check to see whether the adapter supports Direct3D 12, but don't create the
			// // actual device yet.
			// if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			// {
			//	 break;
			// }
		 //}

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
	float fence = g_fenceValue;
	g_CommandQueue->Signal(g_Fence.Get(), fence);
	g_fenceValue++;
	if (g_Fence->GetCompletedValue() < fence)
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


	ThrowIfFailed(g_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocator)));
}

void App::LoadAsset()
{
	{

		/*CD3DX12_STATIC_SAMPLER_DESC linearClamp(0, D3D12_FILTER_MIN_MAG_MIP_POINT,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER);*/
		CD3DX12_STATIC_SAMPLER_DESC linearClamp(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
		CD3DX12_ROOT_SIGNATURE_DESC rootDesc;

		CD3DX12_DESCRIPTOR_RANGE rootPara[1];
		rootPara[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		/*CD3DX12_ROOT_PARAMETER para[1];
		para[0].InitAsDescriptorTable(1, rootPara,D3D12_SHADER_VISIBILITY_PIXEL);*/

		D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
		descriptorTable.NumDescriptorRanges = _countof(rootPara); // we only have one range
		descriptorTable.pDescriptorRanges = rootPara; // the pointer to the beginning of our ranges array
		D3D12_ROOT_PARAMETER  rootParameters[1];
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].DescriptorTable = descriptorTable; // this is the root descriptor for this root parameter
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;




		rootDesc.Init(1, rootParameters, 1, &linearClamp, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
				ComPtr<ID3DBlob> error;
		ComPtr<ID3DBlob> signature;

		ThrowIfFailed(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
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
		auto hr = D3DCompileFromFile(L"F:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/DeferShader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader,&error);
		if (error != nullptr)
		{
			OutputDebugStringA((char*)error->GetBufferPointer());
		}
		ThrowIfFailed(hr);
		hr = D3DCompileFromFile(L"F:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/DeferShader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, &error);
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
	
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }

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

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = inputLayoutDesc;
		psoDesc.pRootSignature = g_rootSignature.Get();
		psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
		psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
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

	ThrowIfFailed(g_commandList->Close());

	{
		Vertex_Pos_Uv triangleVertices[] =
		{
			{ { -1.0f, 1.f , 0.0f },{0,0} },
			{ { 1.0f, 1.f , 0.0f }, {1,0} },
			{ { -1.f, -1.f , 0.0f }, {0,1}},

			{ { -1.f, -1.f , 0.0f }, {0,1}},
			{ { 1.f, 1.f , 0.0f }, {1,0}},
			{ { 1.f, -1.f , 0.0f }, {1,1}}
		};



		const UINT vertexBufferSize = sizeof(triangleVertices);
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
		g_VertexBufferView.SizeInBytes = vertexBufferSize;

	}
	
	{

		YiaEngine::AssetLoder assetLoder;
	
		YiaEngine::BmpParser bmpParser;
		auto image = bmpParser.Parser(assetLoder.OpenAndReadBinary("bb.bmp"));
		
		ComPtr<ID3D12GraphicsCommandList> tmpList;

		/*ThrowIfFailed(g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_commandAllocator.Get(), g_pipelineState.Get(), IID_PPV_ARGS(&tmpList)));

		ThrowIfFailed(tmpList->Close());*/


		ThrowIfFailed(g_commandAllocator->Reset());
		ThrowIfFailed(g_commandList->Reset(g_commandAllocator.Get(), g_pipelineState.Get()));


		std::vector<UINT8> texture	= GenerateTextureData();

		//D3D12_RESOURCE_DESC desc;
		auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM,image.width,image.height);
		ThrowIfFailed(g_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&g_texture)));
		ComPtr<ID3D12Resource> tempraryUpload = nullptr;

		UINT64 textureUploadBufferSize, textureUploadBufferSize2;
		g_device->GetCopyableFootprints(&desc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);
		
		textureUploadBufferSize2 = GetRequiredIntermediateSize(g_texture.Get(), 0, 1);

		ThrowIfFailed(g_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&tempraryUpload)));

		//D3D12_SUBRESOURCE_DATA initData = { &texture[0], TextureWidth * TexturePixelSize,TextureWidth * TexturePixelSize * TextureHeight };
		D3D12_SUBRESOURCE_DATA initData = { image.pData, image.pitch,image.data_size };
		UpdateSubresources<1>(g_commandList.Get(), g_texture.Get(), tempraryUpload.Get(), 0, 0, 1, &initData);
		g_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			g_texture.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapdesc = {};
		srvHeapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapdesc.NumDescriptors = 1;
		srvHeapdesc.Flags =	D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(g_device->CreateDescriptorHeap(&srvHeapdesc, IID_PPV_ARGS(&g_SRVHeap)));
		
	
		// now we create a shader resource view (descriptor that points to the texture and describes it)
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		g_device->CreateShaderResourceView(g_texture.Get(), &srvDesc, g_SRVHeap->GetCPUDescriptorHandleForHeapStart());

	//	g_device->CreateShaderResourceView(g_texture.Get(), nullptr, srvHandle);

		ThrowIfFailed(g_commandList->Close());
		ID3D12CommandList* ppCommandLists[] = { g_commandList.Get() };

		g_CommandQueue->ExecuteCommandLists(1, ppCommandLists);
	}
	
	{
		ThrowIfFailed(g_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_Fence)));
		g_fenceValue = 1;

		// Create an event handle to use for frame synchronization.
		g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (g_fenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}
		WaitForPreviousFrame();
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
	ThrowIfFailed(g_commandAllocator->Reset());
	ThrowIfFailed(g_commandList->Reset(g_commandAllocator.Get(), g_pipelineState.Get()));
	
	g_commandList->SetGraphicsRootSignature(g_rootSignature.Get());
	g_commandList->RSSetViewports(1, &g_viewport);
	g_commandList->RSSetScissorRects(1, &g_scissorRect);

	// set the descriptor heap
	ID3D12DescriptorHeap* descriptorHeaps[] = { g_SRVHeap.Get() };
	g_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);


	//CD3DX12_GPU_DESCRIPTOR_HANDLE tex();
	//tex.Offset(0, 0);
	g_commandList->SetGraphicsRootDescriptorTable(0, g_SRVHeap->GetGPUDescriptorHandleForHeapStart());



	auto  barrier = CD3DX12_RESOURCE_BARRIER::Transition(g_renderTargets[g_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	g_commandList->ResourceBarrier(1, &barrier);
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_RTVHeap->GetCPUDescriptorHandleForHeapStart(), g_frameIndex, g_rtvDescriptorSize);
	g_commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	g_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	g_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		
	//g_commandList->IASetIndexBuffer();
	g_commandList->IASetVertexBuffers(0, 1, &g_VertexBufferView);
	g_commandList->DrawInstanced(6, 1, 0, 0);
	
	barrier = CD3DX12_RESOURCE_BARRIER::Transition(g_renderTargets[g_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	g_commandList->ResourceBarrier(1, &barrier);

	ThrowIfFailed(g_commandList->Close());
}

void App::Destroy()
{
	WaitForPreviousFrame();
	CloseHandle(g_fenceEvent);
}

void App::Update()
{
	

}
