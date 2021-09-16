#pragma once

#include"Graphic.h"
#include"GpuResource.h"
namespace YiaEngine
{
	namespace Graphic
	{
		class UploadBuffer:GpuResource
		{
		public:
			UploadBuffer(const GpuResource& resource, size_t dataSize) :GpuResource(resource), bufferSize_(dataSize) {}
			void Create(void* data, size_t data_size);
			void Create(const GpuResource& resource, size_t dataSize);
		protected:

			size_t bufferSize_;
		};
	}
}