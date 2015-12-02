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
	template<typename T, typename Allocator>
	class DoubleBuffer
	{
		typedef size_t size_type;
		typedef T value_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;

		template<bool isPOD>
		struct Destructor
		{
			template<typename T>
			void operator()(T*)
			{
			}
		};

		template<>
		struct Destructor<false>
		{
			template<typename T>
			void operator()(T* ptr)
			{
				ptr->~T();
			}
		};

		typedef Destructor<std::is_pod<value_type>::value> MyDestructor;

		pointer m_frontBuffer;
		pointer m_backBuffer;
		size_type m_sizeFront;
		size_type m_sizeBack;
		size_type m_capacity;
		Allocator m_allocator;
		size_t m_allocSize;

		void allocateMemory(size_type capacity)
		{
			auto block = m_allocator.allocate(sizeof(value_type) * capacity * 2, __alignof(value_type));
			if (block.ptr)
			{
				m_frontBuffer = reinterpret_cast<pointer>(block.ptr);
				m_backBuffer = m_frontBuffer + capacity;
				m_capacity = capacity;
				m_allocSize = block.size;
			}
		}

		void clearBuffer(pointer ptr, size_type count)
		{
			for (size_type i = 0; i < count; ++i)
			{
				MyDestructor()(ptr + i);
			}
		}

		void clearFrontBuffer()
		{
			auto size = m_sizeFront;
			auto ptr = m_frontBuffer;
			clearBuffer(ptr, size);
			m_sizeFront = 0;
		}

		void clearBackBuffer()
		{
			auto size = m_sizeBack;
			auto ptr = m_backBuffer;
			clearBuffer(ptr, size);
			m_sizeBack = 0;
		}

	public:
		DoubleBuffer()
			:m_frontBuffer(nullptr),
			m_backBuffer(nullptr),
			m_sizeFront(0),
			m_sizeBack(0),
			m_capacity(0),
			m_allocator(nullptr),
			m_allocSize(0){}

		DoubleBuffer(Allocator &&allocator, size_type capacity)
			:m_frontBuffer(nullptr),
			m_backBuffer(nullptr),
			m_sizeFront(0),
			m_sizeBack(0),
			m_capacity(0),
			m_allocator(std::move(allocator)),
			m_allocSize(0)
		{
			allocateMemory(capacity);
		}

		template<typename U>
		DoubleBuffer(U* ptr, size_type capacity)
			:DoubleBuffer(std::move(Allocator(ptr)), capacity)
		{
		}

		DoubleBuffer(const DoubleBuffer&) = delete;

		DoubleBuffer(DoubleBuffer &&rhs) noexcept
			:m_frontBuffer(rhs.m_frontBuffer),
			m_backBuffer(rhs.m_backBuffer),
			m_sizeFront(rhs.m_sizeFront),
			m_sizeBack(rhs.m_sizeBack),
			m_capacity(rhs.m_capacity),
			m_allocator(std::move(rhs.m_allocator)),
			m_allocSize(rhs.m_allocSize)
		{
		}

		~DoubleBuffer()
		{
			release();
		}

		DoubleBuffer& operator=(const DoubleBuffer&) = delete;

		DoubleBuffer& operator=(DoubleBuffer &&rhs) noexcept
		{
			if (this != &rhs)
			{
				swap(rhs);
			}

			return *this;
		}

		void swap(DoubleBuffer &other)
		{
			vx::swap(m_frontBuffer, other.m_frontBuffer);
			vx::swap(m_backBuffer, other.m_backBuffer);
			vx::swap(m_sizeFront, other.m_sizeFront);
			vx::swap(m_sizeBack, other.m_sizeBack);
			vx::swap(m_capacity, other.m_capacity);
			m_allocator.swap(other.m_allocator);
			vx::swap(m_allocSize, other.m_allocSize);
		}

		void release()
		{
			if (m_frontBuffer)
			{
				clearFrontBuffer();
				clearBackBuffer();

				m_allocator.deallocate({ (u8*)m_frontBuffer, m_allocSize });
				m_frontBuffer = nullptr;
				m_allocSize = 0;
			}
		}

		template<typename ...Args>
		bool push(Args&& ...args)
		{
			if (m_sizeFront >= m_capacity)
				return false;

			auto p = m_frontBuffer + m_sizeFront;
			new (p)value_type{std::forward<Args>(args)...};
			++m_sizeFront;

			return true;
		}

		void pop(value_type &value)
		{
			auto &lastElement = m_frontBuffer[m_sizeFront - 1];
			value = std::move(lastElement);

			MyDestructor()(&lastElement);
			--m_sizeFront;
		}

		void pop_backBuffer(value_type &value)
		{
			auto &lastElement = m_backBuffer[m_sizeBack - 1];
			value = std::move(lastElement);

			MyDestructor()(&lastElement);
			--m_sizeBack;
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