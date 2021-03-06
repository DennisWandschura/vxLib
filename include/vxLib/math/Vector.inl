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

namespace vx
{
	inline __m128 VX_CALLCONV fma(CVEC4 a, CVEC4 b, CVEC4 c)
	{
#if _VX_FMA
		return _mm_fmadd_ps(a, b, c);
#else
		__m128 tmp = _mm_mul_ps(a, b);
		return _mm_add_ps(tmp, c);
#endif
	}

	inline __m128 VX_CALLCONV VectorNegativeMultiplySubtract(CVEC4 a, CVEC4 b, CVEC4 c)
	{
		__m128 R = _mm_mul_ps(a, b);
		return _mm_sub_ps(c, R);
	}

	inline __m128 VX_CALLCONV dot2(CVEC4 v1, CVEC4 v2)
	{
#if _VX_SSE41
		return _mm_dp_ps(v1, v2, _VX_DOT(1, 1, 1, 1, 1, 1, 0, 0));
#else
		__m128 tmp = _mm_mul_ps(v1, v2);
		// y, x, w, z
		__m128 tmp1 = VX_PERMUTE_PS(tmp, _MM_SHUFFLE(2, 3, 0, 1));
		// x+y, y+x, ...
		tmp = _mm_add_ps(tmp, tmp1);
		// splat result
		return VX_PERMUTE_PS(tmp, _MM_SHUFFLE(0, 0, 0, 0));
#endif
	}

	inline __m128 VX_CALLCONV dot3(CVEC4 v1, CVEC4 v2)
	{
#if _VX_SSE41
		return _mm_dp_ps(v1, v2, _VX_DOT(1, 1, 1, 1, 1, 1, 1, 0));
#else
		__m128 tmp = _mm_mul_ps(v1, v2);
		// y, x, w, z
		__m128 tmp1 = VX_PERMUTE_PS(tmp, _MM_SHUFFLE(2, 3, 0, 1));
		// z z z z
		__m128 tmp2 = VX_PERMUTE_PS(tmp, _MM_SHUFFLE(2, 2, 2, 2));
		// x+y, y+x, ...
		tmp1 = _mm_add_ps(tmp, tmp1);
		tmp1 = _mm_add_ps(tmp1, tmp2);

		return VX_PERMUTE_PS(tmp1, _MM_SHUFFLE(0, 0, 0, 0));
#endif
	}

	inline __m128 VX_CALLCONV dot4(CVEC4 v1, CVEC4 v2)
	{
#if _VX_SSE41
		return _mm_dp_ps(v1, v2, 255);
#else
		__m128 tmp = _mm_mul_ps(v1, v2);
		// y, x, w, z
		__m128 tmp1 = VX_PERMUTE_PS(tmp, _MM_SHUFFLE(2, 3, 0, 1));
		// x+y, y+x, z + w
		tmp1 = _mm_add_ps(tmp, tmp1);
		__m128 tmp2 = VX_PERMUTE_PS(tmp1, _MM_SHUFFLE(1, 0, 3, 2));
		tmp1 = _mm_add_ps(tmp1, tmp2);

		return VX_PERMUTE_PS(tmp1, _MM_SHUFFLE(0, 0, 0, 0));
#endif
	}

	inline __m128 VX_CALLCONV min(CVEC4 a, CVEC4 b)
	{
		return _mm_min_ps(a, b);
	}

	inline __m128 VX_CALLCONV max(CVEC4 a, CVEC4 b)
	{
		return _mm_max_ps(a, b);
	}

	inline float4a VX_CALLCONV max(const float4a &a, const float4a &b)
	{
		return _mm_max_ps(a.v, b.v);
	}

	inline float4a VX_CALLCONV max3(const float4a &a)
	{
		__m128 x = a;
		__m128 y = _mm_load_ss(&a.y);
		__m128 z = _mm_load_ss(&a.z);

		return _mm_max_ss(x, _mm_max_ss(y, z));
	}

	inline float4a VX_CALLCONV min(const float4a &a, const float4a &b)
	{
		return _mm_min_ps(a.v, b.v);
	}

	inline float4a VX_CALLCONV min3(const float4a &a)
	{
		__m128 x = a;
		__m128 y = _mm_load_ss(&a.y);
		__m128 z = _mm_load_ss(&a.z);

		return _mm_min_ss(x, _mm_min_ss(y, z));
	}

