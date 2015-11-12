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
#include <algorithm>

template<typename T, typename K, typename Allocator, typename Cmp = std::less<>>
class sorted_array
{
	typedef K key_type;
	typedef T value_type;
	typedef value_type* pointer;

	AllocatedBlock m_keyBlock;
	AllocatedBlock m_dataBlock;
	size_t m_size;
	Allocator* m_allocator;

public:
	sorted_array() : m_keyBlock(), m_dataBlock(), m_size(0), m_allocator(nullptr) {}

	explicit sorted_array(Allocator* alloc, size_t capacity) 
		: m_keyBlock(), m_dataBlock(), m_size(0), m_allocator(alloc)
	{
		m_keyBlock = alloc->allocate(sizeof(key_type) * capacity, __alignof(key_type));
		m_dataBlock = alloc->allocate(sizeof(value_type) * capacity, __alignof(value_type));
	}

	sorted_array(const sorted_array&) = delete;

	sorted_array(sorted_array &&other)
		: m_keyBlock(other.m_keyBlock),
		m_dataBlock(other.m_dataBlock),
		m_size(other.m_size),
		m_allocator(other.m_allocator)
	{
		other.m_keyBlock = {nullptr, 0};
		other.m_dataBlock = {nullptr, 0};
		other.m_allocator = nullptr;
	}

	~sorted_array()
	{
		if (m_allocator)
		{
			clear();

			m_allocator->deallocate(m_keyBlock);
			m_allocator->deallocate(m_dataBlock);
		}
	}

	sorted_array& operator=(const sorted_array&) = delete;

	sorted_array& operator=(sorted_array &&rhs)
	{
		if (this != &rhs)
		{
			swap(rhs);
		}
		return *this;
	}

	void swap(sorted_array &other)
	{
		std::swap(m_keyBlock, other.m_keyBlock);
		std::swap(m_dataBlock, other.m_dataBlock);
		std::swap(m_size, other.m_size);
		std::swap(m_allocator, other.m_allocator);
	}

	template<typename ...Args>
	pointer insert(const key_type &key, Args&&... args)
	{
		auto currentSize = m_size;

		auto keyPtrBegin = ((key_type*)m_keyBlock.ptr);
		auto keyPtrEnd = keyPtrBegin + currentSize;
		auto keyPtrLast = (key_type*)(m_keyBlock.ptr + m_keyBlock.size);

		if (keyPtrEnd >= keyPtrLast)
		{
			return end();
		}

		auto it = std::lower_bound(keyPtrBegin, keyPtrEnd, key, Cmp());
		auto idx = it - keyPtrBegin;
		auto dataPtrBegin = begin();

		auto dataPtr = dataPtrBegin + idx;

		if (it == keyPtrEnd || Cmp()(key, *it))
		{
			auto dataPtrEnd = dataPtrBegin + currentSize;

			new (keyPtrEnd++) key_type{key};
			new (dataPtrEnd++) value_type{std::forward<Args>(args)...};

			std::rotate(keyPtrBegin + idx, keyPtrEnd - 1, keyPtrEnd);
			std::rotate(dataPtr, dataPtrEnd - 1, dataPtrEnd);

			++m_size;
		}

		return dataPtr;
	}

	pointer find(const key_type &key)
	{
		auto keyPtrBegin = ((key_type*)m_keyBlock.ptr);
		auto keyPtrEnd = keyPtrBegin + m_size;

		auto it = std::lower_bound(keyPtrBegin, keyPtrEnd, key, Cmp());
		auto idx = it - keyPtrBegin;

		auto dataPtrBegin = begin();
		auto result = dataPtrBegin + idx;
		if (it != keyPtrEnd && Cmp()(key, *it))
			result = end();

		return result;
	}

	const pointer find(const key_type &key) const
	{
		auto keyPtrBegin = ((key_type*)m_keyBlock.ptr);
		auto keyPtrEnd = keyPtrBegin + m_size;

		auto it = std::lower_bound(keyPtrBegin, keyPtrEnd, key, Cmp());
		auto idx = it - keyPtrBegin;

		auto dataPtrBegin = begin();
		auto result = dataPtrBegin + idx;
		if (it != keyPtrEnd && Cmp()(key, *it))
			result = end();

		return result;
	}

	void clear()
	{
		auto p = begin();
		auto e = p + m_size;

		auto keyPtr = (key_type*)m_keyBlock.ptr;

		while (p != e)
		{
			keyPtr->~key_type();
			++keyPtr;

			p->~value_type();
			++p;
		}

		m_size = 0;
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
		return (pointer)m_dataBlock.ptr;
	}

	inline pointer end()
	{
		return &begin()[m_size];
	}

	inline const pointer begin() const
	{
		return (pointer)m_dataBlock.ptr;
	}

	inline const pointer end() const
	{
		return &begin()[m_size];
	}

	inline bool empty() const
	{
		return (m_size == 0);
	}

	inline size_t size() const
	{
		return m_size;
	}

	inline size_t capacity() const
	{
		auto last = (pointer)(m_dataBlock.ptr + m_dataBlock.size);

		return last - begin();
	}
};