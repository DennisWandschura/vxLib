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
#include <malloc.h>
#include <utility>

namespace vx
{
	template<size_t SIZE, size_t ALIGNMENT>
	struct GetAlignedSize
	{
		enum { size = (SIZE + (ALIGNMENT - 1) & ~(ALIGNMENT - 1)) };
	};

	inline size_t getAlignedSize(size_t size, size_t alignment)
	{
		return (size + (alignment - 1) & ~(alignment - 1));
	}

	template<typename T>
	inline size_t getAlignedSize(size_t count)
	{
		const auto size = sizeof(T) * count;
		const auto alignment = __alignof(T);
		return (size + (alignment - 1) & ~(alignment - 1));
	}

	inline u8* getAlignedPtr(u8* ptr, size_t alignment)
	{
		return (u8*)getAlignedSize((size_t)ptr, alignment);
	}

	template<size_t PADDING_SIZE>
	struct Padding
	{
		u8 m_padding[PADDING_SIZE];
	};

	template<>
	struct Padding<0> {};

	struct AllocatedBlock
	{
		u8* ptr;
		size_t size;
	};

	struct GpuAllocatedBlock
	{
		size_t offset;
		size_t size;
	};

	template<typename T>
	struct Allocator
	{
	private:
		T& get() { return static_cast<T&>(*this); }

	public:
		AllocatedBlock allocate(size_t size, size_t alignment)
		{
			return get().allocate(size, alignment);
		}

		AllocatedBlock reallocate(const AllocatedBlock block, size_t size, size_t alignment)
		{
			return get().reallocate(size, alignment);
		}

		void deallocate(const AllocatedBlock block)
		{
			return get().deallocate(block);
		}

		void deallocateAll()
		{
			return get().deallocateAll();
		}

		bool contains(const AllocatedBlock block) const
		{
			return get().contains(block);
		}
	};

	class AllocatorBase
	{
	public:
		AllocatorBase() {}
		virtual ~AllocatorBase() {}

		virtual AllocatedBlock allocate(size_t size, size_t alignment) = 0;
		virtual AllocatedBlock reallocate(const AllocatedBlock block, size_t size, size_t alignment) = 0;
		virtual void deallocate(const AllocatedBlock block) = 0;

		virtual bool contains(const AllocatedBlock block) const = 0;
	};

	typedef vx::AllocatedBlock(*AllocationCallbackSignature)(size_t size, size_t alignment);
	typedef u32 (*DeallocationCallbackSignature)(const vx::AllocatedBlock block);
}