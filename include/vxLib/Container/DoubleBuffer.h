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
#ifndef __VX_DOUBLEBUFFER_H
#define __VX_DOUBLEBUFFER_H
#pragma once

#include <vxLib/types.h>

namespace vx
{
	template<typename T>
	class DoubleBuffer
	{
		typedef u32 size_type;
		typedef T value_type;
		typedef value_type& reference;
		typedef const reference const_reference;
		typedef value_type* pointer;

		pointer m_frontBuffer{ nullptr };
		pointer m_backBuffer{ nullptr };
		size_type m_sizeFront{ 0 };
		size_type m_sizeBack{ 0 };
		size_type m_capacity{ 0 };

		void destroyValue(pointer p)
		{
			p->~value_type();
		}

		void clearFrontBuffer()
		{
			for (size_type i = 0; i < m_sizeFront; ++i)
			{
				destroyValue(m_frontBuffer + i);
			}

			m_sizeFront = 0;
		}

		void clearBackBuffer()
		{
			for (size_type i = 0; i < m_sizeBack; ++i)
			{
				destroyValue(m_backBuffer + i);
			}

			m_sizeBack = 0;
		}

	public:
		DoubleBuffer(){}

		DoubleBuffer(pointer p, size_type m_capacity)
			:m_frontBuffer(p),
			m_backBuffer(p + m_capacity),
			m_capacity(m_capacity)
		{
		}

		DoubleBuffer(const DoubleBuffer&) = delete;

		DoubleBuffer(DoubleBuffer &&rhs) noexcept
			:m_frontBuffer(rhs.m_frontBuffer),
			m_backBuffer(rhs.m_backBuffer),
			m_sizeFront(rhs.m_sizeFront),
			m_sizeBack(rhs.m_sizeBack)
			m_capacity(rhs.m_capacity)
		{
			rhs.m_frontBuffer = nullptr;
			rhs.m_backBuffer = nullptr;
			rhs.m_sizeFront = 0;
			rhs.m_sizeBack = 0;
			rhs.m_capacity = 0;
		}

		~DoubleBuffer()
		{
			clearFrontBuffer();
			clearBackBuffer();
			m_frontBuffer = nullptr;
			m_backBuffer = nullptr;
		}

		DoubleBuffer& operator=(const DoubleBuffer&) = delete;

		DoubleBuffer& operator=(DoubleBuffer &&rhs) noexcept
		{
			std::swap(m_frontBuffer, rhs.m_frontBuffer);
			std::swap(m_backBuffer, rhs.m_backBuffer);
			std::swap(m_sizeFront, rhs.m_sizeFront);
			std::swap(m_sizeBack, rhs.m_sizeBack);
			std::swap(m_capacity, rhs.m_capacity);

			return *this;
		}

		bool push(const value_type &value)
		{
			if (m_sizeFront >= m_capacity)
				return false;

			auto p = m_frontBuffer + m_sizeFront;
			new (p) value_type(value);
			++m_sizeFront;

			return true;
		}

		bool push(value_type &&value)
		{
			if (m_sizeFront >= m_capacity)
				return false;

			auto p = m_frontBuffer + m_sizeFront;
			new (p)value_type (std::move(value));
			++m_sizeFront;

			return true;
		}

		void pop(value_type &value)
		{
			auto lastElement = m_frontBuffer[m_sizeFront - 1];
			value = std::move(lastElement);

			destroyValue(&lastElement);
			--m_sizeFront;
		}

		void swapBuffers()
		{
			std::swap(m_frontBuffer, m_backBuffer);
			std::swap(m_sizeFront, m_sizeBack);

			clearFrontBuffer();
		}

		reference getItemFromBackBuffer(size_type i)
		{
			return m_backBuffer[i];
		}

		const_reference getItemFromBackBuffer(size_type i) const
		{
			return m_backBuffer[i];
		}

		size_type sizeFront() const
		{
			return m_sizeFront;
		}

		size_type sizeBack() const
		{
			return this->m_sizeBack;
		}

		size_type capacity() const
		{
			return this->m_capacity;
		}

		bool emptyFront() const
		{
			return m_sizeFront == 0;
		}

		bool emptyBack() const
		{
			return m_sizeBack == 0;
		}
	};
}
#endif