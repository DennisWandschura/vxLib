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

#include <vxLib/Container/string.h>

namespace vx
{
	template<u32 N>
	class string_array
	{
		char m_data[N];
		u32 m_size;

	public:
		string_array() :m_data(), m_size(0) {}

		string_array(const char* str, u32 size)
			:m_data(), m_size(0)
		{
			assign(str, size);
		}

		string_array(const char* str)
			:m_data(), m_size(0)
		{
			assign(str);
		}

		template<size_t SIZE>
		string_array(const char(&str)[SIZE])
			: m_data(), m_size(0)
		{
			assign(str, SIZE);
		}

		string_array& operator=(const char* str)
		{
			assign(str);
			return *this;
		}

		template<size_t SIZE>
		string_array& operator=(const char(&str)[SIZE])
		{
			assign(str, SIZE);
			return *this;
		}

		template<typename Allocator>
		string_array& operator=(const basic_string<char, Allocator> &str)
		{
			assign(str.c_str(), str.size());
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
			size = std::min(size, N - 1);
			memcpy(m_data, str, size);
			m_data[size] = '\0';
			m_size = size;
		}

		const char* c_str() const { return m_data; }
		u32 size() const { return m_size; }
		u32 capacity() const { return N };

		char* begin() { return m_data; }
		const char* begin() const { return m_data; }

		char* end() { return m_data + m_size; }
		const char* end() const { return m_data + m_size; }
	};
}