/*
The MIT License(MIT)

Copyright(c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <vxLib/Allocator/PoolAllocator.h>
#include <algorithm>

namespace vx
{
	PoolAllocator::PoolAllocator() noexcept
		:m_pMemory(nullptr),
		m_firstFreeEntry(0),
		m_freeEntries(0),
		m_poolSize(0),
		m_size(0)
	{
	}

	PoolAllocator::PoolAllocator(U8 *ptr, U32 size, U32 poolSize, U8 alignment) noexcept
		:m_pMemory(ptr),
		m_firstFreeEntry(0),
		m_freeEntries(0),
		m_poolSize(poolSize),
		m_size(size)
	{
		auto adjustment = vx::AllocatorBase::getAdjustment(ptr, alignment);
		m_firstFreeEntry = adjustment;

		U8 *pCurrent = ptr + adjustment;
		poolSize = poolSize + vx::AllocatorBase::getAdjustment(pCurrent + poolSize, alignment);

		while (size >= poolSize)
		{
			auto pNext = pCurrent + poolSize;
			*reinterpret_cast<U32*>(pCurrent) = (pNext - ptr);

			pCurrent = pNext;

			++m_freeEntries;
			size -= poolSize;
		}
	}

	PoolAllocator::~PoolAllocator()
	{
		m_pMemory = nullptr;
		m_freeEntries = 0;
	}

	PoolAllocator& PoolAllocator::operator = (PoolAllocator &&rhs)
	{
		if (this != &rhs)
		{
			std::swap(m_pMemory, rhs.m_pMemory);
			std::swap(m_firstFreeEntry, rhs.m_firstFreeEntry);
			std::swap(m_freeEntries, rhs.m_freeEntries);
			std::swap(m_poolSize, rhs.m_poolSize);
			std::swap(m_size, rhs.m_size);
		}
		return *this;
	}

	void* PoolAllocator::allocate(U32 size) noexcept
	{
		VX_UNREFERENCED_PARAMETER(size);

		if (m_freeEntries == 0)
			return nullptr;

		auto ptr = m_pMemory + m_firstFreeEntry;
		m_firstFreeEntry = *((U32*)ptr);
		--m_freeEntries;

		return ptr;
	}

		void* PoolAllocator::allocate(U32 size, U8 alignment) noexcept
	{
		VX_UNREFERENCED_PARAMETER(alignment);
		return allocate(size);
	}

		void PoolAllocator::deallocate(void *ptr)
	{
		if (ptr == nullptr ||
			ptr < m_pMemory ||
			m_pMemory + m_size <= ptr)
			return;

		U32 firstFreeEntry = ((U8*)ptr) - m_pMemory;
		*reinterpret_cast<U32*>(ptr) = m_firstFreeEntry;
		m_firstFreeEntry = firstFreeEntry;
		++m_freeEntries;
	}

	void* PoolAllocator::release() noexcept
	{
		auto ptr = m_pMemory;

		m_pMemory = nullptr;
		m_firstFreeEntry = 0;
		m_freeEntries = 0;
		m_poolSize = 0;
		m_size = 0;

		return ptr;
	}
}