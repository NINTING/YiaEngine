#include "pch.h"
#include "ThreadSystem.h"



namespace YiaEngine
{

	namespace ThreadSystem
	{
		static DWORD WINAPI ThreadFunctionStatic(void* data)
		{
			ThreadDesc* pDesc = (ThreadDesc*)data;
			pDesc->pFunc(pDesc->pData);
			return 0;
		}
	
		ThreadHandle CreateThread(ThreadDesc* pDesc)
		{
			ThreadHandle handle = ::CreateThread(0, 0, ThreadFunctionStatic, pDesc, 0, 0);
			return handle;
		}
	}
}