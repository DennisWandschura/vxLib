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
#ifndef __STRINGID_H
#define __STRINGID_H
#pragma once

#include <vxLib/types.h>
#include <vxLib/util/CityHash.h>

namespace vx
{
	struct StringID
	{
		U64 value;

		StringID() :value(0) {}
		StringID(U64 u) :value(u){}

		StringID& operator=(U64 u)
		{
			value = u;
			return *this;
		}

		friend bool operator==(const StringID &lhs, const StringID &rhs)
		{
			return lhs.value == rhs.value;
		}

		friend bool operator!=(const StringID &lhs, const StringID &rhs)
		{
			return lhs.value != rhs.value;
		}

		friend bool operator<(const StringID &lhs, const StringID &rhs)
		{
			return lhs.value < rhs.value;
		}

		friend bool operator<=(const StringID &lhs, const StringID &rhs)
		{
			return lhs.value <= rhs.value;
		}

		friend bool operator>(const StringID &lhs, const StringID &rhs)
		{
			return lhs.value > rhs.value;
		}

		friend bool operator>=(const StringID &lhs, const StringID &rhs)
		{
			return lhs.value >= rhs.value;
		}
	};

	inline StringID make_sid(const char *str)
	{
		StringID sid = CITYHASH64(str);
		return sid;
	}

	template<U64 SIZE>
	inline StringID make_sid(const char(&str)[SIZE])
	{
		StringID sid = CityHash64((char*)str, SIZE);
		return sid;
	}

	inline StringID make_sid(const wchar_t *str)
	{
		StringID sid = CityHash64((char*)str, sizeof(wchar_t) * std::char_traits<wchar_t>::length(str));
		return sid;
	}
}

#endif