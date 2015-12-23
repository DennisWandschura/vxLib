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

namespace vx
{
	inline u32 rev(u32 x)
	{
		x = (x & 0x55555555) << 1 | (x >> 1) & 0x55555555;
		x = (x & 0x33333333) << 2 | (x >> 2) & 0x33333333;
		x = (x & 0x0f0f0f0f) << 4 | (x >> 4) & 0x0f0f0f0f;
		x = (x << 24) | ((x & 0xff00) << 8) |
			((x >> 8) & 0xff00) | (x >> 24);
		return x;
	}

	inline int nlz(u32 x)
	{
		u32 y;
		int n = 32;

		y = x >> 16; if (y != 0) { n = n - 16; x = y; }
		y = x >> 8; if (y != 0) { n = n - 8; x = y; }
		y = x >> 4; if (y != 0) { n = n - 4; x = y; }
		y = x >> 2; if (y != 0) { n = n - 2; x = y; }
		y = x >> 1; if (y != 0) { n = n - 1; x = y; }
		return n - x;
	}

	inline int ntz(u32 x)
	{
		const char table[64] =
		{
			32, 0, 1, 12, 2, 6, 0, 13, 3, 0, 7, 0, 0, 0, 0, 14,
			10, 4, 0, 0, 8, 0, 0, 25, 0, 0, 0, 0, 0, 21, 27, 15,
			31, 11, 5, 0, 0, 0, 0, 0, 9, 0, 0, 24, 0, 0, 20, 26,
			30, 0, 0, 0, 0, 23, 0, 19, 29, 0, 22, 18, 28, 17, 16, 0
		};

		x = (x & -int(x)) * 0x0450fbaf;
		return table[x >> 26];
	}

	inline int ffstrl(u32 x, int n)
	{
		int s;

		while (n > 1)
		{
			s = n >> 1;
			x = x & (x << s);
			n = n - s;
		}

		return nlz(x);
	}
}