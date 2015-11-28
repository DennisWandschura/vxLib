#pragma once

#include <vxLib/Allocator/Allocator.h>

namespace vx
{
	template<size_t SIZE, size_t ALIGNMENT>
	class alignas(ALIGNMENT) StackAllocator : public Allocator<StackAllocator<SIZE, ALIGNMENT>>
	{
		enum : size_t { ClassDataSize = SIZE + sizeof(u8*) + sizeof(u8*) };

		u8 m_data[SIZE];
		u8* m_head;
		u8* m_last;
		Padding<GetAlignedSize<ClassDataSize, ALIGNMENT>::size - ClassDataSize> m_padding;

	public:
		inline StackAllocator() :m_data(), m_head(m_data), m_last(m_data + SIZE) {}

		inline ~StackAllocator() {}

		vx::AllocatedBlock allocate(size_t size, size_t alignment)
		{
			auto alignedPtr = getAlignedPtr(m_head, alignment);
			auto alignedSize = getAlignedSize(size, alignment);

			auto next = alignedPtr + alignedSize;
			if (next > m_last)
			{
				return{ nullptr, 0 };
			}

			m_head = next;

			return{ alignedPtr, alignedSize };
		}

		u32 deallocate(const vx::AllocatedBlock &block)
		{
			auto tmp = block.ptr + block.size;
			if (m_head == tmp)
			{
				m_head = block.ptr;
				return 1;
			}

			return 0;
		}

		void deallocateAll()
		{
			m_head = m_data;
		}

		bool contains(const vx::AllocatedBlock &block) const
		{
			return (block.ptr >= m_data) && (block.ptr < m_last);
		}

		vx::AllocatedBlock release()
		{
			return{ nullptr, 0 };
		}
	};
}