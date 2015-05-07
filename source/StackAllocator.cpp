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
#include <vxLib/Allocator/StackAllocator.h>

namespace vx
{
	StackAllocator::StackAllocator()
		:m_pMemory(nullptr),
		m_head(0),
		m_size(0)
	{

	}

	StackAllocator::StackAllocator(u8 *ptr, u32 size)
		:m_pMemory(ptr),
		m_head(0),
		m_size(size)
	{
		VX_ASSERT(ptr != nullptr);

		auto adjustment = vx::AllocatorBase::getAdjustment(m_pMemory, 16);
		VX_ASSERT(adjustment == 0);
	}

	StackAllocator::~StackAllocator()
	{
		m_pMemory = nullptr;
		m_head = 0;
		m_size = 0;
	}

	StackAllocator& StackAllocator::operator = (StackAllocator &&rhs)
	{
		if (this != &rhs)
		{
			this->swap(rhs);
		}
		return *this;
	}

	u8* StackAllocator::allocate(u32 size) noexcept
	{
		u8 *ptr = nullptr;

		if (m_head + size <= m_size)
		{
			ptr = m_pMemory + m_head;
			m_head += size;
		}

		return ptr;
	}

		u8* StackAllocator::allocate(u32 size, u8 alignment) noexcept
	{
		u8 *ptr = nullptr;

		auto pHead = m_pMemory + m_head;
		auto adjustment = vx::AllocatorBase::getAdjustment(pHead, alignment);
		auto adjustedSize = size + adjustment;

		if (m_head + adjustedSize <= m_size)
		{
			ptr = pHead + adjustment;
			m_head += adjustedSize;
		}

		return ptr;
	}

		void StackAllocator::deallocate(void*) noexcept
	{
	}

	void StackAllocator::clear() noexcept
	{
		m_head = 0;
	}

		void StackAllocator::clear(Marker marker) noexcept
	{
		if (m_head >= marker)
		m_head = marker;
	}

		void* StackAllocator::release() noexcept
	{
		auto ptr = m_pMemory;

		m_pMemory = nullptr;
		m_head = 0;
		m_size = 0;

		return ptr;
	}

		void StackAllocator::swap(StackAllocator &rhs) noexcept
	{
		auto pMemory = m_pMemory;
		auto head = m_head;
		auto size = m_size;

		m_pMemory = rhs.m_pMemory;
		m_head = rhs.m_head;
		m_size = rhs.m_size;

		rhs.m_pMemory = pMemory;
		rhs.m_head = head;
		rhs.m_size = size;
	}

		StackAllocator::Marker StackAllocator::getMarker() const
	{
		return m_head;
	}
}