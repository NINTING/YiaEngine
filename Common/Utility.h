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


	// convert string to wstring
	inline std::wstring String2WString(const std::string& str)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}
	// convert wstring to string 
	inline std::string WString2String(const std::wstring& wstr)
	{
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
	inline size_t HashRange(uint32_t* begin,uint32_t* end,size_t hash)
	{
		for (const uint32_t* i = begin; i != end; i++)
		{
			hash = 16777619U * hash ^ *i;
		}
		return hash;
	}

	template<typename T>
	inline size_t HashState(const T* stateDesc, size_t count = 1, size_t hash = 2166136261U)
	{
		static_assert((sizeof(T) & 3) == 0 && alignof(T) >= 4, "Error:Not a word-align");
		return HashRange((uint32_t*)stateDesc, (uint32_t * )(stateDesc + count), hash);
	}
}