	inline __m128 VX_CALLCONV abs(CVEC4 v)
	{
		__m128 mask = g_VXAbsMaskFloat;
		return _mm_andnot_ps(mask, v);
	}

	inline __m128 VX_CALLCONV normalize(CVEC4 V)
	{
		__m128 vLengthSq = dot4(V, V);

		// Prepare for the division
		__m128 vResult = _mm_sqrt_ps(vLengthSq);
		// Create zero with a single instruction
		__m128 vZeroMask = _mm_setzero_ps();
		// Test for a divide by zero (Must be FP to detect -0.0)
		vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
		// Failsafe on zero (Or epsilon) length planes
		// If the length is infinity, set the elements to zero
		vLengthSq = _mm_cmpneq_ps(vLengthSq, g_VXInfinity);
		// Divide to perform the normalization
		vResult = _mm_div_ps(V, vResult);
		// Any that are infinity, set to zero
		vResult = _mm_and_ps(vResult, vZeroMask);
		// Select qnan or result based on infinite length
		__m128 vTemp1 = _mm_andnot_ps(vLengthSq, g_VXQNaN);
		__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
		vResult = _mm_or_ps(vTemp1, vTemp2);

		return vResult;
	}

	inline bool VX_CALLCONV Vector3IsInfinite(CVEC4 V)
	{
		// Mask off the sign bit
		__m128 vTemp = _mm_and_ps(V, g_VXAbsMask);
		// Compare to infinity
		vTemp = _mm_cmpeq_ps(vTemp, g_VXInfinity);
		// If x,y or z are infinity, the signs are true.
		return ((_mm_movemask_ps(vTemp) & 7) != 0);
	}

	inline bool VX_CALLCONV Vector3Equal(CVEC4 V1, CVEC4 V2)
	{
		__m128 vTemp = _mm_cmpeq_ps(V1, V2);
		return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
	}

	inline __m128 VX_CALLCONV VectorInBounds(CVEC4 V, CVEC4 Bounds)
	{
		// Test if less than or equal
		__m128 vTemp1 = _mm_cmple_ps(V, Bounds);
		// Negate the bounds
		__m128 vTemp2 = _mm_mul_ps(Bounds, vx::g_VXNegativeOne);
		// Test if greater or equal (Reversed)
		vTemp2 = _mm_cmple_ps(vTemp2, V);
		// Blend answers
		vTemp1 = _mm_and_ps(vTemp1, vTemp2);
		return vTemp1;
	}

	inline __m128 VX_CALLCONV VectorTrueInt()
	{
		__m128i V = _mm_set1_epi32(-1);
		return _mm_castsi128_ps(V);
	}

	inline __m128 VX_CALLCONV VectorAndInt
		(
			CVEC4 V1,
			CVEC4 V2
			)
	{
		return _mm_and_ps(V1, V2);
	}

	inline __m128 VX_CALLCONV VectorEqualInt
		(
			CVEC4 V1,
			CVEC4 V2
			)
	{
		__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
		return _mm_castsi128_ps(V);
	}

	inline __m128 VX_CALLCONV VectorOrInt
		(
			CVEC4 V1,
			CVEC4 V2
			)
	{
		__m128i V = _mm_or_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
		return _mm_castsi128_ps(V);
	}

	inline __m128 VX_CALLCONV VectorIsInfinite(CVEC4 V)
	{
		// Mask off the sign bit
		__m128 vTemp = _mm_and_ps(V, g_VXAbsMask);
		// Compare to infinity
		vTemp = _mm_cmpeq_ps(vTemp, g_VXInfinity);
		// If any are infinity, the signs are true.
		return vTemp;
	}

	inline __m128 VX_CALLCONV VectorSelect(CVEC4 V1, CVEC4 V2, CVEC4 Control)
	{
		__m128 vTemp1 = _mm_andnot_ps(Control, V1);
		__m128 vTemp2 = _mm_and_ps(V2, Control);
		return _mm_or_ps(vTemp1, vTemp2);
	}

	inline __m128 VX_CALLCONV negate(CVEC4 V)
	{
		__m128 zero = _mm_setzero_ps();

		return _mm_sub_ps(zero, V);
	}

