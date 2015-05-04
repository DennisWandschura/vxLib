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
#include <vxLib/Container\iterator.h>
#include <algorithm>
#include <vxLib/AlignedStorage.h>
#include <vxLib/Allocator/Allocator.h>

namespace vx
{
	template<class Key, class Type>
	class sorted_vector
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

		template<bool>
		struct ConstructImpl
		{
			template<typename U, U32 S>
			static void construct(U(*p)[S], U32 i, const U(&src)[S]);

			template<U32 S>
			static void construct(char(*p)[S], U32 i, const char(&src)[S])
			{
				strcpy_s(p[i], src);
			}

			template<class U, U32 S>
			static void moveDestroy(U *pDest, U *pSrc, U32 size);

			template<U32 S>
			static void moveDestroy(char (*pDest)[S], char (*pSrc)[S], U32 size)
			{
				for (U32 i = 0; i < size; ++i)
				{
					strcpy_s(pDest[i], pSrc[i]);
					Deleter<std::is_destructible<char[S]>::value>::destroy<char[S]>(pSrc + i);
				}
			}
		};

		template<>
		struct ConstructImpl<false>
		{
			template<typename U, class... _Valty>
			static void construct(U* m_pValues, U32 i, _Valty&& ...args)
			{
				new(m_pValues + i) U(std::forward<_Valty>(args)...);
			}

			template<class U>
			static void moveDestroy(U *pDest, U *pSrc, U32 size)
			{
				for (U32 i = 0; i < size; ++i)
				{
					new (pDest + i) U(std::move(*(pSrc + i)));
					Deleter<std::is_destructible<U>::value>::destroy<U>(pSrc + i);
				}
			}
		};

	public:
		using key_type = Key;
		using value_type = Type;
		using reference = value_type&;
		using const_reference = const reference;
		using pointer = value_type*;
		using const_pointer = const pointer;
		using size_type = U32;
		using difference_type = size_t;

		using const_iterator = vx::vector_const_iterator < sorted_vector<Key, Type> > ;
		using iterator = vx::vector_iterator < sorted_vector<Key, Type> > ;

	private:
		enum{ IsArray = std::is_array<value_type>::value };

		key_type *m_pKeys;
		value_type *m_pValues;
		size_type m_size;
		size_type m_capacity;

		template<class... _Valty>
		void emplace_back(key_type key, _Valty&& ...args)
		{
			if (m_size >= m_capacity)
				reserve((m_capacity + 1) * 1.3f);

			new(m_pKeys + m_size) key_type(key);
			ConstructImpl<IsArray>::construct(m_pValues, m_size, std::forward<_Valty>(args)...);

			++m_size;
		}

		void moveDestroy(value_type *pDest, value_type *pSrc, U32 size)
		{
			ConstructImpl<IsArray>::moveDestroy(pDest, pSrc, size);
		}

		void moveDestroy(key_type *pDest, key_type *pSrc, U32 size)
		{
			for (U32 i = 0; i < size; ++i)
			{
				new (pDest + i) key_type(std::move(*(pSrc + i)));
				Deleter<std::is_destructible<key_type>::value>::destroy<key_type>(pSrc + i);
			}
		}

		template<class U>
		void rangeDestroy(U *ptr, U32 size)
		{
			for (auto i = 0u; i < size; ++i)
			{
				Deleter<std::is_destructible<U>::value>::destroy<U>(ptr + i);
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

		void reserve(U32 n)
		{
			if (n > m_capacity)
			{
				U8 *pNewMemory = (U8*)::operator new((sizeof(key_type) + sizeof(value_type)) * n + __alignof(value_type));

				key_type *pNewKeys = (key_type*)pNewMemory;
				moveDestroy(pNewKeys, m_pKeys, m_size);
				//std::copy(m_pKeys, m_pKeys + m_size, pNewKeys);

				void *oldData = m_pKeys;

				auto tmp = (U8*)(pNewMemory + (sizeof(key_type) * n));
				auto adjustment = AllocatorBase::getAdjustment(tmp, __alignof(value_type));
				tmp += adjustment;

				pointer pNewValues = (pointer)tmp;
				moveDestroy(pNewValues, m_pValues, m_size);

				::operator delete(oldData);

				m_pValues = pNewValues;
				m_pKeys = pNewKeys;

				m_capacity = n;
			}
		}

		iterator insert(key_type key, const value_type &value)
		{
			auto endKeys = m_pKeys + m_size;

			auto it = std::lower_bound(m_pKeys, endKeys, key);

			auto index = it - m_pKeys;
			if (it == endKeys || (key < *it))
			{
				size_type _Off = it - m_pKeys;

				emplace_back(key, value);
				//emplace_back(std::forward<value_type>(value);

				std::rotate(begin() + _Off, end() - 1, end());

				endKeys = m_pKeys + m_size;
				std::rotate(m_pKeys + _Off, endKeys - 1, endKeys);
			}

			return iterator(m_pValues + index, this);
		}

		iterator insert(key_type key, value_type &&value)
		{
			auto curEnd = m_pKeys + m_size;
			auto it = std::lower_bound(m_pKeys, curEnd, key);

			auto index = it - m_pKeys;
			if (it == curEnd || (key < *it))
			{
				size_type _Off = it - m_pKeys;

				emplace_back(key, std::forward<value_type>(value));
				//emplace_back(std::forward<value_type>(value);

				std::rotate(begin() + _Off, end() - 1, end());

				auto endKeys = m_pKeys + m_size;
				std::rotate(m_pKeys + _Off, endKeys - 1, endKeys);
			}

			return iterator(m_pValues + index, this);
		}

		iterator find(key_type key) noexcept
		{
			auto keyEnd = m_pKeys + m_size;
			auto it = std::lower_bound(m_pKeys, keyEnd, key);
			auto index = it - m_pKeys;

			auto result = iterator(m_pValues + index, this);
			if (it != keyEnd && (key < *it))
				result = end();

			return result;
		}

			const_iterator find(key_type key) const noexcept
		{
			auto keyEnd = m_pKeys + m_size;
			auto it = std::lower_bound(m_pKeys, keyEnd, key);
			auto index = it - m_pKeys;

			auto result = const_iterator(m_pValues + index, this);
			if (it != keyEnd && (key < *it))
				result = end();

			return result;
		}

		iterator erase(const_iterator pos)
		{
			auto p = pos.m_pObject;
			auto index = p - m_pValues;
			auto pKey = m_pKeys + index;

			Deleter<std::is_destructible<value_type>::value>::destroy(p);
			Deleter<std::is_destructible<key_type>::value>::destroy(pKey);

			std::move(p + 1, m_pValues + size(), p);
			std::move(pKey + 1, m_pKeys + size(), pKey);

			--m_size;
			return iterator(p, this);
		}

		void clear()
		{
			//m_pKeys = m_pKeys;
			rangeDestroy(m_pKeys, m_size);
			rangeDestroy(m_pValues, m_size);
			//m_pValues;
			m_size = 0;
			//m_capacity;
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