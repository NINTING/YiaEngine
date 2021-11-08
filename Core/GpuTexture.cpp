#include "pch.h"
#include "GpuBuffer.h"
#include "GpuTexture.h"
namespace YiaEngine
{
    namespace Graphic
    {
        D3D12_RESOURCE_DESC GpuTexture::DescribeTex2D(UINT width, UINT Height, UINT depthOrArraySize, UINT mips, DXGI_FORMAT format, UINT flag)
        {
            width_ = width;
            height_ = Height;
            format_ = format;
            numMips_ = mips;
            arraySize_  = depthOrArraySize;

            D3D12_RESOURCE_DESC Desc = {};
            Desc.Alignment = 0;
            Desc.DepthOrArraySize = (UINT16)depthOrArraySize;
            Desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            Desc.Flags = (D3D12_RESOURCE_FLAGS)flag;
            Desc.Format = format;
            Desc.Height = (UINT)Height;
            Desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            Desc.MipLevels = (UINT16)mips;
            Desc.SampleDesc.Count = 1;
            Desc.SampleDesc.Quality = 0;
            Desc.Width = (UINT64)width;

            return Desc;
        }
        void GpuTexture::CreateTextureResource(const wchar_t* Name, const D3D12_RESOURCE_DESC resourceDesc,const D3D12_CLEAR_VALUE* value)
        {
            auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
            ASSERT_SUCCEEDED(Graphic::g_Device->CreateCommittedResource(
                &heap_properties,
                D3D12_HEAP_FLAG_NONE,
                &resourceDesc,
                D3D12_RESOURCE_STATE_COMMON,
                value,
                IID_PPV_ARGS(&resource_)));
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
