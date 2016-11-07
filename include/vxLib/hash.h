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

#include <vxLib/types.h>
#include <cstring>
#include <type_traits>

namespace vx
{
	typedef u32 hash_type;

	namespace detail
	{
		constexpr u32 get_k_1(u32 k)
		{
			return ((k << 15) | (k >> (32 - 15))) * 0x1b873593;
		}

		constexpr u32 get_k(u32 k)
		{
			return get_k_1(k * 0xcc9e2d51);
		}

		constexpr u32 get_h_0(u32 h)
		{
			return ((h << 13) | (h >> (32 - 13))) * 5 + 0xe6546b64;
		}

		constexpr u32 get_h(u32 k, u32 h)
		{
			return get_h_0(h ^ k);
		}

		constexpr u32 fetch32_const(const char* s)
		{
			return static_cast<u32>(s[0])
				| static_cast<u32>(s[1] << 8)
				| static_cast<u32>(s[2] << 16)
				| static_cast<u32>(s[3] << 24);
		}

		template<size_t LEN>
		constexpr u32 loop_body(const char(&p)[LEN], int index = 0, u32 h = 0)
		{
			return (index == ((LEN - 1) / 4) * 4) ? h :
				loop_body(
					p,
					index + 4,
					get_h(get_k(
						static_cast<u32>(p[index]) | static_cast<u32>(p[index + 1] << 8) | static_cast<u32>(p[index + 2] << 16) | static_cast<u32>(p[index + 3] << 24)
					), h)
				);
		}

		constexpr u32 remainder_1_k1(u32 k)
		{
			return ((k << 15) | (k >> (32 - 15))) * 0x1b873593;
		}

		template<size_t LEN>
		constexpr u32 remainder_1_k(const char(&p)[LEN], u32 k, size_t SIZE)
		{
			return remainder_1_k1((k ^ p[SIZE]) * 0xcc9e2d51);
		}

		template<size_t LEN>
		constexpr u32 remainder_1(const char(&p)[LEN], u32 k, size_t cmp, size_t SIZE)
		{
			return (cmp == 1) ? remainder_1_k(p, k, SIZE) : k;
		}

		template<size_t LEN>
		constexpr u32 remainder_2_k(const char(&p)[LEN], u32 k, size_t SIZE)
		{
			return k ^ (p[SIZE + 1] << 8);
		}

		template<size_t LEN>
		constexpr u32 remainder_2(const char(&p)[LEN], u32 k, size_t cmp, size_t SIZE)
		{
			return (cmp == 2) ? remainder_1_k(p, remainder_2_k(p, k, SIZE), SIZE)
				: remainder_1(p, k, cmp, SIZE);
		}

		template<size_t LEN>
		constexpr u32 remainder_3_k(const char(&p)[LEN], u32 k, size_t SIZE)
		{
			return k ^ p[SIZE + 2];
		}

		template<size_t LEN>
		constexpr u32 remainder_3(const char(&p)[LEN], u32 h, size_t cmp, size_t SIZE)
		{
			return h ^ ((cmp == 3) ? remainder_1_k(p, remainder_2_k(p, 0 ^ (p[SIZE + 2] << 16), SIZE), SIZE)
				: remainder_2(p, 0, cmp, SIZE));
		}

		constexpr u32 finalize_h3(u32 h)
		{
			return h ^ (h >> 16);
		}

		constexpr u32 finalize_h2(u32 h)
		{
			return finalize_h3((h ^ (h >> 13)) * 0xc2b2ae35);
		}

		constexpr u32 finalize_h1(u32 h)
		{
			return finalize_h2((h ^ (h >> 16)) * 0x85ebca6b);
		}

		constexpr u32 finalize_h(u32 h, u32 len)
		{
			return finalize_h1(h ^ len);
		}

		extern hash_type murmurhashImpl(const char *key, size_t len, u32 seed);

		template<typename T>
		struct HashMaker;

		template<>
		struct HashMaker<char>
		{
			static vx::hash_type run(const char* key)
			{
				return vx::detail::murmurhashImpl(key, strlen(key), 0);
			}
		};

		template<typename T, size_t SIZE>
		struct HashMakerArray
		{
			static typename
				std::enable_if<!std::is_same<T, char>::value, vx::hash_type>::type
				run(const T(&key)[SIZE])
			{
				return vx::detail::murmurhashImpl(reinterpret_cast<const char*>(key), sizeof(T) * SIZE, 0);
			}
		};

		template<size_t SIZE>
		struct HashMakerArray<char, SIZE>
		{
			constexpr static vx::hash_type run(const char(&key)[SIZE])
			{
				return vx::murmurhash(key);
			}
		};
	}

	template<size_t LEN>
	constexpr hash_type murmurhash(const char(&key)[LEN])
	{
		return detail::finalize_h(detail::remainder_3(key, detail::loop_body(key), (LEN - 1) & 3, ((LEN - 1) / 4) * 4), LEN - 1);
	}

	inline hash_type murmurhash(const char* value, size_t len)
	{
		return detail::murmurhashImpl(value, len, 0);
	}

	template<typename T, size_t SIZE>
	inline constexpr hash_type make_hash(const T(&key)[SIZE])
	{
		return detail::HashMakerArray<T, SIZE>::run(key);
	}

	template<typename T>
	inline hash_type make_hash(const T &key)
	{
		return detail::HashMaker<T>::run(key);
	}

	template<typename T>
	inline hash_type make_hash(const T* value, size_t size)
	{
		return detail::murmurhashImpl(reinterpret_cast<const char*>(value), len, 0);
	}
}