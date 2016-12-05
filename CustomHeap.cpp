#include <cstring>
#include "CustomHeap.hpp"

#define HEAP_META_SIZE sizeof(s_HeapMeta)

Heap::Heap()
{
	this->m_pHeapHead = NULL;
	this->m_pHeapTail = NULL;
	this->m_heapSize = 0;
	this->m_pStartOfMem = NULL;
	this->m_bInitialized = false;
	this->m_pEndOfMem = NULL;
	this->m_pCurrAllocMem = NULL;
}

Heap::Heap(size_t heapSize)
{
	this->m_pHeapHead = NULL;
	this->m_pHeapTail = NULL;
	this->m_heapSize = heapSize;
	this->m_pStartOfMem = malloc(this->m_heapSize);

	if (this->m_pStartOfMem != NULL)
	{
		this->m_bInitialized = true;
		this->m_pEndOfMem = (void *) (((char *) m_pStartOfMem) + m_heapSize);
		this->m_pCurrAllocMem = this->m_pStartOfMem;
		memset(this->m_pStartOfMem, 0, this->m_heapSize);
	}
	else
	{
		this->m_bInitialized = false;
	}
}

Heap::~Heap( )
{
	if (NULL != this->m_pStartOfMem)
	{
		free(this->m_pStartOfMem);
	}
}

void Heap::Allocate(size_t size, void **ppData)
{
	s_HeapMeta *pHeapMeta;

	if (ppData == NULL)
	{
		return;
	}
	else if ((this->m_bInitialized == false) ||
			 (((char *) this->m_pEndOfMem) < (((char *) this->m_pCurrAllocMem) + size + HEAP_META_SIZE)))
	{
		*ppData = NULL;
		return;
	}
	else
	{
		if (this->m_pHeapHead != NULL)
		{
			bool bFound = false;
			s_HeapMeta *pHeapMetaIter = this->m_pHeapHead;

			while (NULL != pHeapMetaIter)
			{
				if ((pHeapMetaIter->free == true) && (pHeapMetaIter->size > size))
				{
					// Construct the meta information
					pHeapMeta = pHeapMetaIter;
					pHeapMeta->free = false;

					bFound = true;
					break;
				}

				pHeapMetaIter = (s_HeapMeta *) pHeapMetaIter->nextBlock;
			}

			if (!bFound)
			{
				// Construct the meta information
				pHeapMeta = (s_HeapMeta *) this->m_pCurrAllocMem;
				pHeapMeta->prevBlock = this->m_pHeapTail;
				pHeapMeta->nextBlock = NULL;
				pHeapMeta->size = size;
				pHeapMeta->free = false;

				// Construct the linked list
				if (NULL != this->m_pHeapTail)
				{
					this->m_pHeapTail->nextBlock = pHeapMeta;
				}

				this->m_pHeapTail = pHeapMeta;
			}
		}
		else
		{
			// Construct the meta information
			pHeapMeta = (s_HeapMeta *) this->m_pCurrAllocMem;
			pHeapMeta->prevBlock = NULL;
			pHeapMeta->nextBlock = NULL;
			pHeapMeta->size = size;
			pHeapMeta->free = false;

			// Construct the linked list
			this->m_pHeapHead = pHeapMeta;
			this->m_pHeapTail = pHeapMeta;
		}

		*ppData = (void *) (((char *) pHeapMeta) + HEAP_META_SIZE);

		this->m_pCurrAllocMem = (void *) (((char *) this->m_pCurrAllocMem) + size + HEAP_META_SIZE);
	}
}

void Heap::Free(void *pData)
{
	s_HeapMeta *pHeapMeta;

	if (pData == NULL)
	{
		return;
	}
	else
	{
		pHeapMeta = (s_HeapMeta *) (((char *) pData) - HEAP_META_SIZE);

		if (this->m_pHeapTail == pHeapMeta)
		{
			this->m_pCurrAllocMem = this->m_pHeapTail;
			memset(pHeapMeta, 0, pHeapMeta->size);
			this->m_pHeapTail = (s_HeapMeta *) this->m_pHeapTail->prevBlock;

			while (this->m_pHeapTail != NULL)
			{
				if (this->m_pHeapTail->free == true)
				{
					this->m_pCurrAllocMem = this->m_pHeapTail;
					memset(this->m_pHeapTail, 0, this->m_pHeapTail->size);
					this->m_pHeapTail = (s_HeapMeta *) this->m_pHeapTail->prevBlock;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			pHeapMeta->free = true;
		}
	}
}
