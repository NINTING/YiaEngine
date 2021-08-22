#pragma once

#ifndef FRAMEWORK_MATH_COMMON_H_
#define FRAMEWORK_MATH_COMMON_H_

namespace YiaEngine
{
	const float PI = 3.1415926;

	float Angle2Rad(float angle);
	float Red2Angle(float rad);
	
	template <typename T> __forceinline T AlignUpWithMask(T value, size_t mask)
	{
		return (T)(((size_t)value + mask) & ~mask);
	}
	template <typename T> __forceinline T AlignUp(T value, size_t alignment)
	{
		return AlignUpWithMask(value, alignment - 1);
	}
}
#endif	//FRAMEWORK_MATH_COMMON_H_