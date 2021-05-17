#pragma once
#include "MemoryManager.h"
#include <malloc.h>
#include<string.h>
using namespace YiaEngine;

namespace YiaEngine
{


 //   YiaEngine::MemoryManager* g_pMemoryManager;

    static const unsigned kBlockSizes[] = {
        // 4-increments
        4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
        52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96,

        // 32-increments
        128, 160, 192, 224, 256, 288, 320, 352, 384,
        416, 448, 480, 512, 544, 576, 608, 640,

        // 64-increments
        704, 768, 832, 896, 960, 1024
    };
    static const unsigned kPageSize = 8192;
    static const unsigned kAlignment = 4;

    // number of elements in the block size array
    static const unsigned kNumBlockSizes =
        sizeof(kBlockSizes) / sizeof(kBlockSizes[0]);

    // largest valid block size
    static const unsigned kMaxBlockSize =
        kBlockSizes[kNumBlockSizes - 1];

    Allocator* MemoryManager::m_pAllocators;
    size_t* MemoryManager::m_pBlockSizeLookup;
}


YiaEngine::MemoryManager::MemoryManager()
{
 //   Initialize();
}

void MemoryManager::Initialize()
{
    static bool s_bInitialized = false;
    if (!s_bInitialized)
    {
        m_pBlockSizeLookup = new size_t[kMaxBlockSize];

        int j = 0;
        for (int i = 0; i < kMaxBlockSize; i++)
        {
            if (i > kBlockSizes[j])
                j++;
            m_pBlockSizeLookup[i] = j;

        }

        m_pAllocators = new Allocator[kNumBlockSizes];
        for (size_t i = 0; i < kNumBlockSizes; i++)
        {
            m_pAllocators[i].Reset(kBlockSizes[i], kPageSize, kAlignment);
        }
        s_bInitialized = true;
    }

}

Allocator* YiaEngine::MemoryManager::LookupAllocate(size_t size)
{

    if (size <= kMaxBlockSize)
        return &m_pAllocators[m_pBlockSizeLookup[size]];
    else
        return nullptr;
}

void* YiaEngine::MemoryManager::Allocate(size_t size)
{
    Allocator* allocator = LookupAllocate(size);
    if (allocator)
      return  allocator->Allocate();
    else
    {
        
#if defined(_DEBUG)
        void* p = malloc(size);
        memset(
            reinterpret_cast<char*>(p) ,
            0,
            size
        );
        return p;
#else
        return malloc(size);
#endif
        
    }
}

void* YiaEngine::MemoryManager::Allocate(size_t size, size_t aligment)
{
    size += aligment;
    Allocator* allocator = LookupAllocate(size);
    if (allocator)
        return  allocator->Allocate();
    else
        return malloc(size);
}

void YiaEngine::MemoryManager::Free(void* p, size_t size)
{
    Allocator* allocator = LookupAllocate(size);
    if (allocator)
        allocator->Free(p);
    else
        free(p);

}
