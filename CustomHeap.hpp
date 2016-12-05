
#ifndef CUSTOM_HEAP_HPP
#define CUSTOM_HEAP_HPP

#include <cstdlib>

typedef struct
{
	size_t size;
	void *nextBlock;
	void *prevBlock;
	bool free;
} s_HeapMeta;

class Heap
{
	public:
		~Heap();
		Heap(size_t heapSize);
		void Allocate(size_t size, void **ppData);
		void Free(void *pData);
	private:
		Heap();
		size_t m_heapSize;
		bool m_bInitialized;
		void *m_pStartOfMem;
		void *m_pEndOfMem;
		void *m_pCurrAllocMem;
		s_HeapMeta *m_pHeapHead;
		s_HeapMeta *m_pHeapTail;
};

#endif /* CUSTOM_HEAP_HPP */
