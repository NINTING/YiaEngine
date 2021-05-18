#pragma once

#include "MemoryManager.h"
#include<stdint.h>
#include<stdio.h>
#include<cstring>
namespace YiaEngine
{
	//extern MemoryManager* g_pMemoryManager;
	struct Buffer {
	public:
		Buffer(size_t size, size_t aligment = 4)
		{
			m_pdata =(uint8_t*) MemoryManager::Instance()->Allocate(size,aligment);
			m_size = size;
			m_aligment = aligment;
		}
		Buffer()
		{
			m_pdata = nullptr;
			m_size = 0;
			m_aligment =0;
		}
		Buffer(Buffer&& rhs)
		{
			printf("move\n");
			m_pdata = rhs.m_pdata;
			m_size = rhs.m_size;
			m_aligment = rhs.m_aligment;
			rhs.m_aligment = 0;
			rhs.m_size = 0;
			rhs.m_pdata = nullptr;
		}
		Buffer(const Buffer& rhs)
		{
			m_pdata = reinterpret_cast<uint8_t*>(MemoryManager::Instance()->Allocate(rhs.m_size));
			memcpy(m_pdata, rhs.m_pdata, rhs.m_size);
			m_size = rhs.m_size;
			m_aligment = rhs.m_aligment;
		}


		size_t m_size;
		size_t m_aligment;
		uint8_t * m_pdata;
	};
}
