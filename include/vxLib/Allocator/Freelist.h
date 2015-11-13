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
	namespace detail
	{
		struct FreelistNode
		{
			FreelistNode* next;
			size_t size;
		};
	};

	template<typename Super, size_t MAX_NODE_COUNT, size_t MIN_SIZE, size_t MAX_SIZE>
	class Freelist : public Super
	{
		typedef detail::FreelistNode Node;

		static_assert(sizeof(Node) <= MIN_SIZE, "");
		static_assert(MIN_SIZE <= MAX_SIZE, "");

		Node* m_head;
		size_t m_nodeCount;

		inline bool isWithinSizeRange(size_t size)
		{
			return size >= MIN_SIZE && size <= MAX_SIZE;
		}

	public:
		Freelist()
			:Super(),
			m_head(nullptr),
			m_nodeCount(0)
		{}

		explicit Freelist(const AllocatedBlock &block)
			:Super(block),
			m_head(nullptr),
			m_nodeCount(0)
		{}

		~Freelist()
		{
		}

		AllocatedBlock allocate(size_t size, size_t alignment)
		{
			auto alignedSize = getAlignedSize(size, alignment);
			if (m_head && isWithinSizeRange(alignedSize) &&
				getAlignedPtr((u8*)m_head, alignment) == (u8*)m_head &&
				m_head->size <= alignedSize)
			{
				AllocatedBlock block = { (u8*)m_head, m_head->size };

				m_head = m_head->next;
				++m_nodeCount;

				return block;
			}

			return Super::allocate(size, alignment);
		}

		u32 deallocate(const AllocatedBlock &block)
		{
			if (!isWithinSizeRange(block.size) ||
				m_nodeCount >= MAX_NODE_COUNT)
			{
				return Super::deallocate(block);
			}

			auto node = (Node*)block.ptr;
			node->next = m_head;
			node->size = block.size;
			m_head = node;

			++m_nodeCount;
			return 1;
		}

		void deallocateAll()
		{
			Super::deallocateAll();
		}

		bool contains(const AllocatedBlock &block) const
		{
			Super::contains(block);
		}
	};
}