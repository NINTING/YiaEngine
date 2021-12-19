#include "pch.h"
#include "CopyContext.h"
namespace YiaEngine
{
	namespace Graphic
	{
		CopyContext& CopyContext::Begin(const wchar_t* Name /*= L""*/)
		{
			CommandContext* commandContext = CommandContext::Begin(Name, D3D12_COMMAND_LIST_TYPE_COPY);

			return reinterpret_cast<CopyContext&>(*commandContext);
		}

		void CopyContext::UpdateBufferData(GpuBuffer& dest, const void* initData)
		{
			UINT bufferSize = dest.BufferSize();
			CopyContext& initContext = CopyContext::Begin();
			GpuBuffer& upload_buffer = initContext.AllocateTemplateUploadBuffer(bufferSize);

			D3D12_SUBRESOURCE_DATA data;
 			data.pData = initData;
			data.RowPitch = bufferSize;                              


			initContext.TransitionBarrier(dest, D3D12_RESOURCE_STATE_COPY_DEST);
			ASSERT_SUCCEEDED(UpdateSubresources<1>(initContext.NativeCommandList(), dest.NativeResource(), upload_buffer.NativeResource(), 0, 0, 1, &data));
			initContext.TransitionBarrier(dest, D3D12_RESOURCE_STATE_COMMON);
			
			
			//initContext.commandList_->CopyBufferRegion(
			//	pBuffer->pDxResource, dstOffset, pSrcBuffer->pDxResource, srcOffset, size);

			initContext.End();



		}

	}
}