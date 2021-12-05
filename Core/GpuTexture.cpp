#include "pch.h"
#include "GpuBuffer.h"
#include "GpuTexture.h"
#include "ResourceAllocator.h"
namespace YiaEngine
{
    namespace Graphic
    {
        TextureDescribe GpuTexture::DescribeTex2D(UINT width, UINT Height,  DXGI_FORMAT format, UINT depthOrArraySize, UINT mips, UINT flag)
        {
        
            describe_.DepthOrArraySize = (UINT16)depthOrArraySize;
            describe_.Dimension =ResourceDimension::TEXTURE2D;
            describe_.Flags = (TextureFlag)flag;
            describe_.Format = format;
            describe_.Height = (UINT)Height;
            describe_.Layout = TextureLayout::UNKNOWN; 
            describe_.MipLevels = (UINT16)mips;
            describe_.MsaaCount = 1;
            describe_.MsaaQuality = 0;
            describe_.Width = (UINT64)width;
            
            return  describe_;
        }
        void GpuTexture::CreateTextureResource(const wchar_t* Name,const ClearState clearValue)
        {
            
#ifdef DIRECT12
			D3D12_RESOURCE_DESC Desc = {};
			Desc.Alignment = describe_.Alignment;
			Desc.DepthOrArraySize = (UINT16)describe_.DepthOrArraySize;
			Desc.Dimension =(D3D12_RESOURCE_DIMENSION) describe_.Dimension;
			Desc.Flags = (D3D12_RESOURCE_FLAGS)describe_.Flags;
			Desc.Format = describe_.Format;
			Desc.Height = (UINT)describe_.Height;
            Desc.Layout = (D3D12_TEXTURE_LAYOUT)describe_.Layout;
			Desc.MipLevels = (UINT16)describe_.MipLevels;
			Desc.SampleDesc.Count = describe_.MsaaCount;
			Desc.SampleDesc.Quality = describe_.MsaaQuality;
			Desc.Width = describe_.Width;


            D3D12_CLEAR_VALUE* pClearvalue = nullptr;
           if (describe_.Flags == ALLOW_RENDER_TARGET || describe_.Flags == ALLOW_DEPTH_STENCIL ||describe_.Dimension == ResourceDimension::BUFFER)
           {
			   D3D12_CLEAR_VALUE clearvalue;
			  
			   clearvalue.Format = Desc.Format;
			   clearvalue.Color[0] = clearValue.ClearColor.R;
			   clearvalue.Color[1] = clearValue.ClearColor.G;
			   clearvalue.Color[2] = clearValue.ClearColor.B;
			   clearvalue.Color[3] = clearValue.ClearColor.A;
			   clearvalue.DepthStencil.Depth = clearValue.DepthStencilClearValue.Depth;
			   clearvalue.DepthStencil.Stencil = clearValue.DepthStencilClearValue.Stencil;
               pClearvalue = &clearvalue;
           }
         
            auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        
            D3D12MA::ALLOCATION_DESC alloc_desc = {};
			alloc_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
            
            g_GpuResourceAllocator.CreateResource(
                &alloc_desc,
                &Desc,
                D3D12_RESOURCE_STATE_COMMON,
                pClearvalue,
                &allocation_,
               &resource_
            );
            usage_ = D3D12_RESOURCE_STATE_COMMON;
#endif // DIRECT12
        }
        DXGI_FORMAT GpuTexture::GetStencilFormat(DXGI_FORMAT format)
        {
            switch (format)
            {
                // 32-bit Z w/ Stencil
            case DXGI_FORMAT_R32G8X24_TYPELESS:
            case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
            case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
            case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
                return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;

                // 24-bit Z
            case DXGI_FORMAT_R24G8_TYPELESS:
            case DXGI_FORMAT_D24_UNORM_S8_UINT:
            case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
            case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
                return DXGI_FORMAT_X24_TYPELESS_G8_UINT;

            default:
                return DXGI_FORMAT_UNKNOWN;
            }
        }

        
        /* const DescriptorHandle& YiaEngine::Graphic::GpuTexture::SrvHandle()const
        {
            return srvHandle_;
        }*/
    }
}
