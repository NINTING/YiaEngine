#pragma once

#include"Graphic.h"

namespace YiaEngine
{
	namespace Graphic
	{
		class GpuResource
		{
		public:
			GpuResource() = default;
			GpuResource(ID3D12Resource* NativeResource) :resource_(NativeResource) {}
			const ID3D12Resource* NativeResource() const{
				return resource_.Get();
			}
			ID3D12Resource* NativeResource() {
				return resource_.Get();
			}
			D3D12_RESOURCE_STATES Usage() { return usage_; };
			~GpuResource() { Destroy(); }
			void Usage(D3D12_RESOURCE_STATES state) { usage_ = state; }
			virtual void Destroy()
			{
				resource_.Detach();
				gpuVirtualAddress_ = ADDRESS_UNKOWN;
				++versionID_;
			}
			void SetResourceName(const wchar_t* Name)
			{
				resource_->SetName(Name);
			}
		protected:
			ComPtr<ID3D12Resource>resource_;
			D3D12_RESOURCE_STATES usage_;
			D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress_ = ADDRESS_UNKOWN;
			uint32_t  versionID_ = 0;
		};
	}
		
}