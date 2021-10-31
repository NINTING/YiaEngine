#include<YiaEngine.h>
#include<Core/YiaGraphic.h>
#include<ThirdParty/WinPixEventRuntime.1.0.210818001/Include/WinPixEventRuntime/pix3.h>
#include<Renderer/Renderer.h>
using namespace YiaEngine;

class SampleLayer :public Layer
{
public:
	SampleLayer():Layer("SampleLayer"){}
	virtual void OnEvent(Event& e) {
		EventListener listener(e);
		listener.Listen<MouseButtonDownEvent>([](MouseButtonDownEvent& e) {
			YIA_INFO("SampleLayer"); 
			YIA_INFO(e);
			return true;
		});
	};
	virtual void OnUpdate() {};
};

class SampleApp :public Application
{
public:
	SampleApp() :Application(), sampleLayer() {}
	virtual void OnEvent(Event& e)
	{
		EventListener listener(e);
		Application::OnEvent(e);
		listener.Listen<WindowCloseEvent>(BIND_MEMBER_CALLBACK(OnWindowCloseEvent));
	}
	void Load()
	{


		Graphic::ShaderLoadDesc loadDesc = {};
		Graphic::ShaderLoadDesc boxLoadDesc = {};
#ifdef USE_PIX
		const wchar_t* filename = L"D:/Yia/YiaEngine/Shader/SampleShader.hlsl";
		const wchar_t* boxFilename = L"D:/Yia/YiaEngine/Shader/DefaultShader.hlsl";
#else
		const wchar_t* filename = L"../Shader/SampleShader.hlsl";
		const wchar_t* boxFilename = L"../Shader/SampleShader.hlsl";
#endif

		loadDesc.StageLoadDesc[0] = { filename,Graphic::Shader_Stage_Vertex,L"vs_6_2" };
		loadDesc.StageLoadDesc[1] = { filename,Graphic::Shader_Stage_Pixel,L"ps_6_2" };
		
		boxLoadDesc.StageLoadDesc[0] = { boxFilename,Graphic::Shader_Stage_Vertex,L"vs_6_2" };
		boxLoadDesc.StageLoadDesc[1] = { boxFilename,Graphic::Shader_Stage_Pixel,L"ps_6_2" };


		Graphic::LoadShader(loadDesc, sampleShader);


		Graphic::LoadShader(boxLoadDesc, defaultShader);

		Math::Vec3f pos[] = {
			{ -1.0f, 1.f , 0.0f },
			{ 1.0f, 1.f , 0.0f },
			{ -1.f, -1.f , 0.0f },
			{ -1.f, -1.f , 0.0f },
			{ 1.f, 1.f , 0.0f },
			{ 1.f, -1.f , 0.0f }
		};


		Math::Vec2f uv[] =
		{
			{0,0},
			{1,0},
			{0,1},
			{0,1},
			{1,0},
			{1,1}
		};
		UINT index[] = { 0,1,2,3,4,5 };

		Math::Vec3f BoxPos[] = {
			//上
			{ -1.0f, 1.f , 1.0f },
			{ 1.0f, 1.f , 1.0f },
			{ 1.0f, 1.f , -1.0f },
			{ -1.0f, 1.f , -1.0f },
			//下
			{ -1.0f, -1.f , 1.0f },
			{ 1.0f, -1.f , 1.0f },
			{ 1.0f, -1.f , -1.0f },
			{ -1.0f, -1.f , -1.0f },
			//左
			{ -1.0f, 1.f , 1.0f },
			{ -1.0f, 1.f , -1.0f },
			{ -1.0f, -1.f , -1.0f },
			{ -1.0f, -1.f , 1.0f },
			//右
			{ 1.0f, 1.f , 1.0f },
			{ 1.0f, 1.f , -1.0f },
			{ 1.0f, -1.f , -1.0f },
			{ 1.0f, -1.f , 1.0f },
			//前
			{ -1.0f, 1.f , -1.0f },
			{ 1.0f, 1.f , -1.0f },
			{ 1.0f, -1.f , -1.0f },
			{ -1.0f, -1.f , -1.0f },
			//前
			{ -1.0f, 1.f ,	1.0f },
			{ 1.0f, 1.f ,	1.0f },
			{ 1.0f, -1.f ,	1.0f },
			{ -1.0f, -1.f , 1.0f },
		};
		Math::Vec2f Boxuv[] =
		{
			{0,0},
			{1,0},
			{1,1},
			{0,1},

			{0,0},
			{1,0},
			{1,1},
			{0,1},

			{0,0},
			{1,0},
			{1,1},
			{0,1},

			{0,0},
			{1,0},
			{1,1},
			{0,1},

			{0,0},
			{1,0},
			{1,1},
			{0,1},

			{0,0},
			{1,0},
			{1,1},
			{0,1},

		};
		UINT boxIndex[] = {
			0,1,3,1,2,3,
			7,5,4,7,6,5,
			8,9,11,9,10,11,
			15,12,13,15,14,13,
			16,17,19,17,18,19,
			23,21,20,23,22,21 };
		D3D12_INPUT_LAYOUT_DESC desc;
		auto& vertexInput = sampleShader.Reflect[0].VertexInput;

		desc.NumElements = sampleShader.Reflect[0].VertexInput.AttributesCount;
		D3D12_INPUT_ELEMENT_DESC elements[8];// = new D3D12_INPUT_ELEMENT_DESC[desc.NumElements];
		for (size_t i = 0; i < desc.NumElements; i++)
		{
			elements[i].SemanticName = vertexInput.Attrs[i].SemanticName;
			elements[i].SemanticIndex = vertexInput.Attrs[i].SemanticIndex;
			elements[i].Format = DataFormatToDX12(vertexInput.Attrs[i].format);
			elements[i].AlignedByteOffset = 0;
			elements[i].InputSlot = i;
			elements[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			elements[i].InstanceDataStepRate = 0;
		}

		signature.Reset(1, 0);
		signature[0].InitAsConstBufferView(D3D12_SHADER_VISIBILITY_PIXEL, 0);
		signature.Finalize(L"SampleSignature", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		pso = Graphic::PipelineStateObject::s_DefaultPSO;
		pso.SetRootSignature(signature); 

		pso.SetInputLayout(desc.NumElements, elements);
		//delete elements;

		pso.SetShader(sampleShader);

		//pso.SetRenderTarget();
		pso.Finalize();


		fullScreenRect.SetName("FullScreenRect");
		fullScreenRect.AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kPosition, DataFormate::kFloat_3, 6, pos));
		fullScreenRect.AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kTexcoord, DataFormate::kFloat_2, 6, uv));
		fullScreenRect.AddIndices(6, index);
		fullScreenRect.CreateMeshGpuBuffer();

		Box.SetName("Box");
		Box.AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kPosition, DataFormate::kFloat_3, 24, BoxPos));
		Box.AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kTexcoord, DataFormate::kFloat_2, 24, Boxuv));
		Box.AddIndices(sizeof(boxIndex) / sizeof(int), boxIndex);
		Box.CreateMeshGpuBuffer();

		BoxSignature.Reset(1, 0);
		BoxSignature[0].InitAsConstBufferView(D3D12_SHADER_VISIBILITY_VERTEX,0);
		BoxSignature.Finalize(L"BoxSignature", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT );

		CD3DX12_RASTERIZER_DESC wirframe_rasterized_desc(D3D12_FILL_MODE_WIREFRAME, D3D12_CULL_MODE_NONE,
			false, 0, 0, 0, true, false, true, 0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);

		default3DPso = Graphic::PipelineStateObject::s_DefaultPSO;
