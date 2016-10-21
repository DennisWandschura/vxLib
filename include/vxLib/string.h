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

#include <vxLib/types.h>
#include <cstring>

namespace vx
{
	///////////////////////////////// write

	template<typename T>
	// writes data from src to dst and returns a ptr offset by size of src from dst
	inline u8* write(u8* dst, const T &src)
	{
		::memcpy(dst, (u8*)&src, sizeof(T));

		return (dst + sizeof(T));
	}

	template<typename T>
	// writes data from src to dst and returns a ptr offset by size of src from dst
	inline T* write(T* dst, const T &src)
	{
		::memcpy((u8*)dst, (u8*)&src, sizeof(T));

		return (dst + 1);
	}

	template<typename T>
	// writes data from src to dst and returns a ptr offset by size of src from dst
	inline u8* write(u8* dst, const T* src, u32 count)
	{
		const auto size = sizeof(T) * count;
		::memcpy(dst, (u8*)src, size);

		return (dst + size);
	}

	template<typename T>
	// writes data from src to dst and returns a ptr offset by size of src from dst
	inline T* write(T* dst, const T* src, u32 count)
	{
		const auto size = sizeof(T) * count;
		::memcpy((u8*)dst, (u8*)src, size);

		return (dst + count);
	}

	// writes data from src to dst and returns a ptr offset by size of src from dst
	inline u8* write(u8* dst, const u8* src, u64 size)
	{
		::memcpy((u8*)dst, (u8*)src, size);

		return (dst + size);
	}

	///////////////////////////////// write

	///////////////////////////////// read

	template<typename T>
	// writes data from src to dst and returns a ptr offset by size of dst from src
	inline const u8* read(T& dst, const u8* src)
	{
		::memcpy((u8*)&dst, src, sizeof(T));

		return (src + sizeof(T));
	}

	template<typename T>
	// writes data from src to dst and returns a ptr offset by size of dst from src
	inline const T* read(T& dst, const T* src)
	{
		::memcpy((u8*)&dst, (u8*)src, sizeof(T));

		return (src + 1);
	}

	template<typename T>
	// writes data from src to dst and returns a ptr offset by size of dst from src
	inline const u8* read(T* dst, const u8* src, u32 count)
	{
		auto size = sizeof(T) * count;
		::memcpy((u8*)dst, src, size);

		return src + size;
	}

	template<>
	// writes data from src to dst and returns a ptr offset by size of dst from src
	inline const u8* read<u8>(u8* dst, const u8* src, u32 count)
	{
		::memcpy(dst, src, count);

		return (src + count);
	}

	///////////////////////////////// read

	template<typename T>
	inline void memset(T* dst, s32 value)
	{
		::memset(dst, value, sizeof(T));
	}

	template<typename T>
	inline void setZero(T* dst)
	{
		::memset(dst, 0, sizeof(T));
	}

	template<typename T>
	inline void setZero(T* dst, size_t size)
	{
		::memset(dst, 0, size);
	}

	namespace detail
	{

		/* calculate absolute value */
		constexpr int abs_val(int x)
		{
			return x < 0 ? -x : x;
		}

		/* calculate number of digits needed, including minus sign */
		constexpr int num_digits(int x)
		{
			return x < 0 ? 1 + num_digits(-x) : x < 10 ? 1 : 1 + num_digits(x / 10);
		}

		/* metaprogramming string type: each different string is a unique type */
		template<char... args>
		struct metastring {
			const char data[sizeof... (args)] = { args... };
		};

		/* recursive number-printing template, general case (for three or more digits) */
		template<int size, int x, char... args>
		struct numeric_builder {
			typedef typename numeric_builder<size - 1, x / 10, '0' + abs_val(x) % 10, args...>::type type;
		};

		/* special case for two digits; minus sign is handled here */
		template<int x, char... args>
		struct numeric_builder<2, x, args...> {
			typedef metastring < x < 0 ? '-' : '0' + x / 10, '0' + abs_val(x) % 10, args...> type;
		};

		/* special case for one digit (positive numbers only) */
		template<int x, char... args>
		struct numeric_builder<1, x, args...> {
			typedef metastring<'0' + x, args...> type;
		};

		/* convenience wrapper for numeric_builder */
		template<int x>
		class numeric_string
		{
		private:
			/* generate a unique string type representing this number */
			typedef typename numeric_builder<num_digits(x), x, '\0'>::type type;

			/* declare a static string of that type (instantiated later at file scope) */
			static constexpr type value{};

		public:
			/* returns a pointer to the instantiated string */
			static constexpr const char * get()
			{
				return value.data;
			}
		};
	}

	template<int x>
	struct IntToString
	{
		static constexpr auto get()
		{
			return detail::numeric_builder<detail::num_digits(x), x, '\0'>::type{};
		}
	};

	extern bool int_to_string(s32 value, char* buffer, u32 size, u32* sizeOut = nullptr);
	extern bool int_to_string(u32 value, char* buffer, u32 size, u32* sizeOut = nullptr);
	extern bool int_to_string(s64 value, char* buffer, u32 size, u32* sizeOut = nullptr);
	extern bool int_to_string(u64 value, char* buffer, u32 size, u32* sizeOut = nullptr);
}