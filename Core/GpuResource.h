#pragma once

#include"Graphic.h"

namespace D3D12MA
{
	class Allocation;
};

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
			D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress() { 
				if (resource_)
				{
					return gpuVirtualAddress_ = resource_->GetGPUVirtualAddress();
				}
				return ADDRESS_UNKOWN;
			};
			D3D12_RESOURCE_STATES State() { return state_; };
			virtual ~GpuResource() { Destroy(); }
			void State(D3D12_RESOURCE_STATES state) { state_ = state; }
			virtual void Destroy();

			void SetResourceName(const wchar_t* Name)
			{
				resource_->SetName(Name);
			}
		private:
			GpuResource(const GpuResource&) = delete;
			GpuResource operator = (const GpuResource&) = delete;

		protected:

			ComPtr<ID3D12Resource>resource_;
			D3D12_RESOURCE_STATES state_;
		
			DescriptorTypeFlags descriptorType_;

			ResourceUsageEnum resourceUsage_;
		
			uint32_t  versionID_ = 0;

			D3D12MA::Allocation* allocation_;

			D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress_ = ADDRESS_UNKOWN;


		};
	}
		
}