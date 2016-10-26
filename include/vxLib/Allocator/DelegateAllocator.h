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
#include <vxLib/TypeInfo.h>

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

		AllocatedBlock reallocate(const vx::AllocatedBlock &block, size_t size, size_t alignment)
		{
			return m_ptr->reallocate(block, size, alignment);
		}

		void deallocate(const vx::AllocatedBlock &block)
		{
			m_ptr->deallocate(block);
		}
	};

	namespace detail
	{
		template<typename T>
		struct GetTypeInfo<DelegateAllocator<T>>
		{
			static const auto& get()
			{
				static const auto typeInfo{ get_constexpr(); };
				return typeInfo;
			}

			constexpr static auto get_constexpr()
			{
				return getTypeInfo(concat("vx::DelegateAllocator<", GetTypeInfo<T>::get_constexpr().m_name, ">"), sizeof(DelegateAllocator<T>), __alignof(DelegateAllocator<T>));
			}
		};
	}
}