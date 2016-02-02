#pragma once

/*
The MIT License(MIT)

Copyright(c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <vxLib/type_traits.h>
#include <algorithm>

namespace vx
{
	namespace detail
	{
		template<typename T>
		struct AbsMask;

		template<>
		struct AbsMask<u32>
		{
			enum : u32 { mask = 0x7fffffff };
		};

		template<>
		struct AbsMask<u64>
		{
			enum : u64 { mask = 0x7fffffff };
		};
	}

	template<typename T>
	inline void memcpy(u8* dst, const T &src)
	{
		::memcpy(dst, (u8*)&src, sizeof(T));
	}

	template<typename T>
	inline void memcpy(T* dst, const T &src)
	{
		::memcpy((u8*)dst, (u8*)&src, sizeof(T));
	}

	template<typename T>
	inline void memcpy(u8* dst, const T* src, u32 count)
	{
		const auto size = sizeof(T) * count;
		::memcpy(dst, (u8*)src, size);
	}

	template<typename T>
	inline void memcpy(T* dst, const T* src, u32 count)
	{
		const auto size = sizeof(T) * count;
		::memcpy((u8*)dst, (u8*)src, size);
	}

	template<typename T>
	void copy(T* dst, T* src, u32 count)
	{
		for (u32 i = 0; i < count; ++i)
		{
			dst[i] = src[i];
		}
	}

	template<typename T, typename U, typename Converter>
	void copy(T* dst, U* src, u32 count, Converter cnvrt)
	{
		for (u32 i = 0; i < count; ++i)
		{
			cnvrt(dst[i], src[i]);
		}
	}

	template<typename T, typename Cmp>
	inline void sort_subrange(T f, T l, T sf, T sl, Cmp cmp)
	{
		if (sf == sl) return;

		if (sf != f)
		{
			std::nth_element(f, sf, l, cmp);
			++sf;
		}

		std::partial_sort(sf, sl, l);
	}

	template<typename T, typename P>
	auto stable_partition(T f, T l, P p) -> T
	{
		auto n = l - f;
		if (n == 0) return f;
		if (n == 1) return f + p(*f);

		auto m = f + (n / 2);

		return std::rotate(stable_partition(f, m, p), m, stable_partition(m, l, p));
	}

	template<typename T, typename P>
	auto stable_partition_position(T f, T l, P p) -> T
	{
		auto n = l - f;
		if (n == 0) return f;
		if (n == 1) return f + p(f);

		auto m = f + (n / 2);

		return std::rotate(stable_partition_position(f, m, p), m, stable_partition_position(m, l, p));
	}
}