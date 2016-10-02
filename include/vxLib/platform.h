#pragma once

#ifdef _MSC_VER
#define _VX_PLATFORM_WINDOWS 1
#define WIN32_LEAN_AND_MEAN
#if _VX_NO_EXCEPTIONS
#define _HAS_EXCEPTIONS 0
#endif
#elif __ANDROID__
#define _VX_PLATFORM_ANDROID 1
#endif

#if defined __i386 || defined __i386__ || defined _X86_ || defined _M_IX86
#define _VX_X86
#elif defined _M_AMD64 || defined _M_X64 || defined __x86_64 || defined __x86_64__
#define _VX_X64
#endif

#if _VX_ASSERT
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#ifdef NDEBUG
#undef NDEBUG
#endif
#if _VX_PLATFORM_ANDROID
#define VX_ASSERT(_Expression) _assert(_Expression)

#else // _VX_ANDROID

#define VX_ASSERT(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )
#endif // _VX_ANDROID

#else // _VX_ASSERT
#define VX_ASSERT(_Expression) ((void)0)
#endif // _VX_ASSERT

#if defined _VX_PLATFORM_ANDROID
#define VX_CALLCONV 
#define _VX_CALLCONV_TYPE 0
#elif defined(_VX_CUDA) || defined (_VX_GCC)
#define VX_CALLCONV __fastcall
#define _VX_CALLCONV_TYPE 0
#else
#define VX_CALLCONV __vectorcall
#define _VX_CALLCONV_TYPE 1
#endif
#if defined (_VX_GCC)
#define VX_GLOBALCONST extern const __attribute__((selectany))
#define VX_GLOBAL extern __attribute__((selectany))
#elif defined (_VX_CLANG) || defined(_VX_PLATFORM_ANDROID)
#define VX_GLOBALCONST static const
#define VX_GLOBAL extern __attribute__((selectany))
#else
#define VX_GLOBAL extern __declspec(selectany)
#define VX_GLOBALCONST extern const __declspec(selectany)
#endif

#if _VX_PLATFORM_ANDROID
#define VX_ALIGN(X) __attribute__((aligned(X)))
#elif defined(_VX_GCC) || defined(_VX_CLANG) || (_MSC_VER > 1800)
#define VX_ALIGN(X) alignas(X)
#else
#define VX_ALIGN(X) __declspec( align( X ) )
#endif