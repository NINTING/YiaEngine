#include"DataFormat.h"

namespace YiaEngine
{
	size_t DataFormatStride(DataFormat data_type)
	{

		int stride_in_bytes = 0;
		switch (data_type)
		{
		case DataFormat::kUint16_1:
			stride_in_bytes = sizeof(uint16_t);
			break;
		case DataFormat::kUint32_1:
		case DataFormat::kFloat_1:
		case DataFormat::kUint16_2:
			stride_in_bytes = 4;
			break;
		case DataFormat::kUint16_3:
			stride_in_bytes = 6;
			break;
		case DataFormat::kUint16_4:
		case DataFormat::kDouble_1:
		case DataFormat::kUint32_2:
		case DataFormat::kFloat_2:
			stride_in_bytes = 8;
			break;
		case DataFormat::kUint32_3:
		case DataFormat::kFloat_3:
			stride_in_bytes = 12;
			break;
		case DataFormat::kDouble_2:
		case DataFormat::kUint32_4:
		case DataFormat::kFloat_4:
			stride_in_bytes = 16;
			break;
		case DataFormat::kDouble_3:
			stride_in_bytes = 24;
			break;
		case DataFormat::kDouble_4:
			stride_in_bytes = 32;
			break;
		default:
			assert(false);
			break;
		}
		return stride_in_bytes;
	}
}