#pragma once

#include<queue>

#include"Common/YiaMath.h"
#include"CommandContext.h"
namespace YiaEngine
{
	namespace Graphic
	{
		class RootSignature;
		class PipelineStateObject;
		class DescriptorHeap;
		class DescriptorHandle;
		/// <summary>
		/// 描述DescriptorTable的绑定描述符数量以及Gpu Handle 起始位置
		/// </summary>



		class CopyContext :public CommandContext
		{
		public:
			static CopyContext& Begin(const wchar_t* Name = L"");
			static void UpdateBufferData(GpuBuffer& dest, const void* initData);

		private:

		};
	}

}