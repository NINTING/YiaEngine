#pragma once
#include "Allocator.h"
#include <new>


namespace YiaEngine
{
	class MemoryManager 
	{
	public:
		MemoryManager();


		template<typename T,typename... Arguments>
		T* New(Arguments... parameters)
		{
			return new (Allocate(sizeof(T)))T(parameters...);
		}
		template<typename T>
		void Delete(T* p)
		{
			reinterpret_cast<T*>(p)->~T();
			Free(p, sizeof(T));
		}
		static MemoryManager* Instance()
		{
			static MemoryManager* instance = nullptr;
			if (instance == nullptr)
			{
				instance = new MemoryManager();
				instance->Initialize();
			}
			return instance;
		}
	public:

		void Initialize();
		Allocator* LookupAllocate(size_t size);
		void* Allocate(size_t size);
		void* Allocate(size_t size, size_t aligment);
		void  Free(void* p, size_t size);
		
	private:
		static Allocator*	m_pAllocators;
		static size_t*		m_pBlockSizeLookup;
	};
}
