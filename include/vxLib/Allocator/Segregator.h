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
	template<typename Large, typename Small, size_t SMALL_LIMIT>
	class Segregator : public Large, public Small
	{
	public:
		Segregator() :Large(), Small() {}

		template<typename Arg0, typename Arg1>
		Segregator(Arg0 &&arg0, Arg1 &&arg1) : Large(std::forward<Args0>(arg0)), Small(std::forward<Arg1>(arg1)) {}

		template<typename Arg0, typename ...Arg1>
		void initialize(Arg0 &&arg0, Arg1 && ...arg1)
		{
			Large::initialize(std::forward<Arg0>(arg0));
			Small::initialize(std::forward<Arg1>(arg1)...);
		}

		vx::AllocatedBlock allocate(size_t size, size_t alignment)
		{
			if (size <= SMALL_LIMIT)
				return Small::allocate(size, alignment);

			return Large::allocate(size, alignment);
		}

		u32 deallocate(const vx::AllocatedBlock &block)
		{
			if (block.size <= SMALL_LIMIT)
			{
				return Small::deallocate(block);
			}

			return Large::deallocate(block);
		}

		bool contains(const vx::AllocatedBlock &block) const
		{
			return Large::contains(block) || Small::contains(block);
		}

		decltype(auto) release()
		{
			return std::make_pair(Large::release(),
				Small::release());
		}

		void print() const
		{
			Large::print();
			Small::print();
		}
	};
}