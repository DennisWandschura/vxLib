#pragma once

#include <vxLib/Allocator/Allocator.h>

namespace vx
{
	template<typename Allocator>
	class DelegateAllocator
	{
		Allocator* m_ptr;

	public:
		DelegateAllocator() :m_ptr(nullptr) {}
		explicit DelegateAllocator(Allocator* ptr) :m_ptr(ptr) {}
		DelegateAllocator(const DelegateAllocator &rhs) :m_ptr(rhs.m_ptr) {}
		DelegateAllocator(DelegateAllocator &&rhs) :m_ptr(rhs.m_ptr) { rhs.m_ptr = nullptr; }

		~DelegateAllocator() {}

		DelegateAllocator& operator=(const DelegateAllocator &rhs)
		{
			if (this != &rhs)
			{
				m_ptr = rhs.m_ptr;
			}
			return *this;
		}

		DelegateAllocator& operator=(DelegateAllocator &&rhs)
		{
			if (this != &rhs)
			{
				swap(rhs);
			}
			return *this;
		}

		void swap(DelegateAllocator &other)
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