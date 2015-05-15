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
	struct Keyboard
	{
		u8 m_keys[256];

		u8& operator[](u32 i)
		{
			return m_keys[i];
		}

		const u8& operator[](u32 i) const
		{
			return m_keys[i];
		}

		static u8 Key_A;
		static u8 Key_B;
		static u8 Key_C;
		static u8 Key_D;
		static u8 Key_E;
		static u8 Key_F;
		static u8 Key_G;
		static u8 Key_H;
		static u8 Key_I;
		static u8 Key_J;
		static u8 Key_K;
		static u8 Key_L;
		static u8 Key_M;
		static u8 Key_N;
		static u8 Key_O;
		static u8 Key_P;
		static u8 Key_Q;
		static u8 Key_R;
		static u8 Key_S;
		static u8 Key_T;
		static u8 Key_U;
		static u8 Key_V;
		static u8 Key_W;
		static u8 Key_X;
		static u8 Key_Y;
		static u8 Key_Z;

		static u8 Key_1;
		static u8 Key_2;
		static u8 Key_3;
		static u8 Key_4;
		static u8 Key_5;
		static u8 Key_6;
		static u8 Key_7;
		static u8 Key_8;
		static u8 Key_9;
		static u8 Key_0;

		static u8 Key_F1;
		static u8 Key_F2;
		static u8 Key_F3;
		static u8 Key_F4;
		static u8 Key_F5;
		static u8 Key_F6;
		static u8 Key_F7;
		static u8 Key_F8;
		static u8 Key_F9;
		static u8 Key_F10;
		static u8 Key_F11;
		static u8 Key_F12;

		static u8 Key_Enter;
		static u8 Key_Shift;
		static u8 Key_Ctrl;
		static u8 Key_Alt;
		static u8 Key_Space;
		static u8 Key_Tab;
		static u8 Key_Escape;

		static u8 Key_Num0;
		static u8 Key_Num1;
		static u8 Key_Num2;
		static u8 Key_Num3;
		static u8 Key_Num4;
		static u8 Key_Num5;
		static u8 Key_Num6;
		static u8 Key_Num7;
		static u8 Key_Num8;
		static u8 Key_Num9;
	};
}