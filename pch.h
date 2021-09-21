#pragma once

#ifdef YIA_WINDOW
	#ifdef YIA_BUILD_DLL
		#define YIA_API __declspec(dllexport)
	#else
		#define YIA_API
	#endif 
#endif

#include"Common/Logger.h"

#include<iostream>
#include<string>
#include<sstream>



