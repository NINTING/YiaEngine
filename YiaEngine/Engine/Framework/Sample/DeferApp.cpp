////��Ⱦ��������
//#include"Dx12Sample.h"
//
//
//
//
//
//
//inline std::string  HrToString(HRESULT hr)
//{
//	char s_str[64] = {};
//	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
//	return std::string(s_str);
//}
//
//
//
//
//inline void ThrowIfFailed(HRESULT hr)
//{ 
//	if (FAILED(hr))
//	{
//	
//		throw HrException(hr);
//	}
//}
//
// void GetHardwareAdapter(
//	 IDXGIFactory1* pFactory,
//	 IDXGIAdapter1** ppAdapter,
//	 bool requestHighPerformanceAdapter)
// {
//	 *ppAdapter = nullptr;
//
//	 ComPtr<IDXGIAdapter1> adapter;
//
//	 ComPtr<IDXGIFactory6> factory6;
//	 if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
//	 {
//		 for (
//			 UINT adapterIndex = 0;
//			 DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
//				 adapterIndex,
//				 requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
//				 IID_PPV_ARGS(&adapter));
//			 ++adapterIndex)
//		 {
//			 DXGI_ADAPTER_DESC1 desc;
//			 adapter->GetDesc1(&desc);
//
//			 if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
//			 {
//				 // Don't select the Basic Render Driver adapter.
//				 // If you want a software adapter, pass in "/warp" on the command line.
//				 continue;
//			 }
//
//			 // Check to see whether the adapter supports Direct3D 12, but don't create the
//			 // actual device yet.
//			 if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
//			 {
//				 break;
//			 }
//		 }
//	 }
//	 else
//	 {
//		 for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
//		 {
//			 DXGI_ADAPTER_DESC1 desc;
//			 adapter->GetDesc1(&desc);
//
//			 if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
//			 {
//				 // Don't select the Basic Render Driver adapter.
//				 // If you want a software adapter, pass in "/warp" on the command line.
//				 continue;
//			 }
//
//			 // Check to see whether the adapter supports Direct3D 12, but don't create the
//			 // actual device yet.
//			 if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
//			 {
//				 break;
//			 }
//		 }
//	 }
//
//	 *ppAdapter = adapter.Detach();
// }
//
//
//
//void DeferApp::WaitForPreviousFrame()
//{
//	float fence = g_fenceValue;
//	g_CommandQueue->Signal(g_Fence.Get(), fence);
//	g_fenceValue++;
//	if (g_Fence->GetCompletedValue() < fence)
//	{
//		g_Fence->SetEventOnCompletion(fence, g_fenceEvent);
//		WaitForSingleObject(g_fenceEvent, INFINITE);
//	}
//	g_frameIndex = g_SwapChain->GetCurrentBackBufferIndex();
//
//}
//
//void DeferApp::InitEngine()
//{
//
//	g_frameIndex = 0;
//	g_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(g_width), static_cast<float>(g_height));
//	g_scissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(g_width), static_cast<LONG>(g_height));
//	g_rtvDescriptorSize = 0;
//}
//
//
//
//void DeferApp::LoadPipeline(HWND hwnd)
//{
//
//	
//#if defined DEBUG
//	{
//		ComPtr<ID3D12Debug>debugger;
//		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugger))))
//		{
//			debugger->EnableDebugLayer();
//		}
//	}
//#endif // DEBUG
//	ComPtr<IDXGIFactory4> factory;
//	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));
//	ComPtr<IDXGIAdapter1> hardwareAdapter;
//
//	GetHardwareAdapter(factory.Get(), &hardwareAdapter,false);
//	ThrowIfFailed(D3D12CreateDevice(
//		hardwareAdapter.Get(),
//		D3D_FEATURE_LEVEL_11_0,
//		IID_PPV_ARGS(&g_device)
//	));
//
//	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
//	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
//
//	ThrowIfFailed(g_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&g_CommandQueue)));
//
//
//	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
//	swapChainDesc.BufferCount = 2;
//	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	swapChainDesc.BufferDesc.Width = g_width;
//	swapChainDesc.BufferDesc.Height = g_height;
//	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//	swapChainDesc.OutputWindow = hwnd;
//	swapChainDesc.SampleDesc.Count = 1;
//	swapChainDesc.Windowed = true;
//
//	ComPtr<IDXGISwapChain> swapChain;
//	ThrowIfFailed(factory->CreateSwapChain(g_CommandQueue.Get(), &swapChainDesc, &swapChain));
//
//	ThrowIfFailed(swapChain.As(&g_SwapChain));
//	
//	//���ƴ����л�(alt-enter)
//	ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));
//
//	g_frameIndex = g_SwapChain->GetCurrentBackBufferIndex();
//
//	{
//		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
//		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
//		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//		rtvHeapDesc.NumDescriptors = 2;
//		ThrowIfFailed(g_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_RTVHeap)));
//
//		g_rtvDescriptorSize = g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//	}
//	{
//		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_RTVHeap->GetCPUDescriptorHandleForHeapStart());
//		for (UINT i = 0; i < 2; i++)
//		{
//			ThrowIfFailed(g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&g_renderTargets[i])));
//			g_device->CreateRenderTargetView(g_renderTargets[i].Get(), nullptr, rtvHandle);
//			rtvHandle.Offset(1, g_rtvDescriptorSize);
//		}
//	}
//	ThrowIfFailed(g_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocator)));
//}
//
//void DeferApp::LoadAsset()
//{
//	{
//		CD3DX12_ROOT_SIGNATURE_DESC rootDesc;
//		rootDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
//		ComPtr<ID3DBlob> error;
//		ComPtr<ID3DBlob> signature;
//
//		ThrowIfFailed(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
//		ThrowIfFailed(g_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&g_rootSignature)));
//
//	}
//
//	{
//		ComPtr<ID3DBlob> vertexShader;
//		ComPtr<ID3DBlob> pixelShader;
//
//#if defined(_DEBUG)
//		// Enable better shader debugging with the graphics debugging tools.
//		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
//#else
//		UINT compileFlags = 0;
//#endif
//		//GetCurrentDirectory();
//		ThrowIfFailed(D3DCompileFromFile(L"F:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/Shader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
//		ThrowIfFailed(D3DCompileFromFile(L"F:/YiaEngineRepos/YiaEngine/Engine/Framework/Shader/Shader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));
//
	//		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	//		{
	//			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	//		};
