#include"pch.h"
#include "SampleDll.h"

#include"Common/YiaMath.h"
#include<stdio.h>

#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>  // support for loading levels from the environment variable
#include <spdlog/fmt/ostr.h> // support for user defined types
void YiaEngine::Print()
{
	spdlog::warn("Easy padding in numbers like {:08d}", 12);
	YiaEngine::Math::Vec3f s(2.f,3.f,1.f);
	
	printf("Hello YiaEngine\n%f",s[1]);
}
