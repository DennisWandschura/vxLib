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

#include <vxLib/Container/ArrayBase.h>
#include <vxLib/string.h>
#include <algorithm>

namespace vx
{
	template<typename T, typename Alloc>
	class basic_string;

	template<u32 COUNT>
	class StringArray : public ArrayBase<char>
	{
		typedef ArrayBase<char> MyBase;

		char m_data[COUNT];

	public:
		constexpr StringArray() :ArrayBase(m_data, &m_data[COUNT]),m_data() {}

		StringArray(const char* str, u32 size)
			:ArrayBase(m_data, m_data + COUNT), m_data()
		{
			assign(str, size);
		}

		StringArray(const char* str)
			:ArrayBase(m_data, m_data + COUNT), m_data()
		{
			assign(str);
		}

		template<size_t SIZE>
		StringArray(const char(&str)[SIZE])
			: ArrayBase(m_data, m_data + COUNT), m_data()
		{
			assign(str, SIZE);
		}


		explicit StringArray(size_t value)
			: ArrayBase(m_data, m_data + COUNT), m_data()
		{
			u32 size = 0;
			int_to_string(value, m_data, COUNT, &size);
			m_end += size;
		}

		StringArray(const StringArray &rhs)
			:ArrayBase(m_data, m_data + rhs.size(), m_data + COUNT), m_data()
		{
			::memcpy(m_data, rhs.m_data, COUNT);
		}

		StringArray(StringArray &&rhs)
			:ArrayBase(m_data, m_data + rhs.size(), m_data + COUNT), m_data()
		{
			::memcpy(m_data, rhs.m_data, COUNT);

			::memset(rhs.m_data, 0, COUNT);
			rhs.m_end = rhs.m_data;
		}

		StringArray& operator=(const char* str)
		{
			assign(str);
			return *this;
		}

		template<size_t SIZE>
		StringArray& operator=(const char(&str)[SIZE])
		{
			assign(str, SIZE);
			return *this;
		}

		template<typename Allocator>
		StringArray& operator=(const basic_string<char, Allocator> &str)
		{
			assign(str.c_str(), str.size());
			return *this;
		}

		StringArray& operator=(const StringArray &rhs)
		{
			if (this != &rhs)
			{
				auto otherSize = rhs.size();
				::memcpy(m_data, rhs.m_data, COUNT);

				m_end = m_data + otherSize;
			}
			return *this;
		}

		void assign(const char* str)
		{
			assign(str, static_cast<u32>(strlen(str)));
		}

		template<size_t SIZE>
		void assign(const char(&str)[SIZE])
		{
			assign(str, SIZE);
		}

		void assign(const char* str, u32 size)
		{
			size = std::min(size, COUNT - 1);
			memcpy(m_data, str, size);
			m_data[size] = '\0';
			m_end = m_begin + size;
		}

		void append(char c)
		{
			if (m_end >= m_last)
				return;

			*(m_end++) = c;
		}

		void append(const char* str, u32 size)
		{
			auto newEnd = m_end + size;
			newEnd = (newEnd >= m_last) ? m_last : newEnd;

			::memcpy(m_end, str, newEnd - m_end);
			m_end = newEnd;
		}

		void append(const char* str)
		{
			append(str, static_cast<u32>(strlen(str)));
		}

		template<size_t OTHER>
		void append(const StringArray<OTHER> &other)
		{
			append(other.c_str(), static_cast<u32>(other.size()));
		}

		void clear()
		{
			m_data[0] = 0;
			m_end = m_data;
		}

		const char* c_str() const { return m_data; }

		constexpr u32 capacity() const { return COUNT };
	};

	template<u32 N>
	inline StringArray<N> operator+(const StringArray<N> &lhs, const StringArray<N> &rhs)
	{
		StringArray<N> tmp(lhs);
		tmp.append(rhs);
		return tmp;
	}

	template<u32 N>
	inline StringArray<N> operator+(const StringArray<N> &lhs, const char* rhs)
	{
		StringArray<N> tmp(lhs);
		tmp.append(rhs);
		return tmp;
	}

	template<u32 N>
	inline StringArray<N> operator+(const StringArray<N> &lhs, char rhs)
	{
		StringArray<N> tmp(lhs);
		tmp.append(rhs);
		return tmp;
	}

	template<u32 N>
	inline StringArray<N> operator+(const char* lhs, const StringArray<N> &rhs)
	{
		StringArray<N> tmp(lhs);
		tmp.append(rhs);
		return tmp;
	}
}