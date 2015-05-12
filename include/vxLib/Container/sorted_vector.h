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
#ifndef __VX_SORTED_VECTOR_H
#define __VX_SORTED_VECTOR_H
#pragma once

#include <vxLib/types.h>
#include <vxLib/Container/iterator.h>
#include <algorithm>
#include <vxLib/Allocator/Allocator.h>

namespace vx
{
	namespace detail
	{
		template<bool>
		struct Deleter
		{
			template<typename U>
			static void destroy(U *p)
			{
				p->~U();
			}
		};

		template<>
		struct Deleter < false >
		{
			template<typename U>
			static void destroy(U *p)
			{
			}
		};

		template<typename T>
		struct ConstructorArray;

		template<>
		struct ConstructorArray<char>
		{
			template<u32 S>
			static void construct(char(*p)[S], u32 i, const char(&src)[S])
			{
				strcpy_s(p[i], src);
			}
		};

		template<bool>
		struct ConstructImpl
		{
			template<typename U, u32 S>
			static void construct(U(*p)[S], u32 i, const U(&src)[S])
			{
				ConstructorArray<U>::construct(p,i, src);
			}

			template<class U, u32 S>
			static void moveDestroy(U *pDest, U *pSrc, u32 size);

			template<u32 S>
			static void moveDestroy(char(*pDest)[S], char(*pSrc)[S], u32 size)
			{
				for (u32 i = 0; i < size; ++i)
				{
					strcpy_s(pDest[i], pSrc[i]);
					//Deleter<std::is_destructible<char>::value>::destroy<char>(pSrc + i);
				}
			}
		};

		template<>
		struct ConstructImpl<false>
		{
			template<typename U, class... _Valty>
			static void construct(U* m_pValues, u32 i, _Valty&& ...args)
			{
				new(m_pValues + i) U(std::forward<_Valty>(args)...);
			}

			template<typename U>
			static void moveDestroy(U *pDest, U *pSrc, u32 size)
			{
				for (u32 i = 0; i < size; ++i)
				{
					new (pDest + i) U(std::move(*(pSrc + i)));
					Deleter<std::is_destructible<U>::value>::destroy(pSrc + i);
				}
			}
		};
	}

	template<typename Key, typename Type, typename Cmp = std::less<Key>>
	class sorted_vector
	{
	public:
		typedef Key key_type;
		typedef Type value_type;
		typedef value_type& reference;
		typedef const reference const_reference;
		typedef value_type* pointer;
		typedef  const pointer const_pointer;

		typedef u32 size_type;
		typedef s64 difference_type;

		typedef sorted_vector < Key, Type, Cmp > _MyCon;
		typedef vx::vector_const_iterator < _MyCon > const_iterator;
		typedef vx::vector_iterator < _MyCon > iterator;

	private:
		enum
		{ 
			IsValueTypeArray = std::is_array<value_type>::value,
			IsKeyTypeArray = std::is_array<key_type>::value
		};

		key_type *m_pKeys;
		value_type *m_pValues;
		size_type m_size;
		size_type m_capacity;

		template<typename U, class... _Valty>
		void emplace_back(U &&key, _Valty&& ...args)
		{
			if (m_size >= m_capacity)
				reserve((m_capacity + 1) * 1.3f);

			new(m_pKeys + m_size) key_type(std::forward<U>(key));
			//new(m_pValues + m_size) value_type(std::forward<_Valty>(args)...);
			detail::ConstructImpl<IsValueTypeArray>::construct(m_pValues, m_size, std::forward<_Valty>(args)...);

			++m_size;
		}

		template<typename U>
		void moveDestroy(U *pDest, U *pSrc, u32 size)
		{
			detail::ConstructImpl<IsValueTypeArray>::moveDestroy(pDest, pSrc, size);
		}

		template<class U>
		void rangeDestroy(U *ptr, u32 size)
		{
			for (auto i = 0u; i < size; ++i)
			{
				detail::Deleter<std::is_destructible<U>::value>::destroy(ptr + i);
			}
		}

	public:
		sorted_vector() noexcept
			: m_pKeys(nullptr), m_pValues(nullptr), m_size(0), m_capacity(0){}

		sorted_vector(sorted_vector &&rhs)
			: m_pKeys(rhs.m_pKeys), m_pValues(rhs.m_pValues), m_size(rhs.m_size), m_capacity(rhs.m_capacity)
		{
			rhs.m_pKeys = nullptr;
			rhs.m_pValues = nullptr;
			rhs.m_size = 0;
			rhs.m_capacity = 0;
		}

