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
#ifndef __VX_BITSET_H
#define __VX_BITSET_H
#pragma once

#include <vxLib\types.h>

namespace vx
{
	template<size_t N>
	class bitset
	{
		enum { SIZE = (N / 8) + ((N % 8) != 0) };

		U8 m_bits[SIZE];

	public:
		bitset()
			:m_bits()
		{
		}

		void set(size_t i)
		{
			VX_ASSERT(i < N);
			const auto index = i / 8;

			const auto bit = i - index * 8;

			m_bits[index] |= (1 << bit);
		}

		template<size_t I>
		void set()
		{
			static_assert(I < N, "Index out of bounds !");
			const auto index = I / 8;

			const auto bit = I - index * 8;

			m_bits[index] |= (1 << bit);
		}

		void clear(size_t i)
		{
			VX_ASSERT(i < N);
			const auto index = i / 8;

			const auto bit = i - index * 8;

			m_bits[index] &= ~(1 << bit);
		}

		template<size_t I>
		void clear()
		{
			static_assert(I < N, "Index out of bounds !");
			const auto index = I / 8;

			const auto bit = I - index * 8;

			m_bits[index] &= ~(1 << bit);
		}

		void clear()
		{
			memset(m_bits, 0, SIZE);
		}

		bool get(size_t i) const
		{
			VX_ASSERT(i < N);
			const auto index = i / 8;

			const auto bit = i - index * 8;

			return ((m_bits[index] & (1 << bit)) == (1 << bit));
		}

		template<size_t I>
		bool get() const noexcept
		{
			static_assert(I < N, "Index out of bounds !");
			const auto index = I / 8;

			const auto bit = I - index * 8;

			return ((m_bits[index] & (1 << bit)) == (1 << bit));
		}

		bool operator[](size_t i) const
		{
			VX_ASSERT(i < N);
			const auto index = i / 8;

			const auto bit = i - index * 8;

			return ((m_bits[index] & (1 << bit)) == (1 << bit));
		}

		constexpr size_t size() noexcept
		{
			return N;
		}
	};
}
#endif