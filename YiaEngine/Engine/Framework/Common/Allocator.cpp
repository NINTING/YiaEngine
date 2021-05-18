

#include"Allocator.h"
#include<algorithm>
using namespace YiaEngine;

Allocator::Allocator()
{
	m_ppageList = nullptr;
	m_pfreeList = nullptr;
	FreeAll();
}

Allocator::Allocator(unsigned dataSize, unsigned pageSize, unsigned alignment)
:m_ppageList(nullptr),m_pfreeList(nullptr)
{
	Reset(dataSize, pageSize, alignment);
}

Allocator::~Allocator(void)
{
	FreeAll();
}

void Allocator::Reset(unsigned dataSize, unsigned pageSize, unsigned alignment)
{
	FreeAll();
	m_dataSize = dataSize;
	m_pageSize = pageSize;
	
	unsigned maxHeadSize = std::max(dataSize,(unsigned)sizeof(BlockHeader));

	m_alignmentSize =maxHeadSize % alignment?alignment - maxHeadSize % alignment:0;
	m_blockSize = maxHeadSize + m_alignmentSize;
	m_blockPerPage = m_pageSize / m_blockSize;
	
}

void* Allocator::Allocate(void)
{
	if (!m_pfreeList)
	{
		PageHeader* newPage = reinterpret_cast<PageHeader*>(new char[m_pageSize]);
		++m_numPage;
		m_numFreePage++;
		m_numBlocks += m_blockPerPage;
		m_numFreeBlocks += m_blockPerPage;
#if defined(_DEBUG)
		FillFreePage(newPage);
#endif
		if (m_ppageList)
		{
			newPage->Next = m_ppageList;
		}
		m_ppageList = newPage;
		BlockHeader* currBlock = newPage->Blocks();
		for (unsigned i = 0; i < m_blockPerPage - 1; i++)
		{
			currBlock->Next = NextBlock(currBlock);
			currBlock = NextBlock(currBlock);
		}
		currBlock->Next = nullptr;
		m_pfreeList = newPage->Blocks();
	}
	BlockHeader* freeBlock = m_pfreeList;
	m_pfreeList = m_pfreeList->Next;
	--m_numFreePage;
#if defined(_DEBUG)
	FillAllocatedBlock(freeBlock);
#endif
	return freeBlock;
}

void Allocator::Free(void* p)
{
	BlockHeader* block = reinterpret_cast<BlockHeader*>(p);;
#if defined(_DEBUG)
	FillFreeBlock(block);
#endif
	block->Next = m_pfreeList;
	m_pfreeList = block;
	++m_numFreeBlocks;
}

void Allocator::FreeAll(void)
{
	PageHeader* page = m_ppageList;
	while (page)
	{
		PageHeader* tmpPage = page;
		page = page->Next;
		delete[] reinterpret_cast<char*>(tmpPage);
	}
	m_ppageList = nullptr;
	m_pfreeList = nullptr;
	m_numPage = 0;
	m_numBlocks = 0;
	m_numFreeBlocks = 0;
	m_numFreePage = 0;
}

BlockHeader* Allocator::NextBlock(BlockHeader* p)
{
	return reinterpret_cast<BlockHeader*>(reinterpret_cast<char*>(p) + m_blockSize);
}
#if defined(_DEBUG)
void Allocator::FillAllocatedBlock(BlockHeader* block)
{
	memset(block, PATTERN_ALLOC, m_blockSize-m_alignmentSize);
	memset(
		reinterpret_cast<char*>(block)+m_blockSize - m_alignmentSize,
		PATTERN_ALIGIN,
		m_alignmentSize
	);

}

void Allocator::FillFreeBlock(BlockHeader* block)
{
	memset(block, PATTERN_FREE, m_blockSize - m_alignmentSize);
	memset(
		reinterpret_cast<char*>(block) + m_blockSize - m_alignmentSize,
		PATTERN_ALIGIN,
		m_alignmentSize
	);
}

void Allocator::FillFreePage(PageHeader* page)
{
	page->Next = nullptr;
	BlockHeader* p = page->Blocks();
	for (int i = 0; i < m_blockPerPage; i++)
	{
		FillFreeBlock(p);
		p = NextBlock(p);
	}
}
#endif