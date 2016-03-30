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

#include <vxLib/math/half.h>
#include <intrin.h>
#include <math.h>

namespace vx
{
	const f32 VX_EPSILON = 1.192092896e-7f;
	const f32 VX_PI = 3.141592654f;
	const f32 VX_2PI = 6.283185307f;
	const f32 VX_1DIVPI = 0.318309886f;
	const f32 VX_1DIV2PI = 0.159154943f;
	const f32 VX_PIDIV2 = 1.570796327f;
	const f32 VX_PIDIV4 = 0.785398163f;

	const f32 VX_DEGTORAD = 0.0174532925f;
	const f32 VX_RADTODEG = 57.2957795f;

	const f64 VX_PI_D = 3.14159265358979323846;
	const f64 VX_2PI_D = 6.28318530717958647692;
	const f64 VX_1DIVPI_D = 0.318309886183790671538;
	const f64 VX_1DIV2PI_D = 0.15915494309189533576;
	const f64 VX_PIDIV2_D = 1.57079632679489661923;
	const f64 VX_PIDIV4_D = 0.785398163397448309616;

	VX_GLOBALCONST __m128 g_VXAbsMaskFloat = { -0.0f, -0.0f, -0.0f, -0.0f };

	namespace detail
	{
		union FloatInt
		{
			f32 f;
			s32 i;
		};
	}

	template<class T, size_t n>
	inline size_t sizeofArray(const T(&p)[n])
	{
		return n;
	}

	inline f32 clampZeroOne(const f32 value)
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

	inline f32 VX_CALLCONV abs(f32 a)
	{
		__m128 fa = _mm_load_ss(&a);
		fa = _mm_andnot_ps(g_VXAbsMaskFloat, fa);
		return *reinterpret_cast<f32*>(&fa);
	}

	inline f32 VX_CALLCONV min(f32 a, f32 b)
	{
		__m128 fa = _mm_load_ss(&a);
		__m128 fb = _mm_load_ss(&b);

		fa = _mm_min_ss(fa, fb);
		return *reinterpret_cast<f32*>(&fa);
	}

	inline f32 VX_CALLCONV max(f32 a, f32 b)
	{
		__m128 fa = _mm_load_ss(&a);
		__m128 fb = _mm_load_ss(&b);

		fa = _mm_max_ss(fa, fb);
		return *reinterpret_cast<f32*>(&fa);
	}

	inline f32 VX_CALLCONV invsqrt(f32 number)
	{
		__m128 fn = _mm_load_ss(&number);
		fn = _mm_rsqrt_ss(fn);
		return *reinterpret_cast<f32*>(&fn);
	}

	template<typename T>
	inline const T& min(const T &l, const T &r) 
	{
		return (l < r) ? l : r;
	}

	template<typename T>
	inline const T& max(const T &l, const T &r)
	{
		return (l < r) ? r : l;
	}

	template<typename T>
	inline const T clamp(const T &value, const T &vmin, const T &vmax)
	{
		return min(max(value, vmin), vmax);
	}

	template<typename T>
	inline const T abs(const T &l)
	{
		return (0 < l) ? -l : l;
	}

	inline bool scalarNearEqual
		(
			f32 S1,
			f32 S2,
			f32 Epsilon
		)
	{
		f32 Delta = S1 - S2;
		return (abs(Delta) <= Epsilon);
	}

	inline void scalarSinCos
		(
			f32* pSin,
			f32* pCos,
			f32  Value
		)
	{
		VX_ASSERT(pSin);
		VX_ASSERT(pCos);

		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		f32 quotient = VX_1DIV2PI * Value;
		if (Value >= 0.0f)
		{
			quotient = (f32)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (f32)((int)(quotient - 0.5f));
		}
		f32 y = Value - VX_2PI * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		f32 sign;
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

		f32 y2 = y * y;

		// 11-degree minimax approximation
		*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		f32 p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*pCos = sign*p;
	}

	inline void scalarSinCos
		(
			f64* pSin,
			f64* pCos,
			f64  Value
			)
	{
		VX_ASSERT(pSin);
		VX_ASSERT(pCos);

		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		f64 quotient = VX_1DIV2PI_D * Value;
		if (Value >= 0.0)
		{
			quotient = (f64)((int)(quotient + 0.5));
		}
		else
		{
			quotient = (f64)((int)(quotient - 0.5));
		}
		f64 y = Value - VX_2PI_D * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		f64 sign;
		if (y > VX_PIDIV2_D)
		{
			y = VX_PI_D - y;
			sign = -1.0;
		}
		else if (y < -VX_PIDIV2_D)
		{
			y = -VX_PI_D - y;
			sign = -1.0;
		}
		else
		{
			sign = +1.0;
		}

		f64 y2 = y * y;

		// 11-degree minimax approximation
		*pSin = (((((-2.3889859e-08 * y2 + 2.7525562e-06) * y2 - 0.00019840874) * y2 + 0.0083333310) * y2 - 0.16666667) * y2 + 1.0) * y;

		// 10-degree minimax approximation
		f64 p = ((((-2.6051615e-07 * y2 + 2.4760495e-05) * y2 - 0.0013888378) * y2 + 0.041666638) * y2 - 0.5) * y2 + 1.0;
		*pCos = sign*p;
	}

	inline float scalarACos
		(
		float Value
		)
	{
		// Clamp input to [-1,1].
		bool nonnegative = (Value >= 0.0f);
		float x = fabsf(Value);
		float omx = 1.0f - x;
		if (omx < 0.0f)
		{
			omx = 0.0f;
		}
		float root = sqrtf(omx);

		// 7-degree minimax approximation
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
		result *= root;

		// acos(x) = pi - acos(-x) when x < 0
		return (nonnegative ? result : VX_PI - result);
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
		fTemp = fTemp - (VX_2PI * (float)((s32)(fTemp / VX_2PI)));
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

	inline u32 bsf32(const u32 x)
	{
		unsigned long leading_zero = 0;

		_BitScanForward(&leading_zero, x);

		return leading_zero;
	}

	inline u32 bsr32(const u32 x)
	{
		unsigned long leading_zero = 0;

		_BitScanReverse(&leading_zero, x);

		return leading_zero;
	}

	inline u32 bsf64(const u64 x)
	{
		unsigned long leading_zero = 0;

		_BitScanForward64(&leading_zero, x);

		return leading_zero;
	}

	inline u32 bsr64(const u64 x)
	{
		unsigned long leading_zero = 0;

		_BitScanReverse64(&leading_zero, x);

		return leading_zero;
	}
}