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
	typename std::enable_if<
		std::is_trivially_copyable<T>::value, 
		void>::type
	copy(T* first, T* last, T* dst)
	{
		::memmove(dst, first, sizeof(T) * (last - first));
	}

	template<typename T>
	typename std::enable_if<
		!std::is_trivially_copyable<T>::value,
		void>::type
	copy(T* first, T* last, T* dst)
	{
		for (; first != last; ++dst, ++first)
			*dst = *first;
	}

	template<typename T>
	typename std::enable_if<
		std::is_trivially_move_assignable<T>::value,
		void>::type
	move(T* first, T* last, T* dst)
	{
		::memmove(dst, first, sizeof(T) * (last - first));
	}

	template<typename T>
	typename std::enable_if<
		!std::is_trivially_move_assignable<T>::value,
		void>::type
	move(T* first, T* last, T* dst)
	{
		for (; first != last; ++dst, ++first)
			*dst = std::move(*first);
	}

	template<typename T>
	typename std::enable_if<
		std::is_trivially_move_assignable<T>::value,
		void>::type
		moveConstruct(T* first, T* last, T* dst)
	{
		::memmove(dst, first, sizeof(T) * (last - first));
	}

	template<typename T>
	typename std::enable_if<
		!std::is_trivially_move_assignable<T>::value,
		void>::type
		moveConstruct(T* first, T* last, T* dst)
	{
		for (; first != last; ++dst, ++first)
			new (dst) T{ std::move(*first) };
	}

	template<typename T>
	typename std::enable_if<
		std::is_pod<T>::value,
		void>::type
	destruct(T* begin, T* end)
	{
		VX_UNREFERENCED_PARAMETER(begin);
		VX_UNREFERENCED_PARAMETER(end);
	}

	template<typename T>
	typename std::enable_if<
		std::is_pod<T>::value,
		void>::type
		destruct(T* p)
	{
		VX_UNREFERENCED_PARAMETER(p);
	}

	template<typename T>
	typename std::enable_if<
		!std::is_pod<T>::value,
		void>::type
		destruct(T* begin, T* end)
	{
		while (begin != end)
		{
			(begin++)->~T();
		}
	}

	template<typename T>
	typename std::enable_if<
		!std::is_pod<T>::value,
		void>::type
		destruct(T* p)
	{
		VX_UNREFERENCED_PARAMETER(p);
		p->~T();
	}

	template<typename T>
	void construct(T* dst)
	{
		new (dst)T{};
	}

	template<typename T>
	void construct(T* dst, const T &value)
	{
		new (dst)T{value};
	}

	template<typename T, typename ...Args>
	void construct(T* dst, Args&& ...args)
	{
		new (dst)T{ std::forward<Args>(args)... };
	}

	template<typename T>
	typename std::enable_if<
		std::is_pod<T>::value, void>::type
		construct(T* begin, T* end)
	{
		::memset(begin, 0, end - begin);
	}

	template<typename T>
	typename std::enable_if<
		!std::is_pod<T>::value, void>::type
		construct(T* begin, T* end)
	{
		while (begin != end)
		{
			new (begin++) T{};
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

	namespace detail
	{
		template<bool b, size_t LHS, size_t RHS>
		struct MAX_T
		{
			enum { value = LHS };
		};

		template<size_t LHS, size_t RHS>
		struct MAX_T<false, LHS, RHS>
		{
			enum { value = RHS };
		};

		template<size_t LHS, size_t ...Args>
		struct MAX
		{
			enum { value = MAX<LHS, MAX<Args...>::value>::value };
		};

		template<size_t SIZE>
		struct MAX<SIZE>
		{
			enum { value = SIZE };
		};

		template<size_t LHS, size_t RHS>
		struct MAX<LHS, RHS>
		{
			enum {
				value = MAX_T<
				Is_Greater<LHS, RHS>::value,
				LHS, RHS>::value
			};
		};

		template<bool b, size_t LHS, size_t RHS>
		struct MIN_T
		{
			enum { value = LHS };
		};

		template<size_t LHS, size_t RHS>
		struct MIN_T<false, LHS, RHS>
		{
			enum { value = RHS };
		};

		template<size_t LHS, size_t ...Args>
		struct MIN
		{
			enum { value = MIN<LHS, MIN<Args...>::value>::value };
		};

		template<size_t LHS>
		struct MIN<LHS>
		{
			enum { value = LHS };
		};

		template<size_t LHS, size_t RHS>
		struct MIN<LHS, RHS>
		{
			enum {
				value = MIN_T<
				Is_Less<LHS, RHS>::value,
				LHS, RHS>::value
			};
		};

		template<typename LHS, typename ...Args>
		struct MAX_SIZE
		{
			enum { value = MAX<sizeof(LHS), MAX_SIZE<Args...>::value>::value };
		};

		template<typename LHS>
		struct MAX_SIZE<LHS>
		{
			enum { value = sizeof(LHS) };
		};

		template<typename LHS, typename RHS>
		struct MAX_SIZE<LHS, RHS>
		{
			enum { value = MAX<sizeof(LHS), sizeof(RHS)>::value };
		};
	}

	template<size_t LHS, size_t RHS>
	struct Is_Greater
	{
		enum { value = (LHS > RHS) };
	};

	template<size_t LHS, size_t RHS>
	struct Is_Less
	{
		enum { value = (LHS < RHS) };
	};

	template<size_t ...Args>
	struct MAX
	{
		enum { value = detail::MAX<Args...>::value };
	};

	template<>
	struct MAX<>
	{
		enum { value = 0 };
	};

	template<size_t ...Args>
	struct MIN
	{
		enum { value = detail::MIN<Args...>::value };
	};

	template<>
	struct MIN<>
	{
		enum { value = 0 };
	};

	template<typename ...Args>
	struct MAX_SIZE
	{
		enum { value = detail::MAX_SIZE<Args...>::value };
	};

	template<>
	struct MAX_SIZE<>
	{
		enum { value = 0 };
	};
}