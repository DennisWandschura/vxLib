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

#include <vxLib/Allocator/DelegateAllocator.h>
#include <utility>

namespace vx
{
	template<typename T, typename Allocator>
	class array
	{
		typedef T value_type;
		typedef value_type* pointer;

		pointer m_begin;
		pointer m_end;
		pointer m_last;
		size_t m_blockSize;
		Allocator m_allocator;

	public:
		array() : m_begin(nullptr), m_end(nullptr), m_last(nullptr), m_blockSize(0), m_allocator() {}

		array(Allocator &&alloc, size_t capacity) : m_begin(nullptr), m_end(nullptr), m_last(nullptr), m_blockSize(0), m_allocator(std::move(alloc))
		{
			auto block = m_allocator.allocate(sizeof(value_type) * capacity, __alignof(value_type));
			m_begin = m_end = (pointer)block.ptr;
			m_last = m_begin + capacity;
			m_blockSize = block.size;
		}

		template<typename T>
		array(T* alloc, size_t capacity) : array(std::move(Allocator(alloc)), capacity) {}

		array(const array&) = delete;

		array(array &&other)
			: m_begin(other.m_begin),
			m_end(other.m_end),
			m_last(other.m_last),
			m_blockSize(other.m_blockSize),
			m_allocator(std::move(other.m_allocator))
		{
			other.m_begin = nullptr;
			other.m_blockSize = 0;
		}

		~array()
		{
			release();
		}

		array& operator=(const array&) = delete;

		array& operator=(array &&rhs)
		{
			if (this != &rhs)
			{
				swap(rhs);
			}
			return *this;
		}

		void swap(array &other)
		{
			std::swap(m_begin, other.m_begin);
			std::swap(m_end, other.m_end);
			std::swap(m_last, other.m_last);
			std::swap(m_blockSize, other.m_blockSize);
			m_allocator.swap(other.m_allocator);
		}

		template<typename ...Args>
		u32 push_back(Args&&... args)
		{
			auto last = m_last;
			auto end = m_end;
			if (end >= last)
			{
				return 0;
			}

			new(m_end) value_type{ std::forward<Args>(args)... };
			++m_end;

			return 1;
		}

		void pop_back()
		{
			(m_end - 1)->~value_type();
			--m_end;
		}

		void assign(const u8* p, size_t count)
		{
			clear();

			std::memcpy(m_begin, p, sizeof(value_type) * count);
			m_end = m_begin + count;
		}

		void clear()
		{
			auto p = begin();
			auto e = end();

			while (p != e)
			{
				p->~value_type();
				++p;
			}

			m_end = begin();
		}

		void release()
		{
			if (m_begin)
			{
				clear();

				m_allocator.deallocate(AllocatedBlock{ (u8*)m_begin, m_blockSize });

				m_begin = m_end = m_last = nullptr;
				m_blockSize = 0;
			}
		}

		inline value_type& operator[](size_t idx)
		{
			return m_begin[idx];
		}

		inline const value_type& operator[](size_t idx) const
		{
			return m_begin[idx];
		}

		value_type& back()
		{
			return *(m_end - 1);
		}

		const value_type& back() const
		{
			return *(m_end - 1);
		}

		inline const pointer data() const
		{
			return m_begin;
		}

		inline pointer begin()
		{
			return m_begin;
		}

		inline pointer end()
		{
			return m_end;
		}

		inline const pointer begin() const
		{
			return m_begin;
		}

		inline const pointer end() const
		{
			return m_end;
		}

		inline bool empty() const
		{
			return (end() == begin());
		}

		inline size_t size() const
		{
			return end() - begin();
		}

		inline size_t sizeInBytes() const
		{
			return size() * sizeof(value_type);
		}

		inline size_t capacity() const
		{
			return m_last - m_begin;
		}
	};
}