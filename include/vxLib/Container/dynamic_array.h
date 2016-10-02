#pragma once

/*
The MIT License (MIT)

Copyright (c) 2016 Dennis Wandschura

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

#include <vxLib/Allocator/Mallocator.h>
#include <vxLib/algorithm.h>

namespace vx
{
	template<typename T, typename Allocator = Mallocator>
	class dynamic_array
	{
		typedef Allocator MyAllocator;

		u8* m_begin;
		u32 m_size;
		u32 m_capacity;
		u64 m_blockSize;
		MyAllocator m_allocator;

	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const pointer const_iterator;
		typedef pointer iterator;

		dynamic_array():m_begin(nullptr), m_size(0), m_capacity(0), m_blockSize(0),m_allocator(){}

		dynamic_array(MyAllocator &&allocator, u32 capacity) :m_begin(nullptr), m_size(0), m_capacity(0), m_blockSize(0), m_allocator(std::move(allocator))
		{
			if (capacity != 0)
			{
				auto block = m_allocator.allocate(sizeof(value_type) * capacity, __alignof(value_type));

				m_begin = block.ptr;
				m_capacity = static_cast<u32>(block.size / sizeof(value_type));
				m_blockSize = block.size;
			}
		}

		dynamic_array(MyAllocator &&allocator, const dynamic_array &rhs)
			:dynamic_array(std::move(allocator), rhs.capacity())
		{
			auto b = rhs.begin();
			auto e = rhs.end();

			while (b != e)
			{
				push_back(*b);
				++b;
			}
		}

		dynamic_array(dynamic_array &&rhs)
			:m_begin(rhs.m_begin),
			m_size(rhs.m_size),
			m_capacity(rhs.m_capacity),
			m_blockSize(rhs.m_blockSize),
			m_allocator(std::move(rhs.m_allocator))
		{
			rhs.m_begin = nullptr;
			rhs.m_size = 0;
			rhs.m_capacity = 0;
			rhs.m_blockSize = 0;
		}

		~dynamic_array()
		{
			release();
		}

		dynamic_array& operator=(const dynamic_array &rhs)
		{
			if (this != &rhs)
			{
				this->clear();
				this->reserve(rhs.size());

				auto b = rhs.begin();
				auto e = rhs.end();
				while (b != e)
				{
					this->push_back(*b);
					++b;
				}
			}
			return *this;
		}

		dynamic_array& operator=(dynamic_array &&rhs)
		{
			if (this != &rhs)
			{
				swap(rhs);
			}
			return *this;
		}

		reference operator[](u32 i)
		{
			return begin()[i];
		}

		const reference operator[](u32 i) const
		{
			return begin()[i];
		}

		void swap(dynamic_array &rhs)
		{
			std::swap(m_begin, rhs.m_begin);
			std::swap(m_size, rhs.m_size);
			std::swap(m_capacity, rhs.m_capacity);
			std::swap(m_blockSize, rhs.m_blockSize);
			m_allocator.swap(rhs.m_allocator);
		}

		void clear()
		{
			vx::destruct(begin(), end());
			m_size = 0;
		}

		void release()
		{
			if (m_begin)
			{
				clear();
				m_allocator.deallocate({ m_begin, m_blockSize });
				m_begin = nullptr;
				m_capacity = 0;
			}
		}

		void resize(u32 sz)
		{
			auto currentSize = size();
			if (currentSize >= sz)
				return;

			reserve(sz);

			auto d = sz - currentSize;
			while (d)
			{
				push_back(value_type{});
				--d;
			}
		}

		void reserve(u32 c)
		{
			auto currentCapacity = capacity();
			c = (c < 2) ? 2 : c;
			if (c <= currentCapacity)
				return;

			auto new_block = m_allocator.allocate(sizeof(value_type) * c, __alignof(value_type));

			if (m_begin)
			{
				vx::moveConstruct<value_type>(begin(), end(), reinterpret_cast<pointer>(new_block.ptr));

				m_allocator.deallocate({m_begin, m_blockSize});
			}

			m_begin = new_block.ptr;
			m_capacity = static_cast<u32>(new_block.size / sizeof(value_type));
			m_blockSize = new_block.size;
		}

		void push_back(const value_type &value)
		{
			auto cap = capacity();
			auto sz = size();
			if (sz >= cap)
			{
				reserve(cap * 2);
			}

			new (&(begin()[m_size++])) value_type{ value };
		}

		template<typename ...Args>
		void push_back(Args&& ...args)
		{
			auto cap = capacity();
			auto sz = size();
			if (sz >= cap)
			{
				reserve(cap * 2);
			}

			new (&(begin()[m_size++])) value_type{ std::forward<Args>(args)... };
		}

		void push_back_range(const_iterator first, const_iterator last)
		{
			auto cap = capacity();
			auto sz = size();
			auto count = last - first;
			auto newSize = static_cast<u32>(sz + count);
			if (newSize >= cap)
			{
				reserve(newSize);
			}

			auto dst = begin() + sz;
			while (first != last)
			{
				new (dst++) value_type{ *(first++) };
			}

			m_size = newSize;
		}

		void push_back_range(const dynamic_array &other)
		{
			auto first = other.begin();
			auto last = other.end();

			push_back_range(first, last);
		}

		template<typename U, typename Cvt>
		void push_back_range(const dynamic_array<U> &other, Cvt cvt)
		{
			auto first = other.begin();
			auto last = other.end();

			auto cap = capacity();
			auto sz = size();
			auto count = last - first;
			auto newSize = static_cast<u32>(sz + count);
			if (newSize >= cap)
			{
				reserve(newSize);
			}

			auto dst = begin() + sz;
			while (first != last)
			{
				new (dst++) value_type{ cvt(*(first++)) };
			}

			m_size = newSize;
		}

		void pop_back()
		{
			vx::destruct(end() - 1);
			--m_size;
		}

		pointer erase(pointer p)
		{
			auto bg = begin();
			VX_ASSERT(p >= bg && 
				(bg + capacity()) > p);

			auto idx = p - bg;
			vx::destruct(p);

			vx::move(p + 1, end(), p);
			--m_size;

			return (bg + idx);
		}

		void sort()
		{
			std::sort(begin(), end());
		}

		template<typename Cmp>
		void sort(Cmp cmp)
		{
			std::sort(begin(), end(), cmp);
		}

		const_iterator find_lower_bound(const value_type &key) const
		{
			return std::lower_bound(begin(), end(), key);
		}

		template<typename Cmp>
		const_iterator find_lower_bound(const value_type &key, Cmp cmp) const
		{
			return std::lower_bound(begin(), end(), key, cmp);
		}

		iterator find_lower_bound(const value_type &key)
		{
			return std::lower_bound(begin(), end(), key);
		}

		template<typename Cmp>
		iterator find_lower_bound(const value_type &key, Cmp cmp)
		{
			return std::lower_bound(begin(), end(), key, cmp);
		}

		reference back() { return *(begin() + m_size - 1); }
		const reference back() const { return *(begin() + m_size - 1); }

		iterator begin() { return iterator(reinterpret_cast<pointer>(m_begin)); }
		iterator end() { return begin() + m_size; }

		const_iterator begin() const { return const_iterator(reinterpret_cast<const pointer>(m_begin)); }
		const_iterator end() const { return begin() + m_size; }

		pointer data() { return reinterpret_cast<pointer>(m_begin); }
		const pointer data() const { return reinterpret_cast<const pointer>(m_begin); }

		u32 size() const { return m_size; }
		u32 capacity() const { return m_capacity; }

		bool empty() const { return (m_size == 0); }
	};
}