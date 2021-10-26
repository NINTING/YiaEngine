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
#ifdef USE_PIX
		const wchar_t* filename = L"D:/Yia/YiaEngine/Shader/SampleShader.hlsl";
#else
		const wchar_t* filename = L"../Shader/SampleShader.hlsl";
#endif

		loadDesc.StageLoadDesc[0] = { filename,Graphic::Shader_Stage_Vertex,L"vs_6_2" };
		loadDesc.StageLoadDesc[1] = { filename,Graphic::Shader_Stage_Pixel,L"ps_6_2" };

		

	
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
		UINT index[] = { 0,1,2,3,4,5 };
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
		pso.SetShader(sampleShader);
		
		//pso.SetRenderTarget();
		pso.Finalize();
		 
		VertexAttributeArray positionAttr = CreateVertexAttribute(VertexAttributeEnum::kPosition,DataFormate::kFloat_3,6,pos);
		VertexAttributeArray uvAttr = CreateVertexAttribute(VertexAttributeEnum::kTexcoord, DataFormate::kFloat_2, 6, uv);
		fullScreenRect.SetName("FullScreenRect");
		fullScreenRect.AddAttribute(positionAttr);
		fullScreenRect.AddAttribute(uvAttr);
		fullScreenRect.AddIndices(6, index);
		fullScreenRect.CreateMeshGpuBuffer();
		//Graphic::UploadBuffer upload;

		//size_t gemoSize =positionAttr.DataSize + uvAttr.DataSize;

		//upload.Create(L"UploadGemoBuffer", sizeof(uv) + sizeof(pos));
		//uint8_t* memory = (uint8_t*)upload.Map();

		//memcpy(memory, positionAttr.Data.get(), positionAttr.DataSize);
		//memcpy(memory + positionAttr.DataSize, uvAttr.Data.get(), uvAttr.DataSize);
		//
		//upload.UnMap();

		//gemoBuffer.Create(L"triangle gemo", gemoSize, 1, upload);

		/*posVbo = gemoBuffer.VertexBufferView(0, positionAttr.Stride, positionAttr.DataSize);
		uvVbo = gemoBuffer.VertexBufferView(positionAttr.DataSize, uvAttr.Stride, uvAttr.DataSize);*/

		posVbo = fullScreenRect.MeshBuffer().VertexBufferView(0, positionAttr.Stride, positionAttr.DataSize);
		uvVbo = fullScreenRect.MeshBuffer().VertexBufferView(positionAttr.DataSize, uvAttr.Stride, uvAttr.DataSize);
		ibo = fullScreenRect.MeshBuffer().IndexBufferView(positionAttr.DataSize + uvAttr.DataSize,sizeof(UINT),sizeof(index));

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

		DefaultRenderer.Begin();
		DefaultRenderer.SetRenderTarget(&sceneColorBuffer);
		DefaultRenderer.ClearRenderTarget();
		DefaultRenderer.SetRootSignature(signature);
		DefaultRenderer.DrawMesh(fullScreenRect, pso, sampleShader);
		DefaultRenderer.End();


	}
	D3D12_VERTEX_BUFFER_VIEW posVbo;
	D3D12_VERTEX_BUFFER_VIEW uvVbo;
	D3D12_INDEX_BUFFER_VIEW ibo;
	
	Graphic::PipelineStateObject pso;

	Graphic::GpuBuffer gemoBuffer;
	CD3DX12_VIEWPORT viewport;
	CD3DX12_RECT scissorRect;
	Graphic::RootSignature signature;
	Mesh fullScreenRect;
	Graphic::Shader sampleShader;
	Graphic::Renderer DefaultRenderer;
};


Application* CreateApplication()
{
	return new SampleApp();
}