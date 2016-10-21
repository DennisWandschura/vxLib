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

namespace vx
{
	template<u64 SIZE, u64 ALIGNMENT>
	class alignas(ALIGNMENT) StackAllocator : public Allocator<StackAllocator<SIZE, ALIGNMENT>>
	{
		enum : u64 { ClassDataSize = SIZE + sizeof(u8*) + sizeof(u8*) };

		u8 m_data[SIZE];
		u8* m_head;
		u8* m_last;
		Padding<GetAlignedSize<ClassDataSize, ALIGNMENT>::size - ClassDataSize> m_padding;

	public:
		inline StackAllocator() :m_data(), m_head(m_data), m_last(m_data + SIZE) {}

		inline ~StackAllocator() {}

		vx::AllocatedBlock allocate(u64 size, u64 alignment)
		{
			if (size == 0)
			{
				return{ nullptr, 0 };
			}
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

		AllocatedBlock reallocate(const vx::AllocatedBlock &block, u64 size, u64 alignment)
		{
			AllocatedBlock newBlock{nullptr, 0};

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

		u32 deallocate(const vx::AllocatedBlock &block)
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

		bool contains(const vx::AllocatedBlock &block) const
		{
			return (block.ptr >= m_data) && (block.ptr < m_last);
		}

		vx::AllocatedBlock release()
		{
			return{ nullptr, 0 };
		}
	};
}