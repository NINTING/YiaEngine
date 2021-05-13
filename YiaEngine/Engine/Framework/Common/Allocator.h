#pragma once


namespace YiaEngine
{
	struct BlockHeader
	{
		//union with data
		BlockHeader* Next;
	};

	struct  PageHeader
	{
		PageHeader* Next;
		BlockHeader* Blocks(void)
		{
			return reinterpret_cast<BlockHeader*>(this + 1);
		}
	};

	class Allocator
	{
	public:
		static const unsigned char PATTERN_ALIGIN = 0xFC;
		static const unsigned char PATTERN_ALLOC  = 0xFC;
		static const unsigned char PATTERN_FREE	  = 0xFC;
		Allocator() ;
		Allocator(unsigned dataSize,unsigned pageSize,unsigned alignment);
		~Allocator(void);
		void Reset(unsigned dataSize, unsigned pageSize, unsigned alignment);
		void* Allocate(void);
		void Free(void* p);
		void FreeAll(void);
	private:
		BlockHeader* Allocator::NextBlock(BlockHeader* p);
#if defined(_DEBUG)
		void FillAllocatedBlock(BlockHeader* block);
		void FillFreeBlock(BlockHeader* block);
		void FillFreePage(PageHeader* page);
#endif
		PageHeader* m_ppageList = nullptr;
		BlockHeader* m_pfreeList = nullptr;
		unsigned m_dataSize;	
		unsigned m_pageSize;
		unsigned m_alignmentSize;
		unsigned m_numPage;
		unsigned m_numFreePage;
		unsigned m_numBlocks;
		unsigned m_numFreeBlocks;
		unsigned m_blockSize;
		unsigned m_blockPerPage;
	};
}