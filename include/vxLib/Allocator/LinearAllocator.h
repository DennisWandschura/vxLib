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
#include <cstdio>

namespace vx
{
	class LinearAllocator : public Allocator<LinearAllocator>
	{
		u8* m_data;
		u8* m_head;
		u8* m_last;

	public:
		inline LinearAllocator() :m_data(), m_head(nullptr), m_last(nullptr) {}

		inline LinearAllocator(const AllocatedBlock &block) : m_data(block.ptr), m_head(block.ptr), m_last(block.ptr + block.size) {}

		inline ~LinearAllocator() {}

		void initialize(const AllocatedBlock &block)
		{
			m_data = m_head = block.ptr;
			m_last = block.ptr + block.size;
		}

		AllocatedBlock allocate(size_t size, size_t alignment)
		{
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

		void print() const
		{
			printf("total size %llu\n", (size_t)m_last - (size_t)m_data);
		}
	};
}