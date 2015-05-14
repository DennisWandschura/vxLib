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

	PoolAllocator::PoolAllocator(u8 *ptr, u32 size, u32 poolSize, u8 alignment) noexcept
		:m_pMemory(ptr),
		m_firstFreeEntry(0),
		m_freeEntries(0),
		m_poolSize(poolSize),
		m_size(size)
	{
		auto adjustment = vx::Allocator::getAdjustment(ptr, alignment);
		m_firstFreeEntry = adjustment;

		u8 *pCurrent = ptr + adjustment;
		poolSize = poolSize + vx::Allocator::getAdjustment(pCurrent + poolSize, alignment);

		while (size >= poolSize)
		{
			auto pNext = pCurrent + poolSize;
			*reinterpret_cast<u32*>(pCurrent) = (pNext - ptr);

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

	u8* PoolAllocator::allocate(u64) noexcept
	{
		if (m_freeEntries == 0)
			return nullptr;

		auto ptr = m_pMemory + m_firstFreeEntry;
		m_firstFreeEntry = *((u32*)ptr);
		--m_freeEntries;

		return ptr;
	}

	u8* PoolAllocator::allocate(u64 size, u8) noexcept
	{
		return allocate(size);
	}

		void PoolAllocator::deallocate(u8 *ptr)
	{
		if (ptr == nullptr ||
			ptr < m_pMemory ||
			m_pMemory + m_size <= ptr)
			return;

		u32 firstFreeEntry = ((u8*)ptr) - m_pMemory;
		*reinterpret_cast<u32*>(ptr) = m_firstFreeEntry;
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