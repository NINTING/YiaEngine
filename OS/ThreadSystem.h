#pragma once



#include <windows.h>


namespace YiaEngine
{

	namespace ThreadSystem
	{

		typedef void(*ThreadFunction)(void*);
		struct ThreadHandle
		{
			void* Handle;
			ThreadHandle() :Handle(nullptr) {}
			ThreadHandle(void* handle) :Handle(handle) {}
			void* operator()() { return Handle; }
		};
		struct ThreadDesc
		{
			ThreadFunction pFunc;
			void* pData;
		};
		
		ThreadHandle CreateThread(ThreadDesc* pDesc);
	}
}