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

#if _VX_NO_EXCEPTIONS
#define _HAS_EXCEPTIONS 0
#endif

#if _VX_WINDOWS
#define WIN32_LEAN_AND_MEAN
#endif

#include <assert.h>

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long int s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

static_assert(sizeof(s8) == 1, "Wrong type size");
static_assert(sizeof(u8) == 1, "Wrong type size");
static_assert(sizeof(s16) == 2, "Wrong type size");
static_assert(sizeof(u16) == 2, "Wrong type size");
static_assert(sizeof(s32) == 4, "Wrong type size");
static_assert(sizeof(s32) == sizeof(long), "Wrong type size");
static_assert(sizeof(u32) == 4, "Wrong type size");
static_assert(sizeof(s64) == 8, "Wrong type size");
static_assert(sizeof(u64) == 8, "Wrong type size");
static_assert(sizeof(f32) == 4, "Wrong type size");
static_assert(sizeof(f64) == 8, "Wrong type size");

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

#if _VX_ASSERT
#ifdef  NDEBUG
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	_CRTIMP void __cdecl _wassert(_In_z_ const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif
#define VX_ASSERT(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )
#else
#define VX_ASSERT(_Expression) ((void)0)
#endif

#if defined(_VX_GCC) || defined(_VX_CLANG) || (_MSC_VER > 1800)
#define VX_ALIGN(X) alignas(X)
#else
#define VX_ALIGN(X) __declspec( align( X ) )
#endif

#define VX_GLOBAL extern __declspec(selectany)
#if defined (_VX_GCC)
#define VX_GLOBALCONST extern const __attribute__((selectany))
#elif defined (_VX_CLANG)
#define VX_GLOBALCONST static const
#else
#define VX_GLOBALCONST extern const __declspec(selectany)
#endif

#if defined(_VX_CUDA) || defined (_VX_GCC)
#define VX_CALLCONV __fastcall
#define _VX_CALLCONV_TYPE 0
#else
#define VX_CALLCONV __vectorcall
#define _VX_CALLCONV_TYPE 1
#endif

#define VX_UNREFERENCED_PARAMETER(P) (P)

#if defined(__GNUC__) || defined(__INTEL_COMPILER)
# define VX_LIKELY(x) __builtin_expect(!!(x), 1)
# define VX_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
# define VX_LIKELY(x)
# define VX_UNLIKELY(x)
#endif

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#define KBYTE *1024
#define MBYTE KBYTE *1024

#ifdef _VX_VS12
#define thread_local __declspec(thread)
#define noexcept
#endif