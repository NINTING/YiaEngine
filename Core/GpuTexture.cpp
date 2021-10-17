#include "pch.h"
#include "GpuBuffer.h"
#include "GpuTexture.h"
namespace YiaEngine
{
    namespace Graphic
    {
        const DescriptorHandle& YiaEngine::Graphic::GpuTexture::SrvHandle()const
        {
            return srvHandle_;
        }
    }
}
