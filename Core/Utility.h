#pragma once

#include <Windows.h>
#include <stdarg.h>
#include<stdio.h>

#include"Common/Logger.h"
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
		inline void PrintSubMessage() {}
#endif

	}
}
