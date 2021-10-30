#pragma once
#include "GpuResource.h"
#include"DescriptorHeap.h"
namespace YiaEngine
{
    namespace Graphic
    {
        class GpuTexture : public GpuResource
        {
        public:
         //   const DescriptorHandle& SrvHandle()const;
            ~GpuTexture() { Destroy(); }
            D3D12_RESOURCE_DESC DescribeTex2D(UINT widht, UINT height, UINT depthOrArraySize, UINT mips, DXGI_FORMAT format, UINT flag);
            void CreateTextureResource(const wchar_t* name,const D3D12_RESOURCE_DESC resourceDesc, const D3D12_CLEAR_VALUE* value);
        protected:
            DXGI_FORMAT GetStencilFormat(DXGI_FORMAT format);
        protected:
            UINT64 width_ = 0;
            UINT64 height_ = 0;
            DXGI_FORMAT format_;
            UINT numMips_;
            UINT arraySize_;
      //      DescriptorHandle srvHandle_;
        };

    }
}