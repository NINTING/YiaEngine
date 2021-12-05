#pragma once

#ifdef YIA_WINDOW
	#ifdef YIA_BUILD_DLL
		#define YIA_API __declspec(dllexport)
	#else
		#define YIA_API
	#endif 
#endif
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#ifdef YIA_WINDOW
#include <windows.h>
#include <windowsx.h>
#include<WinUser.h>
#include<time.h>
#endif
#include<iostream>
#include<string>
#include<sstream>

#include<functional>
#include<vector>
#include<stack>
#include<queue>
#include<memory>
#include<algorithm>
#include<map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <locale>
#include <codecvt>
using std::max;
using std::min;

