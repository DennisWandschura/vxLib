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

#include <vxLib/types.h>

namespace vx
{
	class Allocator
	{
	public:
		virtual ~Allocator(){}

		virtual u8* allocate(u64 size) = 0;
		virtual u8* allocate(u64 size, u8 alignment) = 0;
		virtual void deallocate(u8 *ptr) = 0;

		template<class U>
		static void construct(U *p)
		{
			new (p)U{};
		}

		template<class U>
		static void construct(U *p, const U &v)
		{
			new ((void*)p) U(v);
		}

		template<class U, class ...Args>
		static void construct(U *p, Args&& ...args)
		{
			new (p)U(std::forward<Args>(args)...);
		}

		template<class U>
		static void rangeConstruct(U *start, U *end) noexcept
		{
			VX_ASSERT(start <= end);
			for (; start != end; ++start)
			{
				construct(start);
			}
		}

		template<class U>
		static inline void destroy(U *p)
		{
			p->~U();
		}

		template<class U>
		static void rangeDestroy(U *start, U *end) noexcept
		{
			VX_ASSERT(start <= end);
			for (; start != end; ++start)
			{
				destroy(start);
			}
		}

		static u8 getAdjustment(void *ptr, u8 alignment) noexcept
		{
			u8 adjustment = alignment - ((uintptr_t)ptr & (alignment - 1));

			if (adjustment == alignment)
				return 0; //already aligned

			return adjustment;
		}
	};

	template<class T>
	struct StlAllocator : public Allocator
	{
		typedef T value_type;
		typedef value_type* pointer;

		static pointer allocate(u32 n)
		{
			return (pointer) ::operator new(sizeof(value_type) * n);
		}

		static void deallocate(pointer p)
		{
			::operator delete(p);
		}

		static void destroy(pointer p)
		{
			if (p != nullptr)
			{
				Allocator::destroy(p);
			}
		}
	};
}