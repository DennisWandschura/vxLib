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
	namespace detail
	{
		template<bool GPU>
		struct LinearAllocatorImpl
		{
			typedef GpuAllocatedBlock BlockType;

			static void initialize(const BlockType block, size_t* begin, size_t* end, size_t* last)
			{
				*begin = *end = (size_t)block.offset;
				*last = (size_t)block.offset + block.size;
			}

			static BlockType allocate(size_t size, size_t alignment)
			{
				auto alignedSize = vx::getAlignedSize(size, alignment);
				auto alignedHead = vx::getAlignedSize(m_head, alignment);

				auto newHead = alignedHead + alignedSize;
				if (newHead > m_capacity)
				{
					return{ 0, 0 };
				}

				m_head = newHead;

				return{ alignedHead, alignedSize };
			}

			static void deallocate(const BlockType block, size_t* end)
			{
				if (block.size == 0)
					return;

				auto tmp = block.offset + block.size;
				if (tmp == *end)
				{
					*end = block.offset;
				}
			}
		};

		template<>
		struct LinearAllocatorImpl<false>
		{
			typedef AllocatedBlock BlockType;

			static void initialize(const BlockType block, size_t* begin, size_t* end, size_t* last)
			{
				*begin = *end = (size_t)block.ptr;
				*last = (size_t)block.ptr + block.size;
			}

			static BlockType allocate(size_t size, size_t alignment, size_t* end, size_t last)
			{
				if (size == 0)
					return { 0, 0 };

				auto alignedPtr = getAlignedSize(*end, alignment);
				auto alignedSize = getAlignedSize(size, alignment);

				auto next = alignedPtr + alignedSize;
				if (next > last)
				{
					return{ 0, 0 };
				}

				*end = next;

				return { (u8*)alignedPtr, alignedSize };
			}

			static BlockType reallocate(const BlockType block, size_t size, size_t alignment, size_t* end, size_t last)
			{
				BlockType newBlock{ nullptr, 0 };

				auto alignedPtr = getAlignedPtr(block.ptr, alignment);
				if (alignedPtr != block.ptr)
				{
					newBlock = allocate(size, alignment, end, last);
					::memcpy(newBlock.ptr, block.ptr, block.size);
					::memset(block.ptr, 0, block.size);

					deallocate(block, end);
				}
				else if (*end == (size_t)(block.ptr + block.size))
				{
					auto alignedSize = getAlignedSize(size, alignment);

					newBlock = { block.ptr, alignedSize };
					*end = (size_t)(newBlock.ptr + newBlock.size);
				}

				return newBlock;
			}

			static u32 deallocate(const BlockType block, size_t* end)
			{
				auto tmp = block.ptr + block.size;
				if (*end == (size_t)tmp)
				{
					*end = (size_t)block.ptr;
					return 1;
				}

				return 0;
			}

			static BlockType release(size_t begin, size_t last)
			{
				return  { (u8*)begin, last - begin };
			}

			static bool contains(const BlockType block, size_t begin, size_t last)
			{
				return ((size_t)block.ptr >= begin) && ((size_t)block.ptr < last);
			}
		};

		template<bool GPU>
		class LinearAllocator : public Allocator<LinearAllocator<GPU>>
		{
			typedef LinearAllocatorImpl<GPU> MyImpl;
			typedef typename MyImpl::BlockType BlockType;

			size_t m_begin;
			size_t m_end;
			size_t m_last;

		public:
			LinearAllocator() :m_begin(0), m_end(0), m_last(0) {}
			~LinearAllocator() = default;

			LinearAllocator(const LinearAllocator&) = delete;

			LinearAllocator(LinearAllocator &&rhs)
				:m_begin(rhs.m_begin), m_end(rhs.m_end), m_last(rhs.m_last) {}

			void swap(LinearAllocator &rhs)
			{
				std::swap(m_begin, rhs.m_begin);
				std::swap(m_end, rhs.m_end);
				std::swap(m_last, rhs.m_last);
			}

			LinearAllocator& operator=(const LinearAllocator&) = delete;

			LinearAllocator& operator=(LinearAllocator &&rhs)
			{
				if (this != &rhs)
				{
					this->swap(rhs);
				}
				return *this;
			}

			void initialize(const BlockType block)
			{
				MyImpl::initialize(block, &m_begin, &m_end, &m_last);
			}

			BlockType allocate(size_t size, size_t alignment)
			{
				return MyImpl::allocate(size, alignment, &m_end, m_last);
			}

			static BlockType reallocate(const BlockType block, size_t size, size_t alignment)
			{
				return MyImpl::reallocate(block, size, alignment, &m_end, m_last);
			}

			u32 deallocate(const BlockType block)
			{
				return MyImpl::deallocate(block, &m_end);
			}

			BlockType release()
			{
				auto block =  MyImpl::release(m_begin, m_last);
				m_begin = m_end = m_last = 0;
				return block;
			}

			void deallocateAll()
			{
				m_end = m_begin;
			}

			bool contains(const BlockType block) const
			{
				return MyImpl::contains(block, m_begin, m_last);
			}

			size_t capacity() const { return m_last - m_data; }
		};
	}

	typedef detail::LinearAllocator<false> LinearAllocator;
	typedef detail::LinearAllocator<true> GpuLinearAllocator;
}