#pragma once

/*
The MIT License (MIT)

Copyright (c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <vxLib/Allocator/Allocator.h>
#include <vxLib/algorithm.h>

namespace vx
{
	class LinearAllocator : public Allocator<LinearAllocator>
	{
		u8* m_data;
		u8* m_head;
		u8* m_last;

	public:
		inline LinearAllocator() :m_data(), m_head(nullptr), m_last(nullptr) {}

		inline explicit LinearAllocator(const AllocatedBlock &block) : m_data(block.ptr), m_head(block.ptr), m_last(block.ptr + block.size) {}

		LinearAllocator(const LinearAllocator&) = delete;

		LinearAllocator(LinearAllocator &&rhs)
			:m_data(rhs.m_data), m_head(rhs.m_head), m_last(rhs.m_last) {}

		inline ~LinearAllocator() {}

		LinearAllocator& operator=(const LinearAllocator&) = delete;

		LinearAllocator& operator=(LinearAllocator &&rhs)
		{
			if (this != &rhs)
			{
				this->swap(rhs);
			}
			return *this;
		}

		void swap(LinearAllocator &rhs)
		{
			std::swap(m_data, rhs.m_data);
			std::swap(m_head, rhs.m_head);
			std::swap(m_last, rhs.m_last);
		}

		void initialize(const AllocatedBlock &block)
		{
			m_data = m_head = block.ptr;
			m_last = block.ptr + block.size;
		}

		AllocatedBlock allocate(size_t size, size_t alignment)
		{
			if (size == 0)
				return{ nullptr, 0 };
			auto alignedPtr = getAlignedPtr(m_head, alignment);
			auto alignedSize = getAlignedSize(size, alignment);

			auto next = alignedPtr + alignedSize;
			if (next > m_last)
			{
				return{ nullptr, 0 };
			}

			m_head = next;

			return{ alignedPtr, alignedSize };
		}

		AllocatedBlock reallocate(const vx::AllocatedBlock &block, size_t size, size_t alignment)
		{
			AllocatedBlock newBlock{ nullptr, 0 };

			auto alignedPtr = getAlignedPtr(block.ptr, alignment);
			if (alignedPtr != block.ptr)
			{
				newBlock = allocate(size, alignment);
				::memcpy(newBlock.ptr, block.ptr, block.size);
				::memset(block.ptr, 0, block.size);

				deallocate(block);
			}
			else if (m_head == (block.ptr + block.size))
			{
				auto alignedSize = getAlignedSize(size, alignment);

				newBlock = { block.ptr, alignedSize };
				m_head = newBlock.ptr + newBlock.size;
			}

			return newBlock;
		}

		u32 deallocate(const AllocatedBlock &block)
		{
			auto tmp = block.ptr + block.size;
			if (m_head == tmp)
			{
				m_head = block.ptr;
				return 1;
			}

			return 0;
		}

		void deallocateAll()
		{
			m_head = m_data;
		}

		bool contains(const AllocatedBlock &block) const
		{
			return (block.ptr >= m_data) && (block.ptr < m_last);
		}

		AllocatedBlock release()
		{
			AllocatedBlock block{ m_data, (size_t)m_last - (size_t)m_data};

			m_data = m_head = m_last = nullptr;

			return block;
		}

		size_t capacity() const { return m_last - m_data; }

		void zeroMemory()
		{
			::memset(m_data, 0, capacity());
		}
	};
}