//		
//		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
//		psoDesc.InputLayout = { inputElementDescs ,_countof(inputElementDescs) };
//		psoDesc.pRootSignature = g_rootSignature.Get();
//		psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
//		psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
//		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
//		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
//		psoDesc.DepthStencilState.DepthEnable = FALSE;
//		psoDesc.DepthStencilState.StencilEnable = FALSE;
//		psoDesc.SampleMask = UINT_MAX;
//		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//		psoDesc.NumRenderTargets = 1;
//		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
//		psoDesc.SampleDesc.Count = 1;
//		ThrowIfFailed(g_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_pipelineState)));
//	}
//	ThrowIfFailed(g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_commandAllocator.Get(), g_pipelineState.Get(), IID_PPV_ARGS(&g_commandList)));
//
//	ThrowIfFailed(g_commandList->Close());
//
//	{
//		Vertex triangleVertices[] =
//		{
//			{ { 0.0f, 0.25f , 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
//			{ { 0.25f, -0.25f , 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
//			{ { -0.25f, -0.25f , 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
//		};
//
//		const UINT vertexBufferSize = sizeof(triangleVertices);
//		CD3DX12_HEAP_PROPERTIES heapProerties(D3D12_HEAP_TYPE_UPLOAD);
//		auto desc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
//		ThrowIfFailed(g_device->CreateCommittedResource(
//			&heapProerties,
//			D3D12_HEAP_FLAG_NONE,
//			&desc,
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&g_vertexBuffer)));
//		UINT8* pVertexBegin;
//		CD3DX12_RANGE readRange(0, 0);
//		ThrowIfFailed(g_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexBegin)));
//		memcpy(pVertexBegin, triangleVertices, sizeof(triangleVertices));
//		g_vertexBuffer->Unmap(0, nullptr);
//		g_VertexBufferView.BufferLocation = g_vertexBuffer->GetGPUVirtualAddress();
//		g_VertexBufferView.StrideInBytes = sizeof(Vertex);
//		g_VertexBufferView.SizeInBytes = vertexBufferSize;
//
//	}
//	{
//		ThrowIfFailed(g_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_Fence)));
//		g_fenceValue = 1;
//
//		// Create an event handle to use for frame synchronization.
//		g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
//		if (g_fenceEvent == nullptr)
//		{
//			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
//		}
//		WaitForPreviousFrame();
//	}
//}
//void DeferApp::Render()
//{
//	PopulateCommandList();
//
//	// Execute the command list.
//	ID3D12CommandList* ppCommandLists[] = { g_commandList.Get() };
//	g_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
//
//	// Present the frame.
//	ThrowIfFailed(g_SwapChain->Present(1, 0));
//
//	WaitForPreviousFrame();
//}
//
//void DeferApp::PopulateCommandList()
//{
//	ThrowIfFailed(g_commandAllocator->Reset());
//	ThrowIfFailed(g_commandList->Reset(g_commandAllocator.Get(), g_pipelineState.Get()));
//
//	g_commandList->SetGraphicsRootSignature(g_rootSignature.Get());
//	g_commandList->RSSetViewports(1, &g_viewport);
//	g_commandList->RSSetScissorRects(1, &g_scissorRect);
//
//	auto  barrier = CD3DX12_RESOURCE_BARRIER::Transition(g_renderTargets[g_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
//	g_commandList->ResourceBarrier(1, &barrier);
//
//	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_RTVHeap->GetCPUDescriptorHandleForHeapStart(), g_frameIndex, g_rtvDescriptorSize);
//	g_commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
//
//	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
//	g_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
//	g_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		
//	//g_commandList->IASetIndexBuffer();
//	g_commandList->IASetVertexBuffers(0, 1, &g_VertexBufferView);
//	g_commandList->DrawInstanced(3, 1, 0, 0);
//
//	barrier = CD3DX12_RESOURCE_BARRIER::Transition(g_renderTargets[g_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
//	g_commandList->ResourceBarrier(1, &barrier);
//
//	ThrowIfFailed(g_commandList->Close());
//}
//
//void DeferApp::Destroy()
//{
//	WaitForPreviousFrame();
//	CloseHandle(g_fenceEvent);
//}
//
//void DeferApp::Update()
//{
//	
//
//}