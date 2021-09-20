#pragma once

#ifdef YIA_WINDOW
	#ifdef YIA_BUILD_DLL
		#define YIA_API __declspec(dllexport)
	#else
		#define YIA_API __declspec(dllimport)
	#endif 

#endif