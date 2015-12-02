#pragma once

#include <vxLib/Allocator/Allocator.h>

namespace vx
{
	template<typename Allocator>
	class DefaultContainerAllocator
	{
		Allocator* m_ptr;

	public:
		DefaultContainerAllocator() :m_ptr(nullptr) {}
		explicit DefaultContainerAllocator(Allocator* ptr) :m_ptr(ptr) {}
		DefaultContainerAllocator(const DefaultContainerAllocator&) = delete;
		DefaultContainerAllocator(DefaultContainerAllocator &&rhs) :m_ptr(rhs.m_ptr) { rhs.m_ptr = nullptr; }

		~DefaultContainerAllocator() {}

		DefaultContainerAllocator& operator=(const DefaultContainerAllocator& ) = delete;

		DefaultContainerAllocator& operator=(DefaultContainerAllocator &&rhs)
		{
			if (this != &rhs)
			{
				swap(rhs);
			}
			return *this;
		}

		void swap(DefaultContainerAllocator &other)
		{
			auto tmp = m_ptr;
			m_ptr = other.m_ptr;
			other.m_ptr = tmp;
		}

		vx::AllocatedBlock allocate(size_t size, size_t alignment)
		{
			return m_ptr->allocate(size, alignment);
		}

		void deallocate(const vx::AllocatedBlock &block)
		{
			m_ptr->deallocate(block);
		}
	};
}