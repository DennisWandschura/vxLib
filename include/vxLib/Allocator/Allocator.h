#ifndef __VX_ALLOCATOR_H
#define __VX_ALLOCATOR_H
#pragma once

#include <vxLib\types.h>

namespace vx
{
	struct AllocatorBase
	{
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
			VX_ASSERT(start <= end, "");
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
			VX_ASSERT(start <= end, "");
			for (; start != end; ++start)
			{
				destroy(start);
			}
		}

		static U8 getAdjustment(void *ptr, U8 alignment) noexcept
		{
			U8 adjustment = alignment - ((uintptr_t)ptr & (alignment - 1));

			if (adjustment == alignment)
				return 0; //already aligned

			return adjustment;
		}
	};

	struct HeapAllocator : public AllocatorBase
	{
		static void* allocate(U64 size)
		{
			return ::operator new(size);
		}

		static void deallocate(void *p)
		{
			::operator delete(p);
		}
	};

	template<typename T>
	struct AlignedHeapAllocator : public AllocatorBase
	{
		static void* allocate(U64 size)
		{
			return _aligned_malloc(size, __alignof(T));
		}

		static void deallocate(void *p)
		{
			_aligned_free(p);
		}
	};

	template<class T, class AllocBase>
	struct StlAllocator : public AllocBase
	{
		using value_type = T;
		using pointer = value_type*;

		static pointer allocate(U32 n)
		{
			return (pointer)AllocBase::allocate(sizeof(value_type) * n);
		}

		static void deallocate(pointer p)
		{
			AllocBase::deallocate(p);
		}

		static void destroy(pointer p)
		{
			if (p != nullptr)
			{
				AllocBase::destroy(p);
			}
		}
	};
}
#endif