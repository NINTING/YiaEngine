#pragma once

#ifdef YIA_WINDOW
	#ifdef YIA_BUILD_DLL
		#define YIA_API __declspec(dllexport)
	#else
		#define YIA_API
	#endif 
#endif

#define NOMINMAX

#ifdef YIA_WINDOW
#include <windows.h>
#include <windowsx.h>
#endif
#include<iostream>
#include<string>
#include<sstream>

#include<functional>

#include<memory>
#include<algorithm>

using std::max;
using std::min;