	inline __m128 VX_CALLCONV round
		(
			CVEC4 V
			)
	{
#if _VX_SSE41
		return _mm_round_ps(V, _MM_FROUND_TO_NEAREST_INT);
#else
		__m128 sign = _mm_and_ps(V, vx::g_VXNegativeZero);

		__m128 sMagic = _mm_or_ps(vx::g_VXNoFraction, sign);

		__m128 R1 = _mm_add_ps(V, sMagic);
		R1 = _mm_sub_ps(R1, sMagic);

		__m128 R2 = _mm_and_ps(V, vx::g_VXAbsMask);
		__m128 mask = _mm_cmple_ps(R2, vx::g_VXNoFraction);
		R2 = _mm_andnot_ps(mask, V);
		R1 = _mm_and_ps(R1, mask);

		__m128 vResult = _mm_xor_ps(R1, R2);
		return vResult;
#endif
	}

	inline __m128 VX_CALLCONV cross3(CVEC4 v1, CVEC4 v2)
	{
		__m128 tmp = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 c1 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2));
		c1 = _mm_mul_ps(tmp, c1);

		tmp = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 1, 0, 2));
		__m128 c2 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 0, 2, 1));
		c2 = _mm_mul_ps(tmp, c2);

		return _mm_sub_ps(c1, c2);
	}

	inline __m128 VX_CALLCONV length2(CVEC4 V)
	{
		return _mm_sqrt_ps(dot2(V, V));
	}

	inline __m128 VX_CALLCONV length3(CVEC4 V)
	{
		return _mm_sqrt_ps(dot3(V, V));
	}

	inline __m128 VX_CALLCONV length4(CVEC4 V)
	{
		return _mm_sqrt_ps(dot4(V, V));
	}

	inline __m128 VX_CALLCONV normalizeImpl(CVEC4 VL, CVEC4 V)
	{
		__m128 vLengthSq = VL;
		// Prepare for the division
		__m128 vResult = _mm_sqrt_ps(vLengthSq);
		// Create zero with a single instruction
		__m128 vZeroMask = _mm_setzero_ps();
		// Test for a divide by zero (Must be FP to detect -0.0)
		vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
		// Failsafe on zero (Or epsilon) length planes
		// If the length is infinity, set the elements to zero
		vLengthSq = _mm_cmpneq_ps(vLengthSq, g_VXInfinity);
		// Divide to perform the normalization
		vResult = _mm_div_ps(V, vResult);
		// Any that are infinity, set to zero
		vResult = _mm_and_ps(vResult, vZeroMask);
		// Select qnan or result based on infinite length
		__m128 vTemp1 = _mm_andnot_ps(vLengthSq, g_VXQNaN);
		__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
		vResult = _mm_or_ps(vTemp1, vTemp2);
		return vResult;
	}

	inline __m128 VX_CALLCONV normalize2(CVEC4 V)
	{
		// Perform the dot product on x,y and z only
		__m128 vLengthSq = dot2(V, V);

		return normalizeImpl(vLengthSq, V);
	}

	inline __m128 VX_CALLCONV normalize3(CVEC4 V)
	{
		// Perform the dot product on x,y and z only
		__m128 vLengthSq = dot3(V, V);

		return normalizeImpl(vLengthSq, V);
	}

	inline __m128 VX_CALLCONV normalize4(CVEC4 V)
	{
		// Perform the dot product on x,y and z only
		__m128 vLengthSq = dot4(V, V);

		return normalizeImpl(vLengthSq, V);
	}

	inline __m128 VX_CALLCONV quaternionRotation
		(
		CVEC4 V,
		CVEC4 RotationQuaternion
		)
	{
		__m128 A = VectorSelect(g_VXSelect1110.v, V, g_VXSelect1110.v);
		__m128 Q = quaternionConjugate(RotationQuaternion);
		__m128 Result = quaternionMultiply(Q, A);
		return quaternionMultiply(Result, RotationQuaternion);
	}

	inline __m128 VX_CALLCONV quaternionMultiply(CVEC4 Q1, CVEC4 Q2)
	{
		static const __m128 ControlWZYX = { 1.0f, -1.0f, 1.0f, -1.0f };
		static const __m128 ControlZWXY = { 1.0f, 1.0f, -1.0f, -1.0f };
		static const __m128 ControlYXWZ = { -1.0f, 1.0f, 1.0f, -1.0f };
		// Copy to SSE registers and use as few as possible for x86
		__m128 Q2X = Q2;
		__m128 Q2Y = Q2;
		__m128 Q2Z = Q2;
		__m128 vResult = Q2;
		// Splat with one instruction
		vResult = VX_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 3, 3, 3));
		Q2X = VX_PERMUTE_PS(Q2X, _MM_SHUFFLE(0, 0, 0, 0));
		Q2Y = VX_PERMUTE_PS(Q2Y, _MM_SHUFFLE(1, 1, 1, 1));
		Q2Z = VX_PERMUTE_PS(Q2Z, _MM_SHUFFLE(2, 2, 2, 2));
		// Retire Q1 and perform Q1*Q2W
		vResult = _mm_mul_ps(vResult, Q1);
		__m128 Q1Shuffle = Q1;
		// Shuffle the copies of Q1
		Q1Shuffle = VX_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
		// Mul by Q1WZYX
		Q2X = _mm_mul_ps(Q2X, Q1Shuffle);
		Q1Shuffle = VX_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(2, 3, 0, 1));
		// Flip the signs on y and z
		Q2X = _mm_mul_ps(Q2X, ControlWZYX);
		// Mul by Q1ZWXY
		Q2Y = _mm_mul_ps(Q2Y, Q1Shuffle);
		Q1Shuffle = VX_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
		// Flip the signs on z and w
		Q2Y = _mm_mul_ps(Q2Y, ControlZWXY);
		// Mul by Q1YXWZ
		Q2Z = _mm_mul_ps(Q2Z, Q1Shuffle);
		vResult = _mm_add_ps(vResult, Q2X);
		// Flip the signs on x and w
		Q2Z = _mm_mul_ps(Q2Z, ControlYXWZ);
		Q2Y = _mm_add_ps(Q2Y, Q2Z);
		vResult = _mm_add_ps(vResult, Q2Y);
		return vResult;
	}

	inline __m128 VX_CALLCONV quaternionConjugate(CVEC4 Q)
	{
		static const __m128 NegativeOne3 = { -1.0f, -1.0f, -1.0f, 1.0f };
		return _mm_mul_ps(Q, NegativeOne3);
	}

	inline __m128 VX_CALLCONV quaternionRotationRollPitchYawFromVector(CVEC4 Angles)
	{
		static const __m128  Sign = { 1.0f, -1.0f, -1.0f, 1.0f };

		__m128 HalfAngles = _mm_mul_ps(Angles, g_VXOneHalf);

		__m128 SinAngles, CosAngles;
		sinCos(&SinAngles, &CosAngles, HalfAngles);

		__m128 P0 = VectorPermute<VX_PERMUTE_0X, VX_PERMUTE_1X, VX_PERMUTE_1X, VX_PERMUTE_1X>(SinAngles, CosAngles);
		__m128 Y0 = VectorPermute<VX_PERMUTE_1Y, VX_PERMUTE_0Y, VX_PERMUTE_1Y, VX_PERMUTE_1Y>(SinAngles, CosAngles);
		__m128 R0 = VectorPermute<VX_PERMUTE_1Z, VX_PERMUTE_1Z, VX_PERMUTE_0Z, VX_PERMUTE_1Z>(SinAngles, CosAngles);
		__m128 P1 = VectorPermute<VX_PERMUTE_0X, VX_PERMUTE_1X, VX_PERMUTE_1X, VX_PERMUTE_1X>(CosAngles, SinAngles);
		__m128 Y1 = VectorPermute<VX_PERMUTE_1Y, VX_PERMUTE_0Y, VX_PERMUTE_1Y, VX_PERMUTE_1Y>(CosAngles, SinAngles);
		__m128 R1 = VectorPermute<VX_PERMUTE_1Z, VX_PERMUTE_1Z, VX_PERMUTE_0Z, VX_PERMUTE_1Z>(CosAngles, SinAngles);

		__m128 Q1 = _mm_mul_ps(P1, Sign);
		__m128 Q0 = _mm_mul_ps(P0, Y0);
		Q1 = _mm_mul_ps(Q1, Y1);
		Q0 = _mm_mul_ps(Q0, R0);

		__m128 Q = fma(Q1, R1, Q0);

		return Q;
	}

	inline void VX_CALLCONV quaternionToAxisAngle(CVEC4 Q, __m128* pAxis, f32* pAngle)
	{
		VX_ASSERT(pAxis);
		VX_ASSERT(pAngle);

		*pAxis = Q;

		*pAngle = 2.0f * scalarACos(VectorGetW(Q));
	}

	inline void VX_CALLCONV sinCos(__m128* pSin, __m128* pCos, CVEC4 V)
	{
		// Force the value within the bounds of pi
		__m128 x = modAngles(V);

		// Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
		__m128 sign = _mm_and_ps(x, g_VXNegativeZero);
		__m128 c = _mm_or_ps(g_VXPi, sign);  // pi when x >= 0, -pi when x < 0
		__m128 absx = _mm_andnot_ps(sign, x);  // |x|
		__m128 rflx = _mm_sub_ps(c, x);
		__m128 comp = _mm_cmple_ps(absx, g_VXHalfPi);
		__m128 select0 = _mm_and_ps(comp, x);
		__m128 select1 = _mm_andnot_ps(comp, rflx);
		x = _mm_or_ps(select0, select1);
		select0 = _mm_and_ps(comp, g_VXOne);
		select1 = _mm_andnot_ps(comp, g_VXNegativeOne);
		sign = _mm_or_ps(select0, select1);

		__m128 x2 = _mm_mul_ps(x, x);

		// Compute polynomial approximation of sine
		const __m128 SC1 = g_VXSinCoefficients1;
		__m128 vConstants = VX_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 Result = _mm_mul_ps(vConstants, x2);

		const __m128 SC0 = g_VXSinCoefficients0;
		vConstants = VX_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = VX_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = VX_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = VX_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);
		Result = _mm_add_ps(Result, g_VXOne);
		Result = _mm_mul_ps(Result, x);
		*pSin = Result;

		// Compute polynomial approximation of cosine
		const __m128 CC1 = g_VXCosCoefficients1;
		vConstants = VX_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_mul_ps(vConstants, x2);

		const __m128 CC0 = g_VXCosCoefficients0;
		vConstants = VX_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = VX_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = VX_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = VX_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);
		Result = _mm_add_ps(Result, g_VXOne);
		Result = _mm_mul_ps(Result, sign);
		*pCos = Result;
	}

	inline __m128 VX_CALLCONV tan(CVEC4 V)
	{
		static const __m128 TanCoefficients0 = { 1.0f, -4.667168334e-1f, 2.566383229e-2f, -3.118153191e-4f };
		static const __m128 TanCoefficients1 = { 4.981943399e-7f, -1.333835001e-1f, 3.424887824e-3f, -1.786170734e-5f };
		static const __m128 TanConstants = { 1.570796371f, 6.077100628e-11f, 0.000244140625f, 0.63661977228f /*2 / Pi*/ };
		static const vx::vector_type_uint Mask = { { 0x1, 0x1, 0x1, 0x1 } };
		__m128 TwoDivPi = VectorSplatW(TanConstants);

		__m128 Zero = _mm_setzero_ps();

		__m128 C0 = VectorSplatX(TanConstants);
		__m128 C1 = VectorSplatY(TanConstants);
		__m128 Epsilon = VectorSplatZ(TanConstants);

		__m128 VA = _mm_mul_ps(V, TwoDivPi);

		VA = round(VA);

		__m128 VC = VectorNegativeMultiplySubtract(VA, C0, V);

		__m128 VB = vx::abs(VA);

		VC = VectorNegativeMultiplySubtract(VA, C1, VC);

		reinterpret_cast<__m128i *>(&VB)[0] = _mm_cvttps_epi32(VB);

		__m128 VC2 = _mm_mul_ps(VC, VC);

		__m128 T7 = VectorSplatW(TanCoefficients1);
		__m128 T6 = VectorSplatZ(TanCoefficients1);
		__m128 T4 = VectorSplatX(TanCoefficients1);
		__m128 T3 = VectorSplatW(TanCoefficients0);
		__m128 T5 = VectorSplatY(TanCoefficients1);
		__m128 T2 = VectorSplatZ(TanCoefficients0);
		__m128 T1 = VectorSplatY(TanCoefficients0);
		__m128 T0 = VectorSplatX(TanCoefficients0);

		__m128 VBIsEven = VectorAndInt(VB, Mask.v);
		VBIsEven = VectorEqualInt(VBIsEven, Zero);

		__m128 N = vx::fma(VC2, T7, T6);
		__m128 D = vx::fma(VC2, T4, T3);
		N = vx::fma(VC2, N, T5);
		D = vx::fma(VC2, D, T2);
		N = _mm_mul_ps(VC2, N);
		D = vx::fma(VC2, D, T1);
		N = vx::fma(VC, N, VC);
		__m128 VCNearZero = VectorInBounds(VC, Epsilon);
		D = vx::fma(VC2, D, T0);

		N = vx::VectorSelect(N, VC, VCNearZero);
		D = vx::VectorSelect(D, vx::g_VXOne, VCNearZero);

		__m128 R0 = vx::negate(N);
		__m128 R1 = _mm_div_ps(N, D);
		R0 = _mm_div_ps(D, R0);

		__m128 VIsZero = _mm_cmpeq_ps(V, Zero);

		__m128 Result = vx::VectorSelect(R0, R1, VBIsEven);

		Result = vx::VectorSelect(Result, Zero, VIsZero);

		return Result;
	}

	inline __m128 VX_CALLCONV atan(CVEC4 V)
	{
		__m128 absV = abs(V);
		__m128 invV = _mm_div_ps(g_VXOne, V);
		__m128 comp = _mm_cmpgt_ps(V, g_VXOne);
		__m128 select0 = _mm_and_ps(comp, g_VXOne);
		__m128 select1 = _mm_andnot_ps(comp, g_VXNegativeOne);
		__m128 sign = _mm_or_ps(select0, select1);
		comp = _mm_cmple_ps(absV, g_VXOne);
		select0 = _mm_and_ps(comp, g_VXZero);
		select1 = _mm_andnot_ps(comp, sign);
		sign = _mm_or_ps(select0, select1);
		select0 = _mm_and_ps(comp, V);
		select1 = _mm_andnot_ps(comp, invV);
		__m128 x = _mm_or_ps(select0, select1);

		__m128 x2 = _mm_mul_ps(x, x);

		// Compute polynomial approximation
		const __m128 TC1 = g_VXATanCoefficients1;
		__m128 vConstants = VX_PERMUTE_PS(TC1, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 Result = _mm_mul_ps(vConstants, x2);

		vConstants = VX_PERMUTE_PS(TC1, _MM_SHUFFLE(2, 2, 2, 2));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = VX_PERMUTE_PS(TC1, _MM_SHUFFLE(1, 1, 1, 1));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = VX_PERMUTE_PS(TC1, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		const __m128 TC0 = g_VXATanCoefficients0;
		vConstants = VX_PERMUTE_PS(TC0, _MM_SHUFFLE(3, 3, 3, 3));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = VX_PERMUTE_PS(TC0, _MM_SHUFFLE(2, 2, 2, 2));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = VX_PERMUTE_PS(TC0, _MM_SHUFFLE(1, 1, 1, 1));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);

		vConstants = VX_PERMUTE_PS(TC0, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_add_ps(Result, vConstants);
		Result = _mm_mul_ps(Result, x2);
		Result = _mm_add_ps(Result, g_VXOne);
		Result = _mm_mul_ps(Result, x);
		__m128 result1 = _mm_mul_ps(sign, g_VXHalfPi);
		result1 = _mm_sub_ps(result1, Result);

		comp = _mm_cmpeq_ps(sign, g_VXZero);
		select0 = _mm_and_ps(comp, Result);
		select1 = _mm_andnot_ps(comp, result1);
		Result = _mm_or_ps(select0, select1);
		return Result;
	}

	inline __m128 VX_CALLCONV atan2(CVEC4 X, CVEC4 Y)
	{
		// Return the inverse tangent of Y / X in the range of -Pi to Pi with the following exceptions:

		//     Y == 0 and X is Negative         -> Pi with the sign of Y
		//     y == 0 and x is positive         -> 0 with the sign of y
		//     Y != 0 and X == 0                -> Pi / 2 with the sign of Y
		//     Y != 0 and X is Negative         -> atan(y/x) + (PI with the sign of Y)
		//     X == -Infinity and Finite Y      -> Pi with the sign of Y
		//     X == +Infinity and Finite Y      -> 0 with the sign of Y
		//     Y == Infinity and X is Finite    -> Pi / 2 with the sign of Y
		//     Y == Infinity and X == -Infinity -> 3Pi / 4 with the sign of Y
		//     Y == Infinity and X == +Infinity -> Pi / 4 with the sign of Y

		static const __m128 ATan2Constants = { VX_PI, VX_PIDIV2, VX_PIDIV4, VX_PI * 3.0f / 4.0f };

		__m128 Zero = _mm_setzero_ps();
		__m128 ATanResultValid = g_VXMask4;//VectorTrueInt();

		__m128 Pi = VectorSplatX(ATan2Constants);
		__m128 PiOverTwo = VectorSplatY(ATan2Constants);
		__m128 PiOverFour = VectorSplatZ(ATan2Constants);
		__m128 ThreePiOverFour = VectorSplatW(ATan2Constants);

		__m128 YEqualsZero = _mm_cmpeq_ps(Y, Zero);
		__m128 XEqualsZero = _mm_cmpeq_ps(X, Zero);
		__m128 XIsPositive = _mm_and_ps(X, g_VXNegativeZero.v);
		XIsPositive = VectorEqualInt(XIsPositive, Zero);
		__m128 YEqualsInfinity = VectorIsInfinite(Y);
		__m128 XEqualsInfinity = VectorIsInfinite(X);

		__m128 YSign = _mm_and_ps(Y, g_VXNegativeZero.v);
		Pi = VectorOrInt(Pi, YSign);
		PiOverTwo = VectorOrInt(PiOverTwo, YSign);
		PiOverFour = VectorOrInt(PiOverFour, YSign);
		ThreePiOverFour = VectorOrInt(ThreePiOverFour, YSign);

		__m128 R1 = VectorSelect(Pi, YSign, XIsPositive);
		__m128 R2 = VectorSelect(ATanResultValid, PiOverTwo, XEqualsZero);
		__m128 R3 = VectorSelect(R2, R1, YEqualsZero);
		__m128 R4 = VectorSelect(ThreePiOverFour, PiOverFour, XIsPositive);
		__m128 R5 = VectorSelect(PiOverTwo, R4, XEqualsInfinity);
		__m128 Result = VectorSelect(R3, R5, YEqualsInfinity);
		ATanResultValid = VectorEqualInt(Result, ATanResultValid);

		__m128 V = _mm_div_ps(Y, X);

		__m128 R0 = atan(V);

		R1 = VectorSelect(Pi, g_VXNegativeZero, XIsPositive);
		R2 = _mm_add_ps(R0, R1);

		return VectorSelect(Result, R2, ATanResultValid);
	}

	inline __m128 VX_CALLCONV modAngles(CVEC4 Angles)
	{
		// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
		__m128 vResult = _mm_mul_ps(Angles, g_VXReciprocalTwoPi);
		// Use the inline function due to complexity for rounding

		vResult = round(vResult);
		//vResult = XMVectorRound(vResult);

		vResult = _mm_mul_ps(vResult, g_VXTwoPi);
		vResult = _mm_sub_ps(Angles, vResult);
		return vResult;
	}

	inline f32 VX_CALLCONV VectorGetW(CVEC4 V)
	{
		__m128 vTemp = VX_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
		return _mm_cvtss_f32(vTemp);
	}

	inline __m128 VX_CALLCONV VectorSplatX
		(
			CVEC4 V
			)
	{
		return VX_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
	}

	inline __m128 VX_CALLCONV VectorSplatY
		(
			CVEC4 V
			)
	{
		return VX_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	}

	inline __m128 VX_CALLCONV VectorSplatZ
		(
			CVEC4 V
			)
	{
		return VX_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	}

	inline __m128 VX_CALLCONV VectorSplatW
		(
			CVEC4 V
			)
	{
		return VX_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
	}
}
