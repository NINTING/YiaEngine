#include<YiaEngine.h>
#include<Core/YiaGraphic.h>
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
	virtual void Init() 
	{
		Application::Init();
		imGuiLayer.OnAwake();
		PushLayer(&sampleLayer);
		PushLayer(&imGuiLayer);
	};
	virtual void Update()
	{
		RenderScene();


		imGuiLayer.Begin();
		imGuiLayer.OnUpdate();
		imGuiLayer.Render();
		imGuiLayer.End();
	}
	
	bool isDestroy = false;
	SampleLayer sampleLayer; 
	ImguiLayer imGuiLayer;

	void RenderScene()
	{

		Graphic::PipelineStateObject pso = Graphic::PipelineStateObject::s_DefaultPSO;
		pso.SetVertexShader(Graphic::Shader::s_DefaultShader);
		pso.SetPixelShader(Graphic::Shader::s_DefaultShader);
	

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
		
		Graphic::GpuBuffer gemoBuffer;
		Graphic::UploadBuffer upload;
		YIA_INFO("{0}", sizeof(uv) + sizeof(pos));
		size_t gemoSize = sizeof(uv) + sizeof(pos);

		upload.Create(L"UploadGemoBuffer",sizeof(uv)+sizeof(pos));
		float* memory =(float*) upload.Map();
		memcpy(memory, pos, sizeof(pos));
		memcpy(memory + 6, pos, sizeof(uv));
		upload.UnMap();

		gemoBuffer.Create(L"triangle gemo", gemoSize, 1, upload);

		D3D12_VERTEX_BUFFER_VIEW views[] = { gemoBuffer.VertexBufferView(0, 12, sizeof(pos)) ,gemoBuffer.VertexBufferView(12, 8, sizeof(uv)) };

		Graphic::GraphicContext sceneContext = Graphic::GraphicContext::Begin();
		
		sceneContext.SetVertexBuffers(0,2, views);
		sceneContext.DrawInstance(6, 1);
		sceneContext.End();
	}
};


Application* CreateApplication()
{
	return new SampleApp();
}