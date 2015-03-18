#ifndef __VX_TYPE_TRAITS
#define __VX_TYPE_TRAITS
#pragma once

#include <vxLib\types.h>

namespace vx
{
	VX_ALIGN(16) struct Aligned16{};
	VX_ALIGN(32) struct Aligned32{};
	VX_ALIGN(64) struct Aligned64{};
	typedef Aligned64 max_align_t;

	template<size_t Align>
	struct AlignedType;

	template<>
	struct AlignedType < 1 >
	{
		typedef U8 type;
	};

	template<>
	struct AlignedType < 2 >
	{
		typedef U16 type;
	};

	template<>
	struct AlignedType < 4 >
	{
		typedef U32 type;
	};

	template<>
	struct AlignedType < 8 >
	{
		typedef U64 type;
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
			typedef typename std::conditional<std::alignment_of<T>::value <= ALIGN, AlignedType<ALIGN>::type, SelectAlignImpl<T, ALIGN + ALIGN>::type>::type type;
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

#endif