#pragma once
#include "pch.h"
#include"Graphic.h"
#include"GpuResource.h"
#include"ResourceAllocator.h"

namespace YiaEngine
{
	namespace Graphic
	{
	
		void GpuResource::Destroy()
		{
			if (allocation_)
			{
				allocation_->Release();
			}
			else
			{
				resource_.Detach();
			}
			gpuVirtualAddress_ = ADDRESS_UNKOWN;
			++versionID_;
		}
	}
		
}