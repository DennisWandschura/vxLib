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
#ifndef __VX_VECTOR_H
#define __VX_VECTOR_H
#pragma once

#include <vxLib\Container\iterator.h>
#include <new>
#include <vxLib\Allocator\Allocator.h>

namespace vx
{
	template<class T, class Allocator = StlAllocator<T>, typename = typename std::enable_if<std::is_object<T>::value>::type>
	class vector
	{
	public:
		typedef T value_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef const pointer const_pointer;

		typedef u32 size_type;
		typedef u64 difference_type;

		typedef vector<T, Allocator> _MyVector;
		typedef vector_const_iterator<_MyVector> const_iterator;
		typedef vector_iterator<_MyVector> iterator;

	private:
		pointer m_pData;
		u32 m_size;
		u32 m_capacity;

		// destroys all current objects and deallocates used memory
		void cleanUp() noexcept
		{
			Allocator::destroy(m_pData, m_pData + m_size);
			Allocator::deallocate(m_pData);
		}

	public:
		vector() :m_pData(nullptr),m_size(0), m_capacity(0) {}

		vector(const vector &rhs)
			:m_pData(nullptr),
			m_size(rhs.m_size),
			m_capacity(0)
		{
			reserve(rhs.size());
			std::copy(rhs.begin(), rhs.end(), m_pData);
		}

		vector(vector &&rhs) noexcept
			:m_pData(rhs.m_pData), 
			m_size(rhs.m_size),
			m_capacity(rhs.m_capacity)
		{

			rhs.m_pData = nullptr;
			rhs.m_size = 0;
			rhs.m_capacity = 0;
		}

		~vector()
		{
			cleanUp();
		}

		vector& operator=(vector &&rhs) noexcept
		{
			if (this != &rhs)
			{
				this->swap(rhs);
			}
			return *this;
		}

		void swap(vector &rhs) noexcept
		{
			std::swap(m_pData, rhs.m_pData);
			std::swap(m_size, rhs.m_size);
			std::swap(m_capacity, rhs.m_capacity);
		}

		pointer data()
		{
			return m_pData;
		}

		const_pointer data() const
		{
			return m_pData;
		}

		size_type size() const noexcept
		{
			return m_size;
		}

		size_type capacity() const noexcept
		{
			return m_capacity;
		}

		void push_back(const value_type &v)
		{
			auto sz = size();
			auto cap = capacity();
			if (sz >= cap)
			{
				reserve(cap * 1.3f + 1);
			}

			auto p = data() + sz;

			Allocator::construct(p, v);

			++m_size;
		}

		void push_back(value_type &&v)
		{
			auto sz = size();
			if (sz == m_capacity)
			{
				reserve(m_capacity * 1.3f + 1);
			}

			auto p = data() + sz;

			Allocator::construct(p, std::forward<value_type>(v));

			++m_size;
		}

		void pop_back()
		{
			VX_ASSERT(!empty());
			Allocator::destroy(&this->back());
			--m_size;
		}

		void reserve(size_type n)
		{
			// if requested capacity is smaller/equal to current, do nothing
			// the minimum capacity is N
			if (n <= m_capacity)
				return;

			// the minimum capacity is N, so if we need more than that, we need to use the heap
			auto p = Allocator::allocate(n);

			// move old stuff into new array
			std::move(begin(), end(), p);
			//rangeMove(getPtr(), getPtr() + m_size, p);

			// deallocate old data if we used heap previously
			Allocator::deallocate(m_pData);

			// assign new values
			m_pData = p;
			m_capacity = n;
		}

		void clear()
		{
			Allocator::destroy(m_pData, m_pData + m_size);
			m_size = 0;
		}

		// moves the last element to pos
		iterator erase(const_iterator pos)
		{
			// get pointer of object
			auto p = pos.m_pObject;
			// destroy object
			Allocator::destroy(p);

			// move last element to object pos
			*p = std::move(*(m_pData + m_size - 1));
			//std::move(p + 1, data() + size(), p);
			//std::move(pos + 1, end(), p);
			--m_size;
			return iterator(p, this);
		}

		bool empty() const noexcept
		{
			return m_size == 0;
		}

		reference operator[](size_type i)
		{
			return m_pData[i];
		}

		const_reference operator[](size_type i) const
		{
			return m_pData[i];
		}

		reference front()
		{
			return (*begin());
		}

		const_reference front() const
		{
			return (*begin());
		}

		reference back()
		{
			return (*(end() - 1));
		}

		const_reference back() const
		{
			return (*(end() - 1));
		}

		iterator begin()
		{
			return iterator(m_pData, this);
		}

		const_iterator begin() const
		{
			return const_iterator(m_pData, this);
		}

		iterator end()
		{
			return iterator(m_pData + m_size, this);
		}

		const_iterator end() const
		{
			return const_iterator(m_pData + m_size, this);
		}
	};
}

#endif