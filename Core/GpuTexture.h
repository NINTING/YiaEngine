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
            const DescriptorHandle& SrvHandle()const;
            ~GpuTexture() { Destroy(); }
        protected:
            DescriptorHandle srvHandle_;
        };

    }
}