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
#ifndef __VX_ALIGNEDSTORAGE_H
#define __VX_ALIGNEDSTORAGE_H
#pragma once

#include <vxLib/type_traits.h>

namespace vx
{
	template<class T>
	struct AlignedStorage
	{
		union
		{
			u8 m_buffer[sizeof(T)];
			typename typename detail::SelectAlign<T>::type _aligner;
		};

	};

	template<class T, u32 N>
	struct AlignedStorageN
	{
		union
		{
			u8 m_buffer[sizeof(T) * N];
			typename typename detail::SelectAlign<T>::type _aligner;
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