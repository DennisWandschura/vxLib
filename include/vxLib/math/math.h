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
#ifndef __VX_MATH_H
#define __VX_MATH_H
#pragma once

#include <vxLib/types.h>
#include <intrin.h>
#include <math.h>
#include <cmath>

namespace vx
{
	const F32 VX_EPSILON = 1.192092896e-7f;
	const F32 VX_PI = 3.141592654f;
	const F32 VX_2PI = 6.283185307f;
	const F32 VX_1DIVPI = 0.318309886f;
	const F32 VX_1DIV2PI = 0.159154943f;
	const F32 VX_PIDIV2 = 1.570796327f;
	const F32 VX_PIDIV4 = 0.785398163f;

	const F32 VX_DEGTORAD = 0.0174532925f;
	const F32 VX_RADTODEG = 57.2957795f;

	namespace detail
	{
		union FloatInt
		{
			float f;
			int i;
		};
	}

	template<class T, size_t n>
	inline size_t sizeofArray(const T(&p)[n])
	{
		return n;
	}

	inline F32 clampZeroOne(const F32 value)
	{
		detail::FloatInt tmp;

		tmp.f = value;
		int mask = tmp.i >> 31;
		tmp.i &= ~mask;

		tmp.f -= 1.0f;
		mask = tmp.i >> 31;
		tmp.i &= mask;

		return tmp.f + 1.0f;
	}

	extern F32 VX_CALLCONV abs(F32 a);

	extern F32 VX_CALLCONV min(F32 a, F32 b);
	extern F32 VX_CALLCONV max(F32 a, F32 b);

	extern F32 VX_CALLCONV invsqrt(F32 number);

	template<typename T>
	inline const T& clamp(const T &value, const T &min, const T &max)
	{
		return ::std::min(::std::max(value, min), max);
	}

	inline bool scalarNearEqual
		(
		float S1,
		float S2,
		float Epsilon
		)
	{
		float Delta = S1 - S2;
		return (abs(Delta) <= Epsilon);
	}

	inline void scalarSinCos
		(
		float* pSin,
		float* pCos,
		float  Value
		)
	{
		assert(pSin);
		assert(pCos);

		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = VX_1DIV2PI * Value;
		if (Value >= 0.0f)
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = Value - VX_2PI * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign;
		if (y > VX_PIDIV2)
		{
			y = VX_PI - y;
			sign = -1.0f;
		}
		else if (y < -VX_PIDIV2)
		{
			y = -VX_PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*pCos = sign*p;
	}

	inline float scalarModAngle
		(
		float Angle
		)
	{
		// Note: The modulo is performed with unsigned math only to work
		// around a precision error on numbers that are close to PI

		// Normalize the range from 0.0f to XM_2PI
		Angle = Angle + VX_PI;
		// Perform the modulo, unsigned
		float fTemp = abs(Angle);
		fTemp = fTemp - (VX_2PI * (float)((I32)(fTemp / VX_2PI)));
		// Restore the number to the range of -XM_PI to XM_PI-epsilon
		fTemp = fTemp - VX_PI;
		// If the modulo'd value was negative, restore negation
		if (Angle<0.0f) 
		{
			fTemp = -fTemp;
		}
		return fTemp;
	}

	/*
	Trigonometry
	*/

	inline float degToRad(float degAngle)
	{
		return degAngle * VX_DEGTORAD;
	}

	inline float radToDeg(float radAngle)
	{
		return radAngle * VX_RADTODEG;
	}

	inline U32 bsf32(const U32 x)
	{
		unsigned long leading_zero = 0;

		_BitScanForward(&leading_zero, x);

		return leading_zero;
	}

	inline U32 bsr32(const U32 x)
	{
		unsigned long leading_zero = 0;

		_BitScanReverse(&leading_zero, x);

		return leading_zero;
	}

	inline U32 bsf64(const U64 x)
	{
		unsigned long leading_zero = 0;

		_BitScanForward64(&leading_zero, x);

		return leading_zero;
	}

	inline U32 bsr64(const U64 x)
	{
		unsigned long leading_zero = 0;

		_BitScanReverse64(&leading_zero, x);

		return leading_zero;
	}
}
#endif