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
#ifndef __VX_KEYBOARD_H
#define __VX_KEYBOARD_H
#pragma once

#include <vxLib/types.h>

namespace vx
{
	struct Keyboard
	{
		U8 m_keys[256];

		U8& operator[](U32 i)
		{
			return m_keys[i];
		}

		const U8& operator[](U32 i) const
		{
			return m_keys[i];
		}

		static U8 Key_A;
		static U8 Key_B;
		static U8 Key_C;
		static U8 Key_D;
		static U8 Key_E;
		static U8 Key_F;
		static U8 Key_G;
		static U8 Key_H;
		static U8 Key_I;
		static U8 Key_J;
		static U8 Key_K;
		static U8 Key_L;
		static U8 Key_M;
		static U8 Key_N;
		static U8 Key_O;
		static U8 Key_P;
		static U8 Key_Q;
		static U8 Key_R;
		static U8 Key_S;
		static U8 Key_T;
		static U8 Key_U;
		static U8 Key_V;
		static U8 Key_W;
		static U8 Key_X;
		static U8 Key_Y;
		static U8 Key_Z;

		static U8 Key_1;
		static U8 Key_2;
		static U8 Key_3;
		static U8 Key_4;
		static U8 Key_5;
		static U8 Key_6;
		static U8 Key_7;
		static U8 Key_8;
		static U8 Key_9;
		static U8 Key_0;

		static U8 Key_F1;
		static U8 Key_F2;
		static U8 Key_F3;
		static U8 Key_F4;
		static U8 Key_F5;
		static U8 Key_F6;
		static U8 Key_F7;
		static U8 Key_F8;
		static U8 Key_F9;
		static U8 Key_F10;
		static U8 Key_F11;
		static U8 Key_F12;

		static U8 Key_Enter;
		static U8 Key_Shift;
		static U8 Key_Ctrl;
		static U8 Key_Alt;
		static U8 Key_Space;
		static U8 Key_Tab;
		static U8 Key_Escape;

		static U8 Key_Num0;
		static U8 Key_Num1;
		static U8 Key_Num2;
		static U8 Key_Num3;
		static U8 Key_Num4;
		static U8 Key_Num5;
		static U8 Key_Num6;
		static U8 Key_Num7;
		static U8 Key_Num8;
		static U8 Key_Num9;
	};
}

#endif