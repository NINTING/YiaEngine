#include<YiaEngine.h>
#include<Core/YiaGraphic.h>
#include<ThirdParty/WinPixEventRuntime.1.0.210818001/Include/WinPixEventRuntime/pix3.h>

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
#ifdef USE_PIX
		const wchar_t* filename = L"D:/Yia/YiaEngine/Shader/SampleShader.hlsl";
#else
		const wchar_t* filename = L"../Shader/SampleShader.hlsl";
#endif

		loadDesc.StageLoadDesc[0] = { filename,Graphic::Shader_Stage_Vertex,L"vs_6_2" };
		loadDesc.StageLoadDesc[1] = { filename,Graphic::Shader_Stage_Pixel,L"ps_6_2" };

		

		Graphic::Shader sampleShader;
		Graphic::LoadShader(loadDesc, sampleShader);

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
		
		D3D12_INPUT_LAYOUT_DESC desc;
		auto& vertexInput = sampleShader.Reflect[0].VertexInput;

		desc.NumElements = sampleShader.Reflect[0].VertexInput.AttributesCount;
		D3D12_INPUT_ELEMENT_DESC* elements = new D3D12_INPUT_ELEMENT_DESC[desc.NumElements];
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

		signature.Reset(0, 0);
		signature.Finalize(L"SampleSignature", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		pso = Graphic::PipelineStateObject::s_DefaultPSO;
		pso.SetRootSignature(signature);

		pso.SetInputLayout(desc.NumElements, elements);
		delete elements;
		pso.SetRootSignature(signature);
		pso.SetShader(sampleShader);
		
		//pso.SetRenderTarget();
		pso.Finalize();


		 
		Graphic::UploadBuffer upload;

		size_t gemoSize = sizeof(uv) + sizeof(pos);

		upload.Create(L"UploadGemoBuffer", sizeof(uv) + sizeof(pos));
		float* memory = (float*)upload.Map();
		memcpy(memory, pos, sizeof(pos));
		memcpy(memory + 18, uv, sizeof(uv));
		upload.UnMap();

		gemoBuffer.Create(L"triangle gemo", gemoSize, 1, upload);
		posVbo = gemoBuffer.VertexBufferView(0, 12, sizeof(pos));
		uvVbo = gemoBuffer.VertexBufferView(sizeof(pos), 8, sizeof(uv));
		viewport = CD3DX12_VIEWPORT(0.f, 0.f, 0.f, 0.f);
		scissorRect = CD3DX12_RECT(0.f, 0.f, 0.f, 0.f);
	}
	virtual void Init() 
	{
		Application::Init();
		imGuiLayer.OnAwake();
		PushLayer(&sampleLayer);
		PushLayer(&imGuiLayer);

		Load();
	};
	virtual void Update()
	{
		


		imGuiLayer.Begin();
		imGuiLayer.OnUpdate();


		imGuiLayer.Render();

		RenderScene();
		imGuiLayer.End();
	}
	
	bool isDestroy = false;
	SampleLayer sampleLayer; 
	ImguiLayer imGuiLayer;

	void RenderScene()
	{
		Graphic::RenderBuffer& sceneColorBuffer = imGuiLayer.SceneColorBuffer();
		/*Graphic::RenderBuffer sceneColorBuffer;

	
		auto& editorSceneColorTarget = imGuiLayer.SceneColorBuffer();
		sceneColorBuffer.Create(L"ColorTarget", editorSceneColorTarget.Size().x(), editorSceneColorTarget.Size().y(), DXGI_FORMAT_R8G8B8A8_UNORM);
		*/
		// sceneColorBuffer
		if (sceneColorBuffer.Size().x() == 0 || sceneColorBuffer.Size().y() == 0)
			return;
		
		D3D12_VERTEX_BUFFER_VIEW views[] = { posVbo,uvVbo };
	

		viewport.Width = sceneColorBuffer.Size().x();
		viewport.Height = sceneColorBuffer.Size().y();
		scissorRect.right = sceneColorBuffer.Size().x();
		scissorRect.bottom = sceneColorBuffer.Size().y();
		Graphic::GraphicContext& sceneContext = Graphic::GraphicContext::Begin();
		sceneContext.SetPipelineState(pso);
		sceneContext.SetRootSignature(signature);
		sceneContext.BindGpuDescriptor();
		sceneContext.SetVertexBuffers(0,2, views);
		sceneContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		sceneContext.SetViewPortAndScissorRects(&viewport, &scissorRect);
		sceneContext.TransitionBarrier(sceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
		sceneContext.SetRenderTarget(sceneColorBuffer.RtvCpuHandlePtr(),nullptr);
		sceneContext.ClearRenderTarget(sceneColorBuffer.RtvCpuHandle());
		sceneContext.DrawInstance(6, 1);
		sceneContext.TransitionBarrier(sceneColorBuffer, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		sceneContext.End();
	}
	D3D12_VERTEX_BUFFER_VIEW posVbo;
	D3D12_VERTEX_BUFFER_VIEW uvVbo;
	D3D12_INDEX_BUFFER_VIEW ibo;
	
	Graphic::PipelineStateObject pso;

	Graphic::GpuBuffer gemoBuffer;
	CD3DX12_VIEWPORT viewport;
	CD3DX12_RECT scissorRect;
	Graphic::RootSignature signature;
};


Application* CreateApplication()
{
	return new SampleApp();
}