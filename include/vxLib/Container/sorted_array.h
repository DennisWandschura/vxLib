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
#include <vxLib/algorithm.h>

namespace vx
{
	template<typename K, typename T, typename Allocator, typename Cmp = std::less<>>
	class sorted_array
	{
		typedef K key_type;
		typedef T value_type;
		typedef value_type* pointer;

		AllocatedBlock m_keyBlock;
		AllocatedBlock m_dataBlock;
		size_t m_size;
		size_t m_capacity;
		Allocator m_allocator;

		key_type* beginKey()
		{
			return ((key_type*)m_keyBlock.ptr);
		}

		key_type* endKey()
		{
			return beginKey() + m_size;
		}

		const key_type* beginKey() const
		{
			return ((key_type*)m_keyBlock.ptr);
		}

		const key_type* endKey() const
		{
			return beginKey() + m_size;
		}

	public:
		sorted_array() : m_keyBlock(), m_dataBlock(), m_size(0), m_capacity(0), m_allocator() {}

		sorted_array(Allocator &&alloc, size_t capacity)
			: m_keyBlock(), m_dataBlock(), m_size(0), m_capacity(0), m_allocator(std::move(alloc))
		{
			m_keyBlock = m_allocator.allocate(sizeof(key_type) * capacity, __alignof(key_type));
			m_dataBlock = m_allocator.allocate(sizeof(value_type) * capacity, __alignof(value_type));

			if (m_keyBlock.ptr != nullptr && m_dataBlock.ptr != nullptr)
			{
				m_capacity = capacity;
			}
		}

		template<typename T>
		sorted_array(T* alloc, size_t capacity) : sorted_array(std::move(Allocator(alloc)), capacity) {}

		sorted_array(const sorted_array&) = delete;

		sorted_array(sorted_array &&other)
			: m_keyBlock(other.m_keyBlock),
			m_dataBlock(other.m_dataBlock),
			m_size(other.m_size),
			m_capacity(other.m_capacity),
			m_allocator(std::move(other.m_allocator))
		{
			other.m_keyBlock = { nullptr, 0 };
			other.m_dataBlock = { nullptr, 0 };
			other.m_size = 0;
			other.m_capacity = 0;
		}

		~sorted_array()
		{
			release();
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
			std::swap(m_capacity, other.m_capacity);
			m_allocator.swap(other.m_allocator);
		}

		template<typename ...Args>
		pointer insert(const key_type &key, Args&&... args)
		{
			auto currentSize = m_size;
			if (currentSize >= m_capacity)
			{
				return end();
			}

			auto keyPtrBegin = beginKey();
			auto keyPtrEnd = endKey();

			auto it = std::lower_bound(keyPtrBegin, keyPtrEnd, key, Cmp());
			auto idx = it - keyPtrBegin;
			auto dataPtrBegin = begin();

			auto dataPtr = dataPtrBegin + idx;

			if (it == keyPtrEnd || Cmp()(key, *it))
			{
				auto dataPtrEnd = dataPtrBegin + currentSize;

				new (keyPtrEnd++) key_type{ key };
				new (dataPtrEnd++) value_type{ std::forward<Args>(args)... };

				std::rotate(it, keyPtrEnd - 1, keyPtrEnd);
				std::rotate(dataPtr, dataPtrEnd - 1, dataPtrEnd);

				++m_size;
			}

			return dataPtr;
		}

		pointer find(const key_type &key)
		{
			auto keyPtrBegin = beginKey();
			auto keyPtrEnd = endKey();

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
			auto keyPtrBegin = beginKey();
			auto keyPtrEnd = endKey();

			auto it = std::lower_bound(keyPtrBegin, keyPtrEnd, key, Cmp());
			auto idx = it - keyPtrBegin;

			auto dataPtrBegin = begin();
			auto result = dataPtrBegin + idx;
			if (it != keyPtrEnd && Cmp()(key, *it))
				result = end();

			return result;
		}

		void erase(const pointer p)
		{
			auto idx = p - begin();

			auto currentValue = &begin()[idx];
			auto currentKey = &beginKey()[idx];

			vx::destruct(currentValue);
			vx::destruct(currentKey);

			vx::move(currentValue + 1, end(), currentValue);
			vx::move(currentKey + 1, endKey(), currentKey);

			--m_size;
		}

		void clear()
		{
			vx::destruct(begin(), end());
			vx::destruct(beginKey(), endKey());

			m_size = 0;
		}

		void release()
		{
			if (m_keyBlock.ptr)
			{
				clear();

				m_allocator.deallocate(m_keyBlock);
				m_allocator.deallocate(m_dataBlock);

				m_keyBlock = { nullptr, 0 };
				m_dataBlock = { nullptr, 0 };
				m_capacity = 0;
			}
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
			return m_capacity;
		}
	};

	template<typename K, typename T, typename Allocator, typename Cmp>
	void swap(sorted_array<K, T, Allocator, Cmp> &l, sorted_array<K, T, Allocator, Cmp> &r)
	{
		l.swap(r);
	}
}