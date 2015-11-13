#pragma once

#include <vxLib/Allocator/Allocator.h>

namespace vx
{
	class Mallocator : public Allocator<Mallocator>
	{
	public:
		inline Mallocator() {}

		inline ~Mallocator() {}

		vx::AllocatedBlock allocate(size_t size, size_t alignment)
		{
			auto alignedSize = getAlignedSize(size, alignment);

			return{ (u8*)_aligned_malloc(alignedSize, alignment), alignedSize };
		}

		u32 deallocate(const vx::AllocatedBlock &block)
		{
			_aligned_free(block.ptr);
			return 1;
		}

		void deallocateAll()
		{
		}

		bool contains(const vx::AllocatedBlock &block) const
		{
			return true;
		}
	};
}