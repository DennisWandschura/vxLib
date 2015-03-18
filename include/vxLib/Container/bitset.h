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
			VX_ASSERT(i < N, "Index out of bounds !");
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
			VX_ASSERT(i < N, "Index out of bounds !");
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
			VX_ASSERT(i < N, "Index out of bounds !");
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
			VX_ASSERT(i < N, "Index out of bounds !");
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