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
	template<size_t CHUNK_SIZE, size_t ALIGNMENT, typename Super>
	class ChuckAllocator : public Super
	{
		static_assert(GetAlignedSize<CHUNK_SIZE, ALIGNMENT>::size == CHUNK_SIZE, "");

	public:
		ChuckAllocator() :Super() {}

		explicit ChuckAllocator(const AllocatedBlock &block) :Super(block) {}

		ChuckAllocator(const ChuckAllocator&) = delete;

		ChuckAllocator(ChuckAllocator &&rhs)
			:Super(std::move(rhs))
		{

		}

		~ChuckAllocator() {}

		ChuckAllocator& operator=(const ChuckAllocator&) = delete;

		ChuckAllocator& operator=(ChuckAllocator &&rhs)
		{
			Super::operator=(std::move(rhs));
			return *this;
		}

		AllocatedBlock allocate(size_t, size_t)
		{
			return Super::allocate(CHUNK_SIZE, ALIGNMENT);
		}

		AllocatedBlock reallocate(const AllocatedBlock &block, size_t, size_t)
		{
			return Super::reallocate(block, CHUNK_SIZE, ALIGNMENT);
		}

		u32 deallocate(const AllocatedBlock &block)
		{
			return Super::deallocate(block);
		}

		void deallocateAll()
		{
			Super::deallocateAll();
		}

		bool contains(const AllocatedBlock &block) const
		{
			return (block.size == CHUNK_SIZE) && Super::contains(block);
		}
	};
}