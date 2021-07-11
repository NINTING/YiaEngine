


#include"CommonMath.h"

namespace YiaEngine
{
	float Angle2Rad(float angle)
	{
		return (angle / 180.f) * PI;
	}
	float Red2Angle(float rad)
	{
		return rad / PI * 180.f;
	}
}