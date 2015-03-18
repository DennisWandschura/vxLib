#ifndef __VX_TYPES
#define __VX_TYPES
#pragma once

#include <assert.h>
#include <type_traits>

using I8 = char;
using I16 = short;
using I32 = int;
using I64 = long long;

using U8 = unsigned char;
using U16 = unsigned short;
using U32 = unsigned int;
using U64 = unsigned long long;

using F32 = float;
using F64 = double;

static_assert(sizeof(I8) == 1, "Wrong type size");
static_assert(sizeof(U8) == 1, "Wrong type size");
static_assert(sizeof(I16) == 2, "Wrong type size");
static_assert(sizeof(U16) == 2, "Wrong type size");
static_assert(sizeof(I32) == 4, "Wrong type size");
static_assert(sizeof(U32) == 4, "Wrong type size");
static_assert(sizeof(I64) == 8, "Wrong type size");
static_assert(sizeof(U64) == 8, "Wrong type size");
static_assert(sizeof(F32) == 4, "Wrong type size");
static_assert(sizeof(F64) == 8, "Wrong type size");

#if defined(UNICODE) || defined(_UNICODE)
using char_t = wchar_t;
#else
using char_t = char;
#endif

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

#if defined(_WIN32) || defined(_VX_WINDOWS) || defined(_VX_CUDA)
#define VX_ALIGN(X) __declspec( align( X ) )
#else
#define VX_ALIGN(X) __attribute__((aligned(64)));
#endif

#define VX_GLOBAL extern __declspec(selectany)
#define VX_GLOBALCONST extern const __declspec(selectany)

#ifdef _VX_CUDA
#define VX_CALLCONV __fastcall
#else
#define VX_CALLCONV __vectorcall
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

#endif