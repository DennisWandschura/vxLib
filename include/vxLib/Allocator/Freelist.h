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
			u64 size;
		};
	};

	template<typename Super, u64 MAX_NODE_COUNT, u64 MIN_SIZE, u64 MAX_SIZE>
	class Freelist : public Super
	{
		const static u32 MAGIC_NUMBER = 0x1337b0b;

		typedef detail::FreelistNode Node;

		template<u64 MIN, u64 MAX>
		struct SizeCheck
		{
			static_assert(sizeof(Node) <= MIN, "");

			inline bool operator()(u64 size)
			{
				return size >= MIN && size <= MAX;
			}
		};

		template<u64 SZ>
		struct SizeCheck<SZ, SZ>
		{
			static_assert(sizeof(Node) <= SZ, "");

			inline bool operator()(u64 size)
			{
				return size <= SZ;
			}
		};

		template<>
		struct SizeCheck<0, 0>
		{
			inline bool operator()(u64)
			{
				return true;
			}
		};

		template<u64 MAX_COUNT>
		struct NodeCountCheck
		{
			bool operator()(u64 count)
			{
				return count < MAX_COUNT
			}
		};

		template<>
		struct NodeCountCheck<0>
		{
			bool operator()(u64)
			{
				return true;
			}
		};

		typedef SizeCheck<MIN_SIZE, MAX_SIZE> MySizeCheck;
		typedef NodeCountCheck<MAX_NODE_COUNT> MyNodeCountCheck;

		static_assert(MIN_SIZE <= MAX_SIZE, "");
		//static_assert(sizeof(Node) <= MIN_SIZE, "");

		Node* m_head;
		u64 m_nodeCount;

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

		Freelist(const Freelist&) = delete;

		Freelist(Freelist &&rhs)
			:Super(std::move(rhs)),
			m_head(rhs.m_head),
			m_nodeCount(rhs.m_nodeCount)
		{
			rhs.m_head = nullptr;
			rhs.m_nodeCount = 0;
		}

		~Freelist()
		{
		}

		Freelist& operator=(const Freelist&) = delete;

		Freelist& operator=(Freelist &&rhs)
		{
			if (this != &rhs)
			{
				Super::operator=(std::move(rhs));
				std::swap(m_head, rhs.m_head);
				std::swap(m_nodeCount, rhs.m_nodeCount);
			}
			return *this;
		}

		AllocatedBlock allocate(u64 size, u64 alignment)
		{
			auto alignedSize = getAlignedSize(size, alignment);
			if (m_head &&
				MySizeCheck()(alignedSize) &&
				(getAlignedPtr((u8*)m_head, alignment) == (u8*)m_head) &&
				(m_head->size >= alignedSize))
			{
				AllocatedBlock block = { (u8*)m_head, m_head->size };

				m_head = m_head->next;
				--m_nodeCount;

				return block;
			}

			return Super::allocate(size, alignment);;
		}

		AllocatedBlock reallocate(const AllocatedBlock &block, u64 size, u64 alignment)
		{
			if ((block.size >= size) && (getAlignedPtr(block.ptr, alignment) == block.ptr))
				return block;

			auto newBlock = allocate(size, alignment);
			if (newBlock.ptr)
			{
				memcpy(newBlock.ptr, block.ptr, block.size);
				memset(block.ptr, 0, block.size);
			}

			deallocate(block);

			return newBlock;
		}

		u32 deallocate(const AllocatedBlock &block)
		{
			if (!MySizeCheck()(block.size) ||
				!MyNodeCountCheck()(m_nodeCount))
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
			return Super::contains(block);
		}
	};
}