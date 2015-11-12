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

template<typename T, typename Allocator>
class array
{
	typedef T value_type;
	typedef value_type* pointer;

	AllocatedBlock m_block;
	pointer m_end;
	Allocator* m_allocator;

public:
	array(): m_block(), m_end(nullptr), m_allocator(nullptr){}

	explicit array(Allocator* alloc, size_t capacity) : m_block(), m_end(nullptr), m_allocator(alloc)
	{
		m_block = alloc->allocate(sizeof(value_type) * capacity, __alignof(value_type));
		m_end = (pointer)m_block.ptr;
	}

	array(const array&) = delete;

	array(array &&other)
		: m_block(other.m_block),
		m_end(other.m_end),
		m_allocator(other.m_allocator)
	{
		other.m_block.ptr = nullptr;
		other.m_block.size = 0;
		other.m_allocator = nullptr;
	}

	~array()
	{
		if (m_allocator)
		{
			clear();
			m_allocator->deallocate(m_block);
		}
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
		std::swap(m_block, other.m_block);
		std::swap(m_end, other.m_end);
		std::swap(m_allocator, other.m_allocator);
	}

	template<typename ...Args>
	void push_back(Args&&... args)
	{
		auto last = (pointer)(m_block.ptr + m_block.size);
		auto end = m_end;
		if (end >= last)
		{
			return;
		}

		new(m_end) value_type{std::forward<Args>(args)...};
		++m_end;
	}

	void pop_back()
	{
		(m_end - 1)->~value_type();
		--m_end;
	}

	void clear()
	{
		auto p = begin();
		auto e = end();

		while(p != e)
		{
			p->~value_type();
			++p;
		}

		m_end = begin();
	}

	inline value_type& operator[](size_t idx)
	{
		return begin()[idx];
	}

	inline const value_type& operator[](size_t idx) const
	{
		return begin()[idx];
	}

	inline pointer begin()
	{
		return (pointer)m_block.ptr;
	}

	inline pointer end()
	{
		return m_end;
	}

	inline const pointer begin() const
	{
		return (pointer)m_block.ptr;
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

	inline size_t capacity() const
	{
		auto last = (pointer)(m_block.ptr + m_block.size);

		return last - begin();
	}
};