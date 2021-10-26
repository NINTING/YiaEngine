#include "pch.h"
#include "ResourceLoder.h"



namespace YiaEngine::Graphic
{
	const wchar_t* FillEntryPoint(ShaderStage stage)
	{

		switch (stage)
		{
		case YiaEngine::Graphic::Shader_Stage_Vertex:
			return L"VsMain";
			break;
		case YiaEngine::Graphic::Shader_Stage_Pixel:
			return L"PsMain";
			break;
		
		}
		return L"";
	}
	VertexAttributeEnum SemanticNameToVertexAttribute(const char*semanticName)
	{
		if (strcmp(semanticName, "POSITION")==0)
			return VertexAttributeEnum::kPosition;
		if (strcmp(semanticName, "TEXCOORD")==0)
			return VertexAttributeEnum::kTexcoord;
		
	}
	void LoadShader(const ShaderLoadDesc& desc, Shader& shader)
	{
		shader = {};
		shader.Stages = Shader_Stage_None;
		for (size_t i =0; i < Shader_Stage_Count; i++)
		{
			shader.Stages |= desc.StageLoadDesc[i].stage;

		}
		for (size_t i = 0; i < Shader_Stage_Count; i++)
		{
			ShaderStage p = Shader_Stage_Pixel;

			if ((shader.Stages & (ShaderStage)(1 << i)) == 0)continue;
			ComPtr< ID3D12ShaderReflection> pReflection;
			const wchar_t* entryPoint = FillEntryPoint(desc.StageLoadDesc[i].stage);
			shader.ShaderBlob[i] = CompilerShaderFromDXC(
				desc.StageLoadDesc[i].FileName,
				(const wchar_t *)entryPoint,
				desc.StageLoadDesc[i].target,
				&pReflection);
			
			/*--------------------------------------------------------------------------*/
			D3D12_SHADER_DESC shader_desc = {};
			pReflection->GetDesc(&shader_desc);
			ShaderReflect& reflect = shader.Reflect[i];

			if ((1<<i)== Shader_Stage_Vertex)
			{
				reflect.VertexInput.AttributesCount = shader_desc.InputParameters;

				reflect.VertexInput.Attrs = new VertextInputAttribute[shader_desc.InputParameters];
				for (size_t i = 0; i < shader_desc.InputParameters; i++)
				{
					D3D12_SIGNATURE_PARAMETER_DESC desc;
					pReflection->GetInputParameterDesc(i, &desc);
					shader.Reflect[i].PoolSize += strlen( desc.SemanticName)+1;

				}

				/*TODO:使用自定义内存池分配*/
				reflect.pNamePool = new char[shader.Reflect[i].PoolSize];

				char* currentName = reflect.pNamePool;
				YIA_INFO("InputParament {0}", shader_desc.InputParameters);
				for (int j = 0; j < shader_desc.InputParameters; j++)
				{
					DataFormate format;
					D3D12_SIGNATURE_PARAMETER_DESC desc;
					pReflection->GetInputParameterDesc(j, &desc);
					switch ((int)log2(desc.Mask) + 1)
					{
					case 1:
						switch (desc.ComponentType)
						{
						case D3D_REGISTER_COMPONENT_UNKNOWN:break;
						case D3D_REGISTER_COMPONENT_UINT32: format = DataFormate::kUint32_1; break;
						case D3D_REGISTER_COMPONENT_SINT32: format = DataFormate::kInt32_1;break;
						case D3D_REGISTER_COMPONENT_FLOAT32:format = DataFormate::kFloat_1;break;
							break;
						}
						break;
					case 2:
						switch (desc.ComponentType)
						{
						case D3D_REGISTER_COMPONENT_UNKNOWN:break;
						case D3D_REGISTER_COMPONENT_UINT32:format = DataFormate::kUint32_2;break;
						case D3D_REGISTER_COMPONENT_SINT32:format = DataFormate::kInt32_2;break;
						case D3D_REGISTER_COMPONENT_FLOAT32:format = DataFormate::kFloat_2;break;
						break;
						}
						break;
					case 3:
						switch (desc.ComponentType)
						{
						case D3D_REGISTER_COMPONENT_UNKNOWN:break;
						case D3D_REGISTER_COMPONENT_UINT32:format = DataFormate::kUint32_3;break;
						case D3D_REGISTER_COMPONENT_SINT32:format = DataFormate::kInt32_3;break;
						case D3D_REGISTER_COMPONENT_FLOAT32:format = DataFormate::kFloat_3;break;
							break;

						}
						break;
					case 4:
						switch (desc.ComponentType)
						{
						case D3D_REGISTER_COMPONENT_UNKNOWN:break;
						case D3D_REGISTER_COMPONENT_UINT32:format = DataFormate::kUint32_4;break;
						case D3D_REGISTER_COMPONENT_SINT32:format = DataFormate::kInt32_4;break;
						case D3D_REGISTER_COMPONENT_FLOAT32:format = DataFormate::kFloat_4;break;

							break;
						}
						break;
					default:
						break;
					}
	
					VertextInputAttribute& attr = shader.Reflect[i].VertexInput.Attrs[j];


					attr.format = format;
					
					attr.NameSize = strlen(desc.SemanticName) + 1;

					sprintf_s(currentName, attr.NameSize, "%s", desc.SemanticName);

					attr.SemanticName = currentName;
					attr.Attribute = SemanticNameToVertexAttribute(attr.SemanticName);
					attr.SemanticIndex = desc.SemanticIndex;

					YIA_INFO("  InputParament {0}", j);
					YIA_INFO("	name {0}", desc.SemanticName);
					YIA_INFO("	SemanticIndex {0}", desc.SemanticIndex);
					YIA_INFO("	register {0}", desc.Register);
					YIA_INFO("	SystemValueType {0}", desc.SystemValueType);
					YIA_INFO("	ComponentType {0}", desc.ComponentType);
					YIA_INFO("	Stream {0}", desc.Stream);
					YIA_INFO("	Mask {0}", desc.Mask);
					YIA_INFO("	ReadWriteMask {0}", desc.ReadWriteMask);
					currentName += attr.NameSize+1;
				}

			}

			YIA_INFO("ConstantBuffers {0}", shader_desc.ConstantBuffers);
			for (int i = 0; i < shader_desc.ConstantBuffers; i++)
			{
				ID3D12ShaderReflectionConstantBuffer*
					buffer = pReflection->GetConstantBufferByIndex(i);

				D3D12_SHADER_BUFFER_DESC desc;
				ASSERT_SUCCEEDED(buffer->GetDesc(&desc));
				YIA_INFO("	ConstBuffer {0}", i);
				YIA_INFO("		name {0}", desc.Name);
				YIA_INFO("		Size {0}", desc.Size);
				YIA_INFO("		Type {0}", desc.Type);
				YIA_INFO("		Variables {0}", desc.Variables);
			}
			YIA_INFO("BoundResources {0}", shader_desc.BoundResources);
			YIA_INFO("InstructionCount {0}", shader_desc.InstructionCount);


			for (int i = 0; i < shader_desc.BoundResources; i++)
			{
				D3D12_SHADER_INPUT_BIND_DESC  resource_desc;
				pReflection->GetResourceBindingDesc(i, &resource_desc);


				auto shaderVarName = resource_desc.Name;
				auto registerSpace = resource_desc.Space;
				auto resourceType = resource_desc.Type;
				auto bindPoint = resource_desc.BindPoint;

				std::cout << "var name is " << shaderVarName << std::endl;
				std::cout << "type name is " << resourceType << std::endl;
				std::cout << "bind point is " << bindPoint << std::endl;
				std::cout << "register space is " << registerSpace << std::endl;

			}
		}
	}



