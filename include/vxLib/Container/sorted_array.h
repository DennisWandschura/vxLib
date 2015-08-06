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

#include <vxLib/Container/iterator.h>
#include <vxLib/Allocator/StackAllocator.h>
#include <vxLib/Container/sorted_vector.h>

namespace vx
{
	template<typename K, typename T, typename Cmp = std::less<K>>
	class sorted_array
	{
	public:
		typedef sorted_array<K, T, Cmp> _MyContainer;
		typedef T value_type;
		typedef K key_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef const pointer const_pointer;
		typedef StackAllocator MyAllocator;

		typedef vx::vector_const_iterator<_MyContainer> const_iterator;
		typedef vx::vector_iterator<_MyContainer> iterator;

		typedef u32 size_type;
		typedef s64 difference_type;

	private:
		key_type* m_pKeys{nullptr};
		pointer m_pValues{nullptr};
		size_type m_size{ 0 };
		size_type m_capacity{ 0 };
		MyAllocator* m_pAllocator{ nullptr };

		template<class... _Valty>
		void emplace_back(key_type key, _Valty&& ...args)
		{
			MyAllocator::construct(m_pKeys + m_size, key);
			MyAllocator::construct(m_pValues + m_size, std::forward<_Valty>(args)...);

			++m_size;
		}

	public:
		sorted_array() = default;

		sorted_array(size_type capacity, MyAllocator* pAllocator)
			:m_capacity(capacity),
			m_pAllocator(pAllocator)
		{
			m_pKeys = (key_type*)m_pAllocator->allocate(sizeof(key_type) * capacity, __alignof(key_type));
			assert(m_pKeys);
			m_pValues = (pointer)m_pAllocator->allocate(sizeof(value_type) * capacity, __alignof(value_type));
			assert(m_pValues);
		}

		sorted_array(const sorted_array&) = delete;

		sorted_array(sorted_array &&rhs)
			:m_pKeys(rhs.m_pKeys),
			m_pValues(rhs.m_pValues),
			m_size(rhs.m_size),
			m_capacity(rhs.m_capacity),
			m_pAllocator(rhs.m_pAllocator)
		{
			rhs.m_pKeys = nullptr;
			rhs.m_pValues = nullptr;
			rhs.m_size = 0;
			rhs.m_capacity = 0;
			rhs.m_pAllocator = nullptr;
		}

		~sorted_array()
		{
			cleanup();
		}

		sorted_array& operator=(const sorted_array&) = delete;
		sorted_array& operator=(sorted_array &&rhs)
		{
			if (this != &rhs)
			{
				std::swap(m_pKeys, rhs.m_pKeys);
				std::swap(m_pValues, rhs.m_pValues);
				std::swap(m_size, rhs.m_size);
				std::swap(m_capacity, rhs.m_capacity);
				std::swap(m_pAllocator, rhs.m_pAllocator);
			}
			return *this;
		}

		// destroys all values and sets everything to zero
		void cleanup()
		{
			clear();

			if (m_pValues && m_pAllocator)
			{
				m_size = 0;
				m_capacity = 0;
				m_pValues = nullptr;

				// this does nothing really
				m_pAllocator->deallocate((u8*)m_pValues);
				m_pAllocator = nullptr;
			}
		}

		iterator insert(const key_type &key, const value_type &value)
		{
			if (m_capacity <= m_size)
				return end();

			auto endKeys = m_pKeys + m_size;

			auto it = std::lower_bound(m_pKeys, endKeys, key, Cmp());

			auto index = it - m_pKeys;
			if (it == endKeys || Cmp()(key, *it))
			{
				size_type _Off = static_cast<size_type>(it - m_pKeys);

				emplace_back(key, value);

				std::rotate(begin() + _Off, end() - 1, end());

				endKeys = m_pKeys + m_size;
				std::rotate(m_pKeys + _Off, endKeys - 1, endKeys);
			}

			return iterator(m_pValues + index, this);
		}

		iterator insert(key_type &&key, value_type &&value)
		{
			if (m_capacity <= m_size)
				return end();

			auto curEnd = m_pKeys + m_size;
			auto it = std::lower_bound(m_pKeys, curEnd, key, Cmp());

			auto index = it - m_pKeys;
			if (it == curEnd || Cmp()(key, *it))
			{
				size_type _Off = static_cast<size_type>(it - m_pKeys);

				emplace_back(std::forward<key_type>(key), std::forward<value_type>(value));

				std::rotate(begin() + _Off, end() - 1, end());

				auto endKeys = m_pKeys + m_size;
				std::rotate(m_pKeys + _Off, endKeys - 1, endKeys);
			}

			return iterator(m_pValues + index, this);
		}

		iterator erase(const_iterator pos)
		{
			auto p = pos.m_pObject;
			assert(m_pValues <= p && p < m_pValues + size());
			MyAllocator::destroy(p);

			std::move(p + 1, m_pValues + size(), p);

			--m_size;
			return iterator(p, this);
		}

		iterator find(const key_type &key) noexcept
		{
			auto keyEnd = m_pKeys + m_size;
			auto it = std::lower_bound(m_pKeys, keyEnd, key, Cmp());
			auto index = it - m_pKeys;

			auto result = iterator(m_pValues + index, this);
			if (it != keyEnd && Cmp()(key, *it))
				result = end();

			return result;
		}

			const_iterator find(const key_type &key) const noexcept
		{
			auto keyEnd = m_pKeys + m_size;
			auto it = std::lower_bound(m_pKeys, keyEnd, key, Cmp());
			auto index = it - m_pKeys;

			auto result = const_iterator(m_pValues + index, this);
			if (it != keyEnd && Cmp()(key, *it))
				result = end();

			return result;
		}

		void clear()
		{
			if (m_size != 0)
			{
				MyAllocator::rangeDestroy(m_pKeys, m_pKeys + m_size);
				MyAllocator::rangeDestroy(m_pValues, m_pValues + m_size);
				m_size = 0;
			}
		}

		pointer data()
		{
			return m_pValues;
		}

		const_pointer data() const
		{
			return m_pValues;
		}

		key_type keys()
		{
			return m_pKeys;
		}

		const key_type keys() const
		{
			return m_pKeys;
		}

		reference front()
		{
			return *m_pValues;
		}

		const_reference front() const
		{
			return *m_pValues;
		}

		reference back()
		{
			return *(m_pValues + m_size - 1);
		}

		const_reference back() const
		{
			return *(m_pValues + m_size - 1);
		}

		const_iterator begin() const
		{
			return const_iterator(m_pValues, this);
		}

		iterator begin()
		{
			return iterator(m_pValues, this);
		}

		const_iterator end() const
		{
			return const_iterator(m_pValues + m_size, this);
		}

		iterator end()
		{
			return iterator(m_pValues + m_size, this);
		}

		size_type size() const
		{
			return m_size;
		}

		size_type capacity() const
		{
			return m_capacity;
		}
	};
}