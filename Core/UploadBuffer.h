#pragma once

#include"Graphic.h"
#include"GpuResource.h"
namespace YiaEngine
{
	namespace Graphic
	{
		class UploadBuffer:public GpuBuffer
		{
		public:
			UploadBuffer(const GpuResource& resource, size_t dataSize) :GpuResource(resource), bufferSize_(dataSize) {}
			UploadBuffer() = default;
			void Create(const wchar_t* Name, size_t dataSize);
			void* Map();
			void UnMap(size_t begin = 0, size_t end = -1);
			size_t BufferSize()const { return bufferSize_; }
		protected:

			size_t bufferSize_;
		};
	}
}