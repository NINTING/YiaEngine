#include "pch.h"
//#include "GeometryPass.h"
//
//namespace YiaEngine
//{
//	namespace YiaFrameGraph
//	{
//		void GeometryPass::Setup(FrameGraph& fg)
//		{
//
//
//			VirtualResourceHandle posGbufferId = fg.CreateTemplateRenderBuffer("PositionGBuffer",
//				Graphic::TextureDescribe::RenderTarget2Describe(1920, 1080, DXGI_FORMAT_R32G32B32A32_FLOAT));
//			VirtualResourceHandle normalGbufferId = fg.CreateTemplateRenderBuffer("NormalGBuffer",
//				Graphic::TextureDescribe::RenderTarget2Describe(1920, 1080, DXGI_FORMAT_R32G32B32A32_FLOAT));
//			VirtualResourceHandle albedoGbufferId = fg.CreateTemplateRenderBuffer("AlbedoGBuffer",
//				Graphic::TextureDescribe::RenderTarget2Describe(1920, 1080, DXGI_FORMAT_R8G8B8A8_UNORM));
//		
//			VirtualResourceHandle albedoTexture = fg.GetResourceHandle("AlbedoTexture");
//			AddInputResource(albedoTexture);
//			
//			AddOutput(posGbufferId);
//			AddOutput(normalGbufferId);
//			AddOutput(albedoGbufferId);
//
//		
//		}
//
//		void GeometryPass::Execute()
//		{
//			throw std::logic_error("The method or operation is not implemented.");
//		}
//	}
//}
//
//
