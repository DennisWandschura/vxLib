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
	class Mallocator : public Allocator<Mallocator>
	{
	public:
		inline Mallocator() {}

		inline ~Mallocator() {}

		vx::AllocatedBlock allocate(u64 size, u64 alignment)
		{
			auto alignedSize = getAlignedSize(size, alignment);

			return{ (u8*)_aligned_malloc(alignedSize, alignment), alignedSize };
		}

		vx::AllocatedBlock reallocate(const vx::AllocatedBlock &block, u64 size, u64 alignment)
		{
			auto alignedSize = getAlignedSize(size, alignment);
			return{ (u8*)_aligned_realloc(block.ptr, alignedSize, alignment), alignedSize };
		}

		u32 deallocate(const vx::AllocatedBlock &block)
		{
			_aligned_free(block.ptr);
			return 1;
		}

		void deallocateAll()
		{
		}

		bool contains(const vx::AllocatedBlock&) const
		{
			return true;
		}

		void swap(Mallocator&) {}
	};
}