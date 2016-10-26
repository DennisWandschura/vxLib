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
#include <vxLib/ArrayAnalyzer.h>
#include <vxLib/TypeInfo.h>

namespace vx
{
	template<typename T, size_t SIZE>
	class Array : public ArrayBase<T>
	{
		VX_TYPE_INFO;

		typedef ArrayBase<T> MyBase;

		enum { BUFFERSIZE = sizeof(T) * SIZE };

	public:
		typedef typename MyBase::value_type value_type;
		typedef typename MyBase::pointer pointer;
		typedef typename MyBase::const_pointer const_pointer;
		typedef typename MyBase::reference reference;
		typedef typename MyBase::const_reference const_reference;

	private:
		u8 m_buffer[BUFFERSIZE];

	public:
		Array() 
			:MyBase(reinterpret_cast<pointer>(m_buffer), reinterpret_cast<pointer>(m_buffer + BUFFERSIZE)),
			m_buffer()
		{
		}

		Array(const Array &rhs) 
			:MyBase(m_buffer, m_buffer + BUFFERSIZE),
			m_buffer()
		{
			auto first = rhs.begin();
			auto end = rhs.end();
			while (first != end)
			{
				new (m_end++) value_type{*(first++)};
			}
		}

		Array(Array &&rhs) 
			:MyBase(m_buffer, m_buffer + BUFFERSIZE),
			m_buffer()
		{
			::memcpy(m_buffer, rhs.m_buffer, BUFFERSIZE);
			::memset(rhs.m_buffer, 0xd, BUFFERSIZE);
			rhs.m_end = rhs.m_begin;
			/*auto first = rhs.begin();
			auto end = rhs.end();
			while (first != end)
			{
				new (m_end++) value_type{ std::move(*(first++)) };
			}

			rhs.m_end = reinterpret_cast<pointer>(rhs.m_begin);*/
		}

		~Array() { clear(); }

		Array& operator=(const Array &rhs)
		{
			if (this != &rhs)
			{
				clear();

				auto first = rhs.begin();
				auto end = rhs.end();
				while (first != end)
				{
					new (m_end++) value_type{ *(first++) };
				}
			}
			return *this;
		}

		Array& operator=(Array &&rhs)
		{
			if (this != &rhs)
			{
				swap(rhs);
			}
			return *this;
		}

		void swap(Array &rhs)
		{
			{
				u8 tmpBuffer[BUFFERSIZE];
				::memcpy(tmpBuffer, m_begin, BUFFERSIZE);

				::memcpy(m_begin, rhs.m_begin, BUFFERSIZE);
				::memcpy(rhs.m_begin, tmpBuffer, BUFFERSIZE);
			}

			m_end = reinterpret_cast<pointer>(m_begin) + rhs.size();
			rhs.m_end = reinterpret_cast<pointer>(rhs.m_begin) + size();
		}

		constexpr size_t capacity() const { return SIZE; }
	};
}

namespace vx
{
	namespace detail
	{
		template<typename T, size_t SIZE>
		struct GetTypeInfo<vx::Array<T, SIZE>>
		{
			static const auto& get()
			{
				static const auto typeInfo
				{
					get_constexpr()
				};
				return typeInfo;
			}

			constexpr static auto get_constexpr()
			{
				return getTypeInfo(concat("vx::Array<", GetTypeInfo<T>::get_constexpr().m_name, ", ", IntToString<SIZE>::get().data, ">"),
					sizeof(vx::Array<T, SIZE>),
					__alignof(vx::Array<T, SIZE>));
			}
		};
	}
}