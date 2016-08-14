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

#include <vxLib/Allocator/Mallocator.h>
#include <algorithm>
#include <cstring>
#include <cctype>

namespace vx
{
	template<typename T, typename Allocator>
	class basic_string
	{
		typedef Allocator MyAllocator;
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;

		enum { BUFFER_SIZE_BYTES = 16, BUFFER_LENGTH = BUFFER_SIZE_BYTES / sizeof(value_type) };

		union
		{
			vx::AllocatedBlock m_data;
			value_type m_buffer[BUFFER_LENGTH];
		};
		u32 m_size;
		u32 m_capacity;
		MyAllocator m_allocator;

		u64 strlen(const char* str)
		{
			return ::strlen(str);
		}

		u64 strlen(const wchar_t* str)
		{
			return ::wcslen(str);
		}

		pointer getPtr()
		{
			return (m_capacity == BUFFER_LENGTH) ? m_buffer : reinterpret_cast<pointer>(m_data.ptr);
		}

		const_pointer getPtr() const
		{
			return (m_capacity == BUFFER_LENGTH) ? m_buffer : reinterpret_cast<const_pointer>(m_data.ptr);
		}

		void release()
		{
			if (m_capacity > BUFFER_LENGTH)
			{
				m_allocator.deallocate(m_data);
				m_data = { nullptr, 0 };

				m_capacity = 0;
				m_size = 0;
			}
		}

		void swapPtr(basic_string &rhs)
		{
			if (rhs.m_capacity == BUFFER_LENGTH &&
				m_capacity == BUFFER_LENGTH)
			{
				value_type buffer[BUFFER_LENGTH];

				::memcpy(buffer, rhs.m_buffer, BUFFER_SIZE_BYTES);
				::memcpy(rhs.m_buffer, m_buffer, BUFFER_SIZE_BYTES);
				::memcpy(m_buffer, buffer, BUFFER_SIZE_BYTES);
			}
			else
			{
				std::swap(m_data, rhs.m_data);
			}
		}

		void copyConstructData(const basic_string &rhs)
		{
			if (rhs.m_capacity == BUFFER_LENGTH)
			{
				::memcpy(m_buffer, rhs.m_buffer, BUFFER_SIZE_BYTES);
			}
			else
			{
				auto strSize = rhs.m_size;
				auto newCapacity = (rhs.m_size + 1);
				auto allocSizeBytes = newCapacity * sizeof(value_type);

				m_allocator.deallocate(m_data);
				auto allocatedBlock = m_allocator.allocate(allocSizeBytes, 4);

				::memcpy(allocatedBlock.ptr, rhs.m_data.ptr, allocSizeBytes);

				m_data = allocatedBlock;
				m_size = strSize;
				m_capacity = static_cast<u32>(allocatedBlock.size / sizeof(value_type));
			}
		}

		void moveConstructData(basic_string &rhs)
		{
			if (rhs.m_capacity == BUFFER_LENGTH)
			{
				::memcpy(m_buffer, rhs.m_buffer, BUFFER_SIZE_BYTES);
			}
			else
			{
				std::swap(m_data, rhs.m_data);
				rhs.m_data = {nullptr, 0};
			}
		}

	public:
		basic_string() :m_data(), m_size(0), m_capacity(BUFFER_LENGTH), m_allocator() {}
		explicit basic_string(MyAllocator &&alloc) :m_data(), m_size(0), m_capacity(BUFFER_LENGTH), m_allocator(std::move(alloc)) {}

		basic_string(const_pointer str)
			:m_data(),
			m_size(0),
			m_capacity(BUFFER_LENGTH)
		{
			assign(str);
		}

		basic_string(const basic_string &rhs)
			:m_data(),
			m_size(rhs.m_size),
			m_capacity(BUFFER_LENGTH)
		{
			copyConstructData(rhs);
		}

		basic_string(basic_string &&rhs)
			:m_data(),
			m_size(rhs.m_size),
			m_capacity(rhs.m_capacity)
		{
			moveConstructData(rhs);
			rhs.m_size = 0;
			rhs.m_capacity = BUFFER_LENGTH;
		}

		~basic_string()
		{
			release();
		}

		basic_string& operator=(const_pointer  str)
		{
			assign(str);
			return *this;
		}

		basic_string& operator=(const basic_string &rhs)
		{
			if (this != &rhs)
			{
				assign(rhs.c_str(), rhs.size());
			}
			return *this;
		}

