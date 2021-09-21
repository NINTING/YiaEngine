#pragma once
#include"pch.h"
#include"Graphic.h"
#include"GpuResource.h"
#include"UploadBuffer.h"
namespace YiaEngine
{
	namespace Graphic
	{
		void UploadBuffer::Create(const GpuResource& gpuResource, size_t dataSize)
		{
			/*resource_.Attach(gpuResource.resource_);
			usage_ = gpuResource.usage_;
			gpuVirtualAddress_ = gpuResource.gpuVirtualAddress_;
			bufferSize_ = dataSize;*/
		}
	}
}