		~sorted_vector()
		{
			rangeDestroy(m_pKeys, m_size);
			rangeDestroy(m_pValues, m_size);

			::operator delete(m_pKeys);
			m_pKeys = nullptr;
			m_pValues = nullptr;
			m_size = 0;
			m_capacity = 0;
		}

		sorted_vector& operator=(sorted_vector &&rhs)
		{
			if (this != &rhs)
			{
				m_pKeys = rhs.m_pKeys;
				m_pValues = rhs.m_pValues;
				m_size = rhs.m_size;
				m_capacity = rhs.m_capacity;

				rhs.m_pKeys = nullptr;
				rhs.m_pValues = nullptr;
				rhs.m_size = 0;
				rhs.m_capacity = 0;
			}
			return *this;
		}

		void reserve(u32 n)
		{
			if (n > m_capacity)
			{
				u8 *pNewMemory = (u8*)::operator new((sizeof(key_type) + sizeof(value_type)) * n + __alignof(value_type));

				key_type *pNewKeys = (key_type*)pNewMemory;
				detail::ConstructImpl<IsKeyTypeArray>::moveDestroy(pNewKeys, m_pKeys, m_size);

				void *oldData = m_pKeys;

				auto tmp = (u8*)(pNewMemory + (sizeof(key_type) * n));
				auto adjustment = Allocator::getAdjustment(tmp, __alignof(value_type));
				tmp += adjustment;

				pointer pNewValues = (pointer)tmp;
				detail::ConstructImpl<IsValueTypeArray>::moveDestroy(pNewValues, m_pValues, m_size);

				::operator delete(oldData);

				m_pValues = pNewValues;
				m_pKeys = pNewKeys;

				m_capacity = n;
			}
		}

		iterator insert(const key_type &key, const value_type &value)
		{
			auto endKeys = m_pKeys + m_size;

			auto it = std::lower_bound(m_pKeys, endKeys, key, Cmp());

			auto index = it - m_pKeys;
			if (it == endKeys || Cmp()(key, *it))
			{
				size_type _Off = it - m_pKeys;

				emplace_back(key, value);

				std::rotate(begin() + _Off, end() - 1, end());

				endKeys = m_pKeys + m_size;
				std::rotate(m_pKeys + _Off, endKeys - 1, endKeys);
			}

			return iterator(m_pValues + index, this);
		}

		iterator insert(key_type &&key, value_type &&value)
		{
			auto curEnd = m_pKeys + m_size;
			auto it = std::lower_bound(m_pKeys, curEnd, key, Cmp());

			auto index = it - m_pKeys;
			if (it == curEnd || Cmp()(key, *it))
			{
				size_type _Off = it - m_pKeys;

				emplace_back(std::forward<key_type>(key), std::forward<value_type>(value));

				std::rotate(begin() + _Off, end() - 1, end());

				auto endKeys = m_pKeys + m_size;
				std::rotate(m_pKeys + _Off, endKeys - 1, endKeys);
			}

			return iterator(m_pValues + index, this);
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

		iterator erase(const_iterator pos)
		{
			auto p = pos.m_pObject;
			auto index = p - m_pValues;
			auto pKey = m_pKeys + index;

			detail::Deleter<std::is_destructible<value_type>::value>::destroy(p);
			detail::Deleter<std::is_destructible<key_type>::value>::destroy(pKey);

			std::move(p + 1, m_pValues + size(), p);
			std::move(pKey + 1, m_pKeys + size(), pKey);

			--m_size;
			return iterator(p, this);
		}

		void clear()
		{
			rangeDestroy(m_pKeys, m_size);
			rangeDestroy(m_pValues, m_size);
			m_size = 0;
		}

			reference operator[](size_type i)
		{
			return m_pValues[i];
		}

		const_reference operator[](size_type i) const
		{
			return m_pValues[i];
		}

		const key_type* keys() const { return m_pKeys; } const

		const_iterator begin() const noexcept
		{
			return const_iterator(m_pValues, this);
		}

			iterator begin() noexcept
		{
			return iterator(m_pValues, this);
		}

			const_iterator end() const noexcept
		{
			return const_iterator(m_pValues + m_size, this);
		}

			const_pointer data() const noexcept
		{
			return m_pValues;
		}

			iterator end() noexcept
		{
			return iterator(m_pValues + m_size, this);
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
	};
}
#endif