//		default3DPso.SetRootSignature(BoxSignature);
		default3DPso.SetSolideShowMode();
		//default3DPso.SetWireframeShowMode();
		default3DPso.SetBackCullMode();
		//default3DPso.SetNoneCullMode();
		default3DPso.SetInputLayout(desc.NumElements, elements);
		//delete elements;

		default3DPso.SetShader(defaultShader);

		//pso.SetRenderTarget();
		default3DPso.Finalize();

		camera.Position(Math::Vec3f(1, 0, -5));
		camera.SetFarclip(10);
		camera.SetNearclip(0.01);
		camera.Front(Math::Vec3f(0, 0, 1));
		Math::Mat4x4f viewMat = camera.ViewMatrix();
		Math::Mat4x4f projMat = camera.ProjectionMatrix();
		Math::Mat4x4f worldMat = Math::Mat4x4f::Identity();
		
		auto tmp_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto tmp_buffer_desc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Math::Vec4f));
		ASSERT_SUCCEEDED(Graphic::g_Device->CreateCommittedResource(&tmp_properties,
			D3D12_HEAP_FLAG_NONE,
			&tmp_buffer_desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&g_cbv))
		);
		finalColor = Math::Vec4f(0.5, 0, 0.5, 1);
		CD3DX12_RANGE range(0, 0);
		UINT8* buffer_begin;
		ASSERT_SUCCEEDED(g_cbv->Map(0, &range, reinterpret_cast<void**>(&buffer_begin)));
		memcpy(buffer_begin, &finalColor, sizeof(finalColor));
		g_cbv->Unmap(0, nullptr);
		//Graphic::UploadBuffer perFrameUpLoadBuffer;
		//Graphic::GpuBuffer perFrameBuffer;
		//perFrameBuffer.Create();
	}
	virtual void Init() 
	{
		Application::Init();
		imGuiLayer.OnAwake();
		PushLayer(&sampleLayer);
		PushLayer(&imGuiLayer);

		Load();
	};
		
	virtual void Update(Timestep  timestep)
	{
		
		YIA_INFO("Delta time {0}", timestep.GetSeconds());

		imGuiLayer.Begin();
		imGuiLayer.OnUpdate(timestep);


		imGuiLayer.Render();

		RenderScene();
		/*
		*	Renderer->bindTexture(TextureBuffer);
			Renderer->DrawMesh(Mesh,shader);
		*/
		imGuiLayer.End();
	}
	
	bool isDestroy = false;
	SampleLayer sampleLayer; 
	ImguiLayer imGuiLayer;

	void RenderScene()
	{
		Graphic::RenderBuffer& sceneColorBuffer = imGuiLayer.SceneColorBuffer();
		Graphic::DepthBuffer& sceneDepthBuffer = imGuiLayer.SceneDepthBuffer();
		//DefaultRenderer.Begin();
		//DefaultRenderer.SetRenderTarget(&sceneColorBuffer);
		//DefaultRenderer.SetRootSignature(signature);
		////DefaultRenderer.SetTestConstBufferView(0, g_cbv->GetGPUVirtualAddress());
		//DefaultRenderer.SetConstBufferView(0,sizeof(finalColor), &finalColor);
		//DefaultRenderer.SetCamera(camera);
		//DefaultRenderer.ClearRenderTarget();

		//DefaultRenderer.DrawMesh(fullScreenRect, pso, sampleShader);
		//DefaultRenderer.End();

		DefaultRenderer.Begin();
		DefaultRenderer.SetRenderTarget(&sceneColorBuffer, &sceneDepthBuffer);
		DefaultRenderer.SetCamera(camera);
		DefaultRenderer.ClearRenderTarget();
		DefaultRenderer.ClearDepthStencil();
		DefaultRenderer.SetRootSignature(BoxSignature);
		DefaultRenderer.DrawMesh(Box, default3DPso, sampleShader);
		DefaultRenderer.End();
		/*
		*	DefaultRenderer.BeginPass();
			DefaultRenderer.SetRenderTarget(&sceneColorBuffer, &sceneDepthBuffer);
			DefaultRenderer.SetCamera(camera);
			DefaultRenderer.ClearRenderTarget();
			DefaultRenderer.ClearDepthStencil();
			DefaultRenderer.SetRootSignature(BoxSignature);
			DefaultRenderer.DrawMesh(Box, default3DPso, sampleShader);
			DefaultRenderer.EndPass();
		*/

	}
	D3D12_VERTEX_BUFFER_VIEW posVbo;
	D3D12_VERTEX_BUFFER_VIEW uvVbo;
	D3D12_INDEX_BUFFER_VIEW ibo;
	
	Graphic::PipelineStateObject pso;
	
	Graphic::PipelineStateObject default3DPso;

	Graphic::GpuBuffer gemoBuffer;
	CD3DX12_VIEWPORT viewport;
	CD3DX12_RECT scissorRect;
	Graphic::RootSignature signature;
	Math::Vec4f finalColor = Math::Vec4f(1, 0, 0.5, 1);
	Graphic::RootSignature BoxSignature;

	Mesh fullScreenRect;
	Mesh Box;
	Graphic::Shader sampleShader;
	Graphic::Shader defaultShader;
	Graphic::Renderer DefaultRenderer;
	ComPtr<ID3D12Resource>g_cbv;
	Camera camera;
};


Application* CreateApplication()
{
	return new SampleApp();
}