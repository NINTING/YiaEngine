#pragma once

#include <Windows.h>
#include <stdarg.h>
#include<stdio.h>
namespace YiaEngine
{
	namespace Utility
	{
#ifdef _CONSOLE
		inline void Print(const char* msg) { printf("%s", msg); }
		inline void Print(const wchar_t* msg) { wprintf(L"%ws", msg); }
#else
		inline void Print(const char* msg) { OutputDebugStringA(msg); }
		inline void Print(const wchar_t* msg) { OutputDebugString(msg); }
#endif

#ifndef RELEASE
		inline void PrintSubMessage(const char* format, ...)
		{
			Print("--> ");
			char buffer[256];
			va_list ap;
			va_start(ap, format);
			vsprintf_s(buffer, 256, format, ap);
			va_end(ap);
			Print(buffer);
			Print("\n");
		}
#endif

	}
}
#ifdef RELEASE

#else

#define STRINGIFY(x) #x
#define STRINGIFY_BUILTIN(x) STRINGIFY(x)

#endif // DEBUG



#define ASSEERT_SUCCEEDE(hr,...)\
		if(FAILED(hr))					\
		{\
			YiaEngine::Utility::Print("\nHRESULT failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
			YiaEngine::Utility::PrintSubMessage("hr = 0x%08X", hr); \
            YiaEngine::Utility::PrintSubMessage(__VA_ARGS__); \
            YiaEngine::Utility::Print("\n"); \
            __debugbreak(); \
		}