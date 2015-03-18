#ifndef __VX_ALIGNEDSTORAGE_H
#define __VX_ALIGNEDSTORAGE_H
#pragma once

#include <vxLib\type_traits.h>

namespace vx
{
	template<class T>
	struct AlignedStorage
	{
		union
		{
			U8 m_buffer[sizeof(T)];
			typename detail::SelectAlign<T>::type _aligner;
		};

	};

	template<class T, U32 N>
	struct AlignedStorageN
	{
		union
		{
			U8 m_buffer[sizeof(T) * N];
			typename detail::SelectAlign<T>::type _aligner;
		};

		T& operator[](size_t i)
		{
			return ((T*)m_buffer)[i];
		}

		const T& operator[](size_t i) const
		{
			return ((T*)m_buffer)[i];
		}

		T* data(){ return (T*)m_buffer; }
		const T* data() const { return (const T*)m_buffer; }
	};
}
#endif