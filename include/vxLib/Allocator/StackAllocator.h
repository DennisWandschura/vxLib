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
#ifndef __VX_STACKALLOCATOR_H
#define __VX_STACKALLOCATOR_H
#pragma once

#include <vxLib/Allocator/Allocator.h>

namespace vx
{
	class StackAllocator : public AllocatorBase
	{
		U8 *m_pMemory;
		U32 m_head;
		U32 m_size;

	public:
		typedef U32 Marker;

		StackAllocator();
		// passed in memory ptr must be aligned to 16 bytes
		StackAllocator(U8 *ptr, U32 size);
		StackAllocator(const StackAllocator&) = delete;
		StackAllocator(StackAllocator &&rhs);
		~StackAllocator();

		StackAllocator& operator=(const StackAllocator&) = delete;
		StackAllocator& operator=(StackAllocator &&rhs);

		U8* allocate(U32 size) noexcept;
		U8* allocate(U32 size, U8 alignment) noexcept;

		void deallocate(void *ptr) noexcept;

		void clear() noexcept;
		void clear(Marker marker) noexcept;

		void* release() noexcept;

		void swap(StackAllocator &rhs) noexcept;

		Marker getMarker() const;
	};
}
#endif