#ifndef COMMON_DATAFORMAT_H
#define COMMON_DATAFORMAT_H
#include<stdint.h>
#include<assert.h>
namespace YiaEngine
{
	enum class DataFormat : int
	{
		kUnkown = 0,
		kUint16_1 = 1,		//2
		kUint32_1 = 2,		//4
		kFloat_1 = 3,		//4
		kUint16_2 = 4,		//4
		kUint16_3 = 5,		//6
		kUint16_4 = 6,		//8
		kDouble_1 = 7,		//8
		kUint32_2 = 8,		//8
		kFloat_2 = 9,		//8
		kUint32_3 = 10,		//12
		kFloat_3 = 11,		//12
		kDouble_2 = 12,		//16
		kUint32_4 = 13,		//16
		kFloat_4 = 14,		//16
		kDouble_3 = 15,		//24 
		kDouble_4 = 16,		//32
	};



	size_t DataFormatStride(DataFormat data_type);

	
}
#endif //  COMMON_DATAFORMAT_H