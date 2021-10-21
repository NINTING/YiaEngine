#ifndef COMMON_DATAFORMAT_H
#define COMMON_DATAFORMAT_H
#include<stdint.h>
#include<assert.h>
namespace YiaEngine
{
	enum class DataFormat : int
	{
		kUnkown = 0,
		kUint16_1 = kUnkown   + 1,		//2
		kUint32_1 = kUint16_1 + 1,		//4
		kInt32_1  = kUint32_1 + 1,
		kFloat_1  = kInt32_1  + 1,		
		kUint16_2 = kFloat_1  + 1,		
		kUint16_3 = kUint16_2 + 1,		//6
		kUint16_4 = kUint16_3 + 1,		//8
		kDouble_1 = kUint16_4 + 1,
		kUint32_2 = kDouble_1 + 1,
		kInt32_2  = kUint32_2 + 1,
		kFloat_2  = kInt32_2  + 1,
		kUint32_3 = kFloat_2  + 1,		//12
		kInt32_3  = kUint32_3 + 1,
		kFloat_3  = kInt32_3  + 1,
		kDouble_2 = kFloat_3  + 1,		//16
		kUint32_4 = kDouble_2 + 1,
		kInt32_4  = kUint32_4 + 1,
		kFloat_4  = kInt32_4  + 1,
		kDouble_3 = kFloat_4  + 1,		//24 
		kDouble_4 =kDouble_3  + 1,		//32
	};



	size_t DataFormatStride(DataFormat data_type);

	
}
#endif //  COMMON_DATAFORMAT_H