	ComPtr<IDxcBlob> CompilerShaderFromDXC(const wchar_t* path, const wchar_t* entry, const wchar_t* target, ID3D12ShaderReflection** ppOutReflect)
	{
		ComPtr<IDxcUtils> pUtils;
		ComPtr<IDxcCompiler3> pCompiler;
		DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));
		DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));

		ComPtr<IDxcIncludeHandler> pIncludeHandler;
		pUtils->CreateDefaultIncludeHandler(&pIncludeHandler);

		std::cout << path << std::endl;
		std::cout << entry << std::endl;
		std::cout << target << std::endl;

		LPCWSTR pszArgs[] =
		{
			path,            // Optional shader source file name for error reporting and for PIX shader source view.  
			L"-E", entry,              // Entry point.
			L"-T",target,            // Target.
			L"-Zi",                      // Enable debug information.
			L"-D", L"MYDEFINE=1",        // A single define.
			L"-Fo", L"myshader.bin",     // Optional. Stored in the pdb. 
			L"-Fd", L"myshader.pdb",     // The file name of the pdb. This must either be supplied or the autogenerated file name must be used.
			L"-Qstrip_reflect",          // Strip reflection into a separate blob. 
		};
		ComPtr<IDxcBlobEncoding>pSource = nullptr;
		pUtils->LoadFile(path, nullptr, &pSource);
		DxcBuffer Source;
		Source.Ptr = pSource->GetBufferPointer();
		Source.Size = pSource->GetBufferSize();
		Source.Encoding = DXC_CP_ACP;

		ComPtr<IDxcResult> pResults;
		pCompiler->Compile(
			&Source,                // Source buffer.
			pszArgs,                // Array of pointers to arguments.
			_countof(pszArgs),      // Number of arguments.
			pIncludeHandler.Get(),        // User-provided interface to handle #include directives (optional).
			IID_PPV_ARGS(&pResults) // Compiler output status, buffer, and errors.
		);

		ComPtr<IDxcBlobUtf8> pErrors = nullptr;
		pResults->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&pErrors), nullptr);
		// Note that d3dcompiler would return null if no errors or warnings are present.  
		// IDxcCompiler3::Compile will always return an error buffer, but its length will be zero if there are no warnings or errors.
		if (pErrors != nullptr && pErrors->GetStringLength() != 0)
			wprintf(L"Warnings and Errors:\n%S\n", pErrors->GetStringPointer());

		HRESULT hrStatus;
		pResults->GetStatus(&hrStatus);
		if (FAILED(hrStatus))
		{
			wprintf(L"Compilation Failed\n");
			return nullptr;
		}
		
		ComPtr<IDxcBlob> pShader = nullptr;
		ComPtr<IDxcBlobUtf16> pShaderName = nullptr;
		pResults->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&pShader), &pShaderName);

		ComPtr< ID3D12ShaderReflection > pReflection;

		ComPtr<IDxcBlob> pReflectionData;
		pResults->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&pReflectionData), nullptr);

		if (pReflectionData != nullptr)
		{
			DxcBuffer ReflectionData;
			ReflectionData.Encoding = DXC_CP_ACP;
			ReflectionData.Ptr = pReflectionData->GetBufferPointer();
			ReflectionData.Size = pReflectionData->GetBufferSize();

			pUtils->CreateReflection(&ReflectionData, IID_PPV_ARGS(ppOutReflect));
		}
		
		return pShader;
	}
	
}