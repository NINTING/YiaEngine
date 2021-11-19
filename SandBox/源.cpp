#include<YiaEngine.h>


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



UINT TextureWidth = 200;
UINT TextureHeight = 200;
UINT TexturePixelSize = 4;
Graphic::ImageData GenerateTextureData()
{
	Graphic::ImageData retImage;
	retImage.Width = 200;
	retImage.Height = 200;
	retImage.PerPixelByte = 4;
	retImage.Pitch = 200 * 4;
	retImage.Size = retImage.Pitch * retImage.Height;
	retImage.pData = std::shared_ptr<uint8_t>(new uint8_t[retImage.Size]);

	const UINT rowPitch = TextureWidth * TexturePixelSize;
	const UINT cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
	const UINT cellHeight = TextureWidth >> 3;    // The height of a cell in the checkerboard texture.
	const UINT textureSize = rowPitch * TextureHeight;
	
	std::vector<UINT8> data(textureSize);
	UINT8* pData = retImage.pData.get();

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
	
	return retImage;
}

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




		Graphic::ShaderLibrary::LoadShader("DefaultShader.hlsl",defaultShader);
	//	Graphic::ShaderLibrary::LoadShader("BlitShader.hlsl", blitShader);
		Graphic::ShaderLibrary::LoadShader("PbrShader.hlsl", pbrShader);

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

		Math::Vec3f BoxVertex[] = {
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
			//后
			{ -1.0f, 1.f ,	1.0f },
			{ 1.0f, 1.f ,	1.0f },
			{ 1.0f, -1.f ,	1.0f },
			{ -1.0f, -1.f , 1.0f },
		};
		Math::Vec3f Boxnormal[] =
		{
			//上
			{ 0.0f, 1.f , 0.0f },
			{ 0.0f, 1.f , 0.0f },
			{ 0.0f, 1.f , 0.0f },
			{ 0.0f, 1.f , 0.0f },
			//下
			{ 0.0f, -1.f , 0.0f },
			{ 0.0f, -1.f , 0.0f },
			{ 0.0f, -1.f , 0.0f },
			{ 0.0f, -1.f , 0.0f },
			//左
			{ -1.0f, 0.f , 0.0f },
			{ -1.0f, 0.f , 0.0f },
			{ -1.0f, 0.f , 0.0f },
			{ -1.0f, 0.f , 0.0f },
			//右
			{ 1.0f, 0.f , 0.0f },
			{ 1.0f, 0.f , 0.0f },
			{ 1.0f, 0.f , 0.0f },
			{ 1.0f, 0.f , 0.0f },
			//前
			{ 0.0f, 0.f , -1.0f },
			{ 0.0f, 0.f , -1.0f },
			{ 0.0f, 0.f , -1.0f },
			{ 0.0f, 0.f , -1.0f },
			//后
			{ 0.0f, 0.f , 1.0f },
			{ 0.0f, 0.f , 1.0f },
			{ 0.0f, 0.f , 1.0f },
			{ 0.0f, 0.f , 1.0f },
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
			15,13,12,15,14,13,
			16,17,19,17,18,19,
			23,21,20,23,22,21 };

		

	
		


		fullScreenRect.SetName("FullScreenRect");
		fullScreenRect.AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kPosition, DataFormate::kFloat_3, 6, pos));
		fullScreenRect.AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kTexcoord, DataFormate::kFloat_2, 6, uv));
		fullScreenRect.AddIndices(6, index);
		fullScreenRect.CreateMeshGpuBuffer();

		Box.SetName("Box");
		Box.AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kPosition, DataFormate::kFloat_3, 24, BoxVertex));
		Box.AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kTexcoord, DataFormate::kFloat_2, 24, Boxuv));
		Box.AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kNormal, DataFormate::kFloat_3, 24, Boxnormal));
		Box.AddIndices(sizeof(boxIndex) / sizeof(int), boxIndex);
		Box.CreateMeshGpuBuffer();
		BoxPos = Math::Vec3f(5, 0, 0);


		Window& curWindow = Window::CurrentWindow();

		
		
		camera.SetViewport(1691, 969);
		camera.Position(Math::Vec3f(1, 0, -5));
		camera.SetFarclip(10);
		camera.SetNearclip(0.01);
		camera.Front(Math::Vec3f(0, 0, 1));
		Math::Mat4x4f viewMat = camera.ViewMatrix();
		Math::Mat4x4f projMat = camera.ProjectionMatrix();
		Math::Mat4x4f worldMat = Math::Mat4x4f::Identity();


		defaultMaterial.InitMaterial("DefaultMaterial",defaultShader);
	//	TextureMaterial.InitMaterial("TextureMaterial", blitShader);
		pbrMaterial.InitMaterial("PBRMaterial", pbrShader);
		
		Graphic::ImageData image = GenerateTextureData();

		testTexture.InitializeByImage(image, DXGI_FORMAT_R8G8B8A8_UNORM);
		LightData light;
		light.Dirction =Math::Vec3f (1, 0, 1).normalized();
		light.Color = Color(1,1,1,1);
		light.Intensity = 1;
		mainLight = Light(LightType::Light_Direction, light);


		surfaceData.Albedo = Color(1, 0, 1, 1);

		Transform trans;
		trans.Position = { 1.f,2.f,3.f };
		trans.Translation(1, 2, 3);

		obj.meshRenderer.pMesh = std::shared_ptr<Mesh>(&Box);
		obj.meshRenderer.pMaterial = std::shared_ptr<Graphic::Material>(&pbrMaterial);
		obj.transform.Position = Math::Vec3f(1, 0, 0);
		obj.transform.Rotation(0,Math::Angle2Rad(45),0);
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
		
		//YIA_INFO("Delta time {0}", timestep.GetSeconds());
		camera.Update(timestep);

		imGuiLayer.Begin();
		imGuiLayer.OnUpdate(timestep);
		
		
		imGuiLayer.Render();

		RenderScene(timestep);
		/*
		*	Renderer->bindTexture(TextureBuffer);
			Renderer->DrawMesh(Mesh,shader);
		*/
		imGuiLayer.End();
	}
	
	bool isDestroy = false;
	SampleLayer sampleLayer; 
	ImguiLayer imGuiLayer;


	void RenderObject(Math::Vec3f pos)
	{
		Graphic::RenderBuffer& sceneColorBuffer = imGuiLayer.SceneColorBuffer();
		Graphic::DepthBuffer& sceneDepthBuffer = imGuiLayer.SceneDepthBuffer();
		
		DefaultRenderer.Begin();
		DefaultRenderer.SetRenderTarget(&sceneColorBuffer, &sceneDepthBuffer);
		DefaultRenderer.SetCamera(camera);
		DefaultRenderer.ClearRenderTarget();
		DefaultRenderer.ClearDepthStencil();

		defaultMaterial.SetMatrix("ObjectMat", Math::Mat4x4f::Identity());
		defaultMaterial.SetMatrix("WorldMat", Math::Translate({1,0,1}));
		defaultMaterial.SetMatrix("ViewMat", camera.ViewMatrix());
		defaultMaterial.SetMatrix("ProjMat", camera.ProjectionMatrix());

		defaultMaterial.SetTexture("MainTexture", testTexture);

		DefaultRenderer.DrawMesh(Box, defaultMaterial);
		defaultMaterial.SetMatrix("WorldMat", Math::Translate({ 5,0,1 }));
		DefaultRenderer.DrawMesh(Box, defaultMaterial);

		DefaultRenderer.End();
	}
	void RenderImage()
	{
		Graphic::RenderBuffer& sceneColorBuffer = imGuiLayer.SceneColorBuffer();
		Graphic::DepthBuffer& sceneDepthBuffer = imGuiLayer.SceneDepthBuffer();

		TextureMaterial.SetTexture("MainTexture", testTexture);

		DefaultRenderer.Begin();
		DefaultRenderer.SetRenderTarget(&sceneColorBuffer, &sceneDepthBuffer);
		DefaultRenderer.ClearRenderTarget();
		DefaultRenderer.ClearDepthStencil();
		DefaultRenderer.DrawMesh(fullScreenRect,TextureMaterial);
		DefaultRenderer.End();
	}
	void RenderPBR(Timestep  timestep)
	{
		Graphic::RenderBuffer& sceneColorBuffer = imGuiLayer.SceneColorBuffer();
		Graphic::DepthBuffer& sceneDepthBuffer = imGuiLayer.SceneDepthBuffer();

		DefaultRenderer.Begin();
		DefaultRenderer.SetRenderTarget(&sceneColorBuffer, &sceneDepthBuffer);
		DefaultRenderer.SetCamera(camera);
		DefaultRenderer.ClearRenderTarget();
		DefaultRenderer.ClearDepthStencil();

		obj.transform.EularRotation += Math::Vec3f(0, Math::Angle2Rad(timestep.GetSeconds() * 30), 0);
		Math::Mat4x4f world = obj.transform.FinalMatrix();
		
		pbrMaterial.SetMatrix("ObjectMat", Math::Mat4x4f::Identity());
		pbrMaterial.SetMatrix("WorldMat", world);
		pbrMaterial.SetMatrix("ViewMat", camera.ViewMatrix());
		pbrMaterial.SetMatrix("ProjMat", camera.ProjectionMatrix());
		pbrMaterial.SetMatrix("WorldToObjMat", world.inverse());

		pbrMaterial.SetTexture("MainTexture", testTexture);
		pbrMaterial.SetMemoryValue("surface",&surfaceData);
		pbrMaterial.SetMemoryValue("Lights", &mainLight.data_);


	//	DefaultRenderer.DrawMesh(Box, pbrMaterial);
		DefaultRenderer.DrawMesh(obj.meshRenderer);
		/*defaultMaterial.SetMatrix("WorldMat", Math::Translate({ 5,0,1 }));
		DefaultRenderer.DrawMesh(Box, defaultMaterial);*/

		DefaultRenderer.End();
	}
	void RenderScene(Timestep  timestep)
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
		//RenderObject();
		//RenderObject(Math::Vec3f(3, 0, 0));
		RenderPBR(timestep);
		//RenderImage();
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
	Graphic::RootSignature BlitSignature;
	Mesh fullScreenRect;
	Mesh Box;
	std::shared_ptr<Graphic::Shader> sampleShader;
	std::shared_ptr<Graphic::Shader> defaultShader;
	std::shared_ptr<Graphic::Shader> blitShader;
	std::shared_ptr<Graphic::Shader> pbrShader;

	Graphic::Renderer DefaultRenderer;
	
	ComPtr<ID3D12Resource>g_cbv;
	Camera camera;

	Graphic::Material defaultMaterial;
	Graphic::Material pbrMaterial;
	Graphic::Material TextureMaterial;

	Graphic::Texture testTexture;
	
	Math::Vec3f BoxPos;
	Light mainLight;
	SurfaceData surfaceData;
	GameObject obj;
	
};


Application* CreateApplication()
{
	return new SampleApp();
}