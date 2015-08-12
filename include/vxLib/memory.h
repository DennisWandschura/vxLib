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
#include <memory>

namespace vx
{
	template<class _Ty, class... _Types> 
	inline typename std::enable_if<!std::is_array<_Ty>::value,
		std::unique_ptr<_Ty> >::type make_unique(_Types&&... _Args)
	{	// make a unique_ptr
		return (std::unique_ptr<_Ty>(new _Ty(std::forward<_Types>(_Args)...)));
	}

	template<class _Ty> inline
		typename std::enable_if<std::is_array<_Ty>::value && std::extent<_Ty>::value == 0,
		std::unique_ptr<_Ty> >::type make_unique(size_t _Size)
	{	// make a unique_ptr
		typedef typename std::remove_extent<_Ty>::type _Elem;
		return (std::unique_ptr<_Ty>(new _Elem[_Size]()));
	}

	template<class _Ty,
	class... _Types>
		typename std::enable_if<std::extent<_Ty>::value != 0,
		void>::type make_unique(_Types&&...) = delete;
}