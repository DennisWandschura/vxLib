#ifndef __VX_MEMCPY_H
#define __VX_MEMCPY_H
#pragma once

#include <vxLib/types.h>

namespace vx
{
	template<typename T>
	inline void memcpy(U8* dst, const T &src)
	{
		::memcpy(dst, (U8*)&src, sizeof(T));
	}

	template<typename T>
	inline void memcpy(T* dst, const T &src)
	{
		::memcpy((U8*)dst, (U8*)&src, sizeof(T));
	}

	template<typename T>
	inline void memcpy(U8* dst, const T* src, U32 count)
	{
		const auto size = sizeof(T) * count;
		::memcpy(dst, (U8*)src, size);
	}

	template<typename T>
	inline void memcpy(T* dst, const T* src, U32 count)
	{
		const auto size = sizeof(T) * count;
		::memcpy((U8*)dst, (U8*)src, size);
	}
}

#endif