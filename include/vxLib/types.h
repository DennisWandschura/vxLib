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

#include <vxLib/platform.h>
#include <stdint.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

static_assert(sizeof(s8) == 1, "Wrong type size");
static_assert(sizeof(u8) == 1, "Wrong type size");
static_assert(sizeof(s16) == 2, "Wrong type size");
static_assert(sizeof(u16) == 2, "Wrong type size");
static_assert(sizeof(s32) == 4, "Wrong type size");
#ifndef  _VX_PLATFORM_ANDROID
static_assert(sizeof(s32) == sizeof(long), "Wrong type size");
#endif // ! _VX_PLATFORM_ANDROID
static_assert(sizeof(u32) == 4, "Wrong type size");
static_assert(sizeof(s64) == 8, "Wrong type size");
static_assert(sizeof(u64) == 8, "Wrong type size");
static_assert(sizeof(f32) == 4, "Wrong type size");
static_assert(sizeof(f64) == 8, "Wrong type size");

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

namespace vx
{
	static const s8 s8_max = 0x7f;
	static const u8 u8_max = 0xff;
	static const s16 s16_max = 0x7fff;
	static const u16 u16_max = 0xffff;
	static const s32 s32_max = 0x7fffffff;
	static const u32 u32_max = 0xffffffff;
	static const s64 s64_max = 0x7fffffffffffffff;
	static const u64 u64_max = 0xffffffffffffffff;
}

#define VX_UNREFERENCED_PARAMETER(P) (P)

#if defined(__GNUC__) || defined(__INTEL_COMPILER)
# define VX_LIKELY(x) __builtin_expect(!!(x), 1)
# define VX_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
# define VX_LIKELY(x)
# define VX_UNLIKELY(x)
#endif

#define KBYTE *1024
#define MBYTE KBYTE *1024

#ifdef _VX_VS12
#define thread_local __declspec(thread)
#define noexcept
#endif

namespace vx
{
	namespace detail
	{
		template<u64>
		struct EnumDataType;

		template<>
		struct EnumDataType<1>
		{
			typedef u8 type;
		};

		template<>
		struct EnumDataType<2>
		{
			typedef u16 type;
		};

		template<>
		struct EnumDataType<4>
		{
			typedef u32 type;
		};

		template<>
		struct EnumDataType<8>
		{
			typedef u64 type;
		};

		template<typename T>
		struct EnumType
		{
			typedef typename EnumDataType<sizeof(T)>::type type;
		};
	}
}

#define VX_ENUM_OPERATIONS(ENUM) \
inline ENUM operator|(ENUM l, ENUM r) { return static_cast<ENUM>(static_cast<vx::detail::EnumType<ENUM>::type>(l) | static_cast<vx::detail::EnumType<ENUM>::type>(r)); } \
inline ENUM operator&(ENUM l, ENUM r) { return static_cast<ENUM>(static_cast<vx::detail::EnumType<ENUM>::type>(l) & static_cast<vx::detail::EnumType<ENUM>::type>(r)); } \
inline ENUM operator^(ENUM l, ENUM r) { return static_cast<ENUM>(static_cast<vx::detail::EnumType<ENUM>::type>(l) ^ static_cast<vx::detail::EnumType<ENUM>::type>(r)); } \
inline ENUM operator~(ENUM l) { return static_cast<ENUM>( ~static_cast<vx::detail::EnumType<ENUM>::type>(l) ); } \
inline ENUM &operator |= (ENUM &l, ENUM r) { return reinterpret_cast<ENUM&>(reinterpret_cast<vx::detail::EnumType<ENUM>::type&>(l) |= static_cast<vx::detail::EnumType<ENUM>::type>(r)); } \
inline ENUM &operator &= (ENUM &l, ENUM r) { return reinterpret_cast<ENUM&>(reinterpret_cast<vx::detail::EnumType<ENUM>::type&>(l) &= static_cast<vx::detail::EnumType<ENUM>::type>(r)); } \
inline ENUM &operator ^= (ENUM &l, ENUM r) { return reinterpret_cast<ENUM&>(reinterpret_cast<vx::detail::EnumType<ENUM>::type&>(l) ^= static_cast<vx::detail::EnumType<ENUM>::type>(r)); }
