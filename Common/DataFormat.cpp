#include"pch.h"
#include"DataFormat.h"

namespace YiaEngine
{
	size_t DataFormatStride(DataFormate data_type)
	{

		int stride_in_bytes = 0;
		switch (data_type)
		{
		case DataFormate::kUint16_1:
			stride_in_bytes = sizeof(uint16_t);
			break;
		case DataFormate::kUint32_1:
		case DataFormate::kFloat_1:
		case DataFormate::kInt32_1:
		case DataFormate::kUint16_2:
			stride_in_bytes = 4;
			break;
		case DataFormate::kUint16_3:
			stride_in_bytes = 6;
			break;
		case DataFormate::kUint16_4:
		case DataFormate::kDouble_1:
		case DataFormate::kUint32_2:
		case DataFormate::kInt32_2:
		case DataFormate::kFloat_2:
			stride_in_bytes = 8;
			break;
		case DataFormate::kUint32_3:
		case DataFormate::kFloat_3:
		case DataFormate::kInt32_3:
			stride_in_bytes = 12;
			break;
		case DataFormate::kDouble_2:
		case DataFormate::kUint32_4:
		case DataFormate::kInt32_4:
		case DataFormate::kFloat_4:
			stride_in_bytes = 16;
			break;
		case DataFormate::kDouble_3:
			stride_in_bytes = 24;
			break;
		case DataFormate::kDouble_4:
			stride_in_bytes = 32;
			break;
		default:
			assert(false);
			break;
		}
		return stride_in_bytes;
	}
	DXGI_FORMAT DataFormatToDX12(DataFormate data_type)
	{
		switch (data_type)
		{
		case YiaEngine::DataFormate::kUnkown:
			return DXGI_FORMAT_UNKNOWN;
		case YiaEngine::DataFormate::kUint16_1:
			return DXGI_FORMAT_R16_UINT;
		case YiaEngine::DataFormate::kUint32_1:
			return DXGI_FORMAT_R32_UINT;
		case YiaEngine::DataFormate::kInt32_1:
			return DXGI_FORMAT_R32_SINT;
		case YiaEngine::DataFormate::kFloat_1:
			return DXGI_FORMAT_R32_FLOAT;
		case YiaEngine::DataFormate::kUint16_2:
			return DXGI_FORMAT_R16G16_UINT;
		case YiaEngine::DataFormate::kUint16_4:
			return DXGI_FORMAT_R16G16B16A16_UINT;
		case YiaEngine::DataFormate::kUint32_2:
			return DXGI_FORMAT_R32G32_UINT;
		case YiaEngine::DataFormate::kInt32_2:
			return DXGI_FORMAT_R32G32_SINT;
		case YiaEngine::DataFormate::kFloat_2:
			return DXGI_FORMAT_R32G32_FLOAT;
		case YiaEngine::DataFormate::kUint32_3:
			return DXGI_FORMAT_R32G32B32_UINT;
		case YiaEngine::DataFormate::kInt32_3:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case YiaEngine::DataFormate::kFloat_3:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case YiaEngine::DataFormate::kUint32_4:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		case YiaEngine::DataFormate::kInt32_4:
			return DXGI_FORMAT_R32G32B32A32_SINT;
		case YiaEngine::DataFormate::kFloat_4:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}
}