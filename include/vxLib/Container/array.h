#ifndef __VX_ARRAY_H
#define __VX_ARRAY_H
#pragma once

#include <vxLib/AlignedStorage.h>

namespace vx
{
	template<typename T, U32 N>
	class array
	{
	public:
		using value_type = T;
		using reference = value_type&;
		using const_reference = const reference;
		using pointer = value_type*;
		using const_pointer = const pointer;

		using size_type = U32;
		using difference_type = U64;

	private:
		AlignedStorageN<value_type, N> m_data;

	public:
		array() : m_data(){}
	};
}

#endif