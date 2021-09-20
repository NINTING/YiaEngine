#include "SampleDll.h"

#include"Common/YiaMath.h"
#include<stdio.h>

void YiaEngine::Print()
{
	YiaEngine::Math::Vec3f s(2.f,3.f,1.f);
	
	printf("Hello YiaEngine\n%f",s[1]);
}
