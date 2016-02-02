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
#include <type_traits>

namespace vx
{
	struct VX_ALIGN(16) Aligned16 { f32 padding[4]; };
	struct VX_ALIGN(32) Aligned32{ f32 padding[8]; };
	struct VX_ALIGN(64) Aligned64{ f32 padding[16]; };
	typedef Aligned64 max_align_t;

	template<size_t Align>
	struct AlignedType;

	template<>
	struct AlignedType < 1 >
	{
		typedef u8 type;
	};

	template<>
	struct AlignedType < 2 >
	{
		typedef u16 type;
	};

	template<>
	struct AlignedType < 4 >
	{
		typedef u32 type;
	};

	template<>
	struct AlignedType < 8 >
	{
		typedef u64 type;
	};

	template<>
	struct AlignedType < 16 >
	{
		typedef Aligned16 type;
	};

	template<>
	struct AlignedType < 32 >
	{
		typedef Aligned32 type;
	};

	template<>
	struct AlignedType < 64 >
	{
		typedef Aligned64 type;
	};

	namespace detail
	{
		template<class T, size_t ALIGN>
		struct SelectAlignImpl
		{
			typedef typename std::conditional<std::alignment_of<T>::value <= ALIGN, typename AlignedType<ALIGN>::type, typename SelectAlignImpl<T, ALIGN + ALIGN>::type>::type type;
		};

		// MAX ALIGNMENT
		template<class T>
		struct SelectAlignImpl<T, 64>
		{
			typedef typename AlignedType<64>::type type;
		};

		template<class T>
		struct SelectAlign
		{
			static_assert(__alignof(T) <= __alignof(max_align_t), "Alignment too large !");
			typedef typename SelectAlignImpl<T, 1>::type type;
		};
	}
}