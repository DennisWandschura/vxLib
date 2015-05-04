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
#ifndef __VX_POOLALLOCATOR_H
#define __VX_POOLALLOCATOR_H
#pragma once

#include <vxLib/Allocator/Allocator.h>

namespace vx
{
	class PoolAllocator : public AllocatorBase
	{
		U8 *m_pMemory;
		U32 m_firstFreeEntry;
		U32 m_freeEntries;
		U32 m_poolSize;
		U32 m_size;

	public:
		PoolAllocator() noexcept;
		PoolAllocator(U8 *ptr, U32 size, U32 poolSize, U8 alignment) noexcept;
		PoolAllocator(const PoolAllocator&) = delete;
		PoolAllocator(PoolAllocator &&rhs);
		~PoolAllocator();

		PoolAllocator& operator=(const PoolAllocator&) = delete;
		PoolAllocator& operator=(PoolAllocator &&rhs);

		// ignores size parameter
		void* allocate(U32 size) noexcept;
		// ignores size and alginment parameter
		void* allocate(U32 size, U8 alignment) noexcept;

		void deallocate(void *ptr);

		// returns pointer to memory and sets everything to zero
		void* release() noexcept;
	};
}
#endif