		basic_string& operator=(basic_string &&rhs)
		{
			if (this != &rhs)
			{
				swapPtr(rhs);
				std::swap(m_size, rhs.m_size);
				std::swap(m_capacity, rhs.m_capacity);
			}
			return *this;
		}

		void reserve(u32 cap)
		{
			if (cap <= m_capacity)
				return;

			auto currentSize = m_size;
			auto allocSizeBytes = cap * sizeof(value_type);
			auto newData = m_allocator.allocate(allocSizeBytes, 4);

			::memcpy(newData.ptr, getPtr(), (currentSize + 1) * sizeof(value_type));
			newData.ptr[currentSize] = 0;

			if (BUFFER_LENGTH< m_capacity)
			{
				m_allocator.deallocate(m_data);
			}

			m_data = newData;
			m_capacity = static_cast<u32>(newData.size / sizeof(value_type));
		}

		void assign(const_pointer str)
		{
			assign(str, (u32)strlen(str));
		}

		void assign(const_pointer str, u32 size)
		{
			auto strSize = size + 1;
			if (strSize > m_capacity)
			{
				reserve(strSize);
			}

			auto dst = getPtr();
			::memcpy(dst, str, size * sizeof(value_type));
			dst[size] = '\0';
			m_size = size;
		}

		void push_back(value_type c)
		{
			auto newSize = m_size + 2;
			if (newSize > m_capacity)
			{
				reserve((u32)(newSize * 1.5f) + 1);
			}

			auto dst = getPtr();
			dst[m_size++] = c;
			dst[m_size] = 0;
		}

		void append(const basic_string &str)
		{
			return append(str.c_str(), static_cast<u64>(str.size()));
		}

		void append(const_pointer str)
		{
			return append(str, strlen(str));
		}

		void append(const_pointer str, u64 size)
		{
			auto currentSize = m_size;
			auto newSize = currentSize + size;
			auto reqSize = newSize + 1;
			if (reqSize > m_capacity)
			{
				reserve(static_cast<u32>(reqSize * 1.5f) + 1);
			}

			auto copySize = size * sizeof(value_type);
			auto dst = getPtr();
			::memcpy(&dst[currentSize], str, copySize);
			dst[newSize] = 0;

			m_size = static_cast<u32>(newSize);
		}

		pointer begin() { return getPtr(); }
		const_pointer begin() const { return getPtr(); }

		pointer end() { return getPtr() + m_size; }
		const_pointer end() const { return getPtr() + m_size; }

		u32 capacity() const { return m_capacity; }
		u32 size() const { return m_size; }

		pointer c_str() { return getPtr(); }
		const_pointer c_str() const { return getPtr(); }

		void strip(value_type c)
		{
			auto ptr = getPtr();
			auto sz = size();
			for (u32 i = 0; i < sz; ++i)
			{
				if (ptr[i] == c)
				{
					for (u32 k = i + 1, j = i; k < sz; ++k, ++j)
					{
						ptr[j] = ptr[k];
					}

					--sz;
				}
			}

			ptr[sz] = 0;
			m_size = sz;
		}

		void toLowerCase()
		{
			std::transform(begin(), end(), begin(), std::tolower);
		}
	};

	typedef basic_string<char, Mallocator> string;
	typedef basic_string<wchar_t, Mallocator> wstring;

	template<typename T, typename Allocator>
	inline basic_string<T, Allocator> operator+(const basic_string<T, Allocator> &lhs, const basic_string<T, Allocator> &rhs)
	{
		basic_string<T, Allocator> result(lhs);
		result.append(rhs);

		return result;
	}

	template<typename T, typename Allocator>
	inline basic_string<T, Allocator> operator+(basic_string<T, Allocator> &&lhs, const basic_string<T, Allocator> &rhs)
	{
		basic_string<T, Allocator> result(std::move(lhs));
		result.append(rhs);

		return result;
	}

	template<typename T, typename Allocator>
	inline basic_string<T, Allocator> operator+(const basic_string<T, Allocator> &lhs, const char* rhs)
	{
		basic_string<T, Allocator> result(lhs);
		result.append(rhs);

		return result;
	}

	template<typename T, typename Allocator>
	inline basic_string<T, Allocator> operator+(basic_string<T, Allocator> &&lhs, const char* rhs)
	{
		basic_string<T, Allocator> result(std::move(lhs));
		result.append(rhs);

		return result;
	}
}