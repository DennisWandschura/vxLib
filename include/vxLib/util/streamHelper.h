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
#ifndef __VX_STREAMHELPER_H
#define __VX_STREAMHELPER_H

#include <istream>

namespace vx
{
	template<class T>
	bool write(std::ostream &out, const T &value)
	{
		out.write((char*)&value, sizeof(T));

		return out.good();
	}

	template<class T>
	bool write(std::ostream &out, const T *ptr, u32 count)
	{
		out.write((char*)ptr, sizeof(T) * count);

		return out.good();
	}

	template<class T>
	bool read(std::istream &is, T &value)
	{
		is.read((char*)&value, sizeof(T));

		return is.good();
	}

	template<class T>
	bool read(std::istream &is, T *ptr, u32 count)
	{
		is.read((char*)ptr, sizeof(T)*count);

		return is.good();
	}
}
#endif