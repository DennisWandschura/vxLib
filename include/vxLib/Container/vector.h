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

#include <vxLib/AlignedStorage.h>
#include <vxLib\Container\iterator.h>
#include <new>
#include <vxLib\Allocator\Allocator.h>

#include <vector>

namespace vx
{
	template<class T,class Allocator>
	class vector_base
	{
	public:
		using value_type = T;
		using reference = value_type&;
		using const_reference = const reference;
		using pointer = value_type*;
		using const_pointer = const pointer;

		using size_type = U32;
		using difference_type = U64;

	protected:
		void rangeDestroy(pointer start, pointer end) noexcept
		{
			for (; start != end; ++start)
			{
				Allocator::destroy(start);
			}
		}
	};

	template<class T, U32 N, class Allocator = StlAllocator<T, HeapAllocator>, typename = typename std::enable_if_t<std::is_object<T>::value>>
	class hybrid_vector : public vector_base<T, Allocator>
	{
	public:
		enum { SIZE = N };

		using MyBase = vector_base < T, Allocator>;

		using value_type = typename MyBase::value_type;
		using reference = typename MyBase::reference;
		using const_reference = typename MyBase::const_reference;
		using pointer = typename MyBase::pointer;
		using const_pointer = typename MyBase::const_pointer;

		using size_type = typename MyBase::size_type;
		using difference_type = typename MyBase::difference_type;

		using _MyVector = hybrid_vector<T, N, Allocator>;
		using const_iterator = vector_const_iterator<_MyVector>;
		using iterator = vector_iterator<_MyVector>;

	private:
		union
		{
			AlignedStorageN<T, N> m_data;
			pointer m_pData;
		};
		size_type m_size;
		size_type m_capacity;

		pointer getPtr()
		{
			if (m_capacity == N)
				return (pointer)m_data.m_buffer;
			else
				return m_pData;
		}

		const_pointer getPtr() const
		{
			if (m_capacity == N)
				return (const_pointer)m_data.m_buffer;
			else
				return m_pData;
		}

		// destroys all current objects and deallocates used memory
		void cleanUp() noexcept
		{
			auto p = getPtr();
			rangeDestroy(p, p + size());

			clearMemory();
		}

		// deallocates used memory if we are using heap memory
		void clearMemory() noexcept
		{
			if (m_capacity > N)
			{
				Allocator::deallocate(m_pData);
			}
		}

	public:
		hybrid_vector() :m_size(0), m_capacity(N) {}

		hybrid_vector(const hybrid_vector &rhs) = delete;

		hybrid_vector(hybrid_vector &&rhs)
			:m_size(rhs.m_size),
			m_capacity(rhs.m_capacity)
		{
			if (m_capacity == N)
			{
				//rangeMove(rhs.m_data, rhs.m_data + m_size, m_data);
				std::move(rhs.begin(), rhs.end(), begin());
			}
			else
			{
				m_pData = rhs.m_pData;
			}

			rhs.m_size = 0;
			rhs.m_capacity = N;
		}

		~hybrid_vector()
		{
			cleanUp();
		}

		void swap(hybrid_vector &rhs)
		{
			std::swap(m_data, rhs.m_data);
			std::swap(m_size, rhs.m_size);
			std::swap(m_capacity, rhs.m_capacity);
		}

		void push_back(const value_type &v)
		{
			auto sz = size();
			if (sz == m_capacity)
			{
				reserve(m_capacity * 1.3f + 1);
			}

			auto p = getPtr() + sz;

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

			auto p = getPtr() + sz;

			Allocator::construct(p, std::forward<value_type>(v));

			++m_size;
		}

		void pop_back()
		{
			VX_ASSERT(!empty());
			Allocator::destroy(getPtr() + m_size - 1);
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
			clearMemory();

			// assign new values
			m_pData = p;
			m_capacity = n;
		}

		void clear()
		{
			auto p = getPtr();
			rangeDestroy(p, p + size());
			m_size = 0;
		}

		iterator erase(const_iterator pos)
		{
			auto p = pos.m_pObject;
			Allocator::destroy(p);

			//rangeMove(p + 1, getPtr() + size(), p);
			std::move(pos + 1, end(), p);

			return iterator(p, this);
		}

		bool empty() const
		{
			return m_size == 0;
		}

		size_type size() const
		{
			return m_size;
		}

		size_type capacity() const
		{
			return m_capacity;
		}

		reference operator[](size_type i)
		{
			VX_ASSERT(i < m_size);
			return getPtr()[i];
		}

		const_reference operator[](size_type i) const
		{
			VX_ASSERT(i < m_size);
			return getPtr()[i];
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

		pointer data()
		{
			return getPtr();
		}

		const_pointer data() const
		{
			return getPtr();
		}

		iterator begin()
		{
			return iterator(getPtr(), this);
		}

		const_iterator begin() const
		{
			return const_iterator(getPtr(), this);
		}

		iterator end()
		{
			return iterator(getPtr() + size(), this);
		}

		const_iterator end() const
		{
			return const_iterator(getPtr() + size(), this);
		}
	};

	template<class T, class Allocator = StlAllocator<T, HeapAllocator>, typename = typename std::enable_if_t<std::is_object<T>::value>>
	class vector
	{
	public:
		using value_type = T;
		using reference = value_type&;
		using const_reference = const reference;
		using pointer = value_type*;
		using const_pointer = const pointer;

		using size_type = U32;
		using difference_type = U64;

		using _MyVector = vector<T, Allocator>;
		using const_iterator = vector_const_iterator<_MyVector>;
		using iterator = vector_iterator<_MyVector>;

	private:
		pointer m_pData;
		U32 m_size;
		U32 m_capacity;

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

		void push_back(const value_type &v)
		{
			auto sz = size();
			if (sz == (auto cap = capacity()))
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
			auto p = getPtr();
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

			size_type size() const noexcept
		{
			return m_size;
		}

			size_type capacity() const noexcept
		{
			return m_capacity;
		}

		reference operator[](size_type i)
		{
			VX_ASSERT(i < m_size);
			return getPtr()[i];
		}

		const_reference operator[](size_type i) const
		{
			VX_ASSERT(i < m_size);
			return getPtr()[i];
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

		pointer data()
		{
			return m_pData;
		}

		const_pointer data() const
		{
			return m_pData;
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