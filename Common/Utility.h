#pragma once

#include"pch.h"

namespace YiaEngine
{

	inline void Char2Wchar(const char* source, wchar_t* dest)
	{
		UINT size = strlen(source) + 1;
		MultiByteToWideChar(CP_ACP, 0, source, -1, dest, size);
	}

	inline void Wchar2char(const wchar_t* source, char* dest)
	{
		UINT size = wcslen(source) + 1;
		WideCharToMultiByte(CP_ACP, 0, source, -1, dest, size, NULL, NULL);
	}
}