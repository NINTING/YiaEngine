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
		/// ����DescriptorTable�İ������������Լ�Gpu Handle ��ʼλ��
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