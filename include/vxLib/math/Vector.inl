#pragma once

namespace vx
{
	inline __m128 VX_CALLCONV normalize(__m128 V)
	{
		auto vLengthSq = _mm_dp_ps(V, V, 255);

		// Prepare for the division
		auto vResult = _mm_sqrt_ps(vLengthSq);
		// Create zero with a single instruction
		auto vZeroMask = _mm_setzero_ps();
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
		auto vTemp1 = _mm_andnot_ps(vLengthSq, g_VXQNaN);
		auto vTemp2 = _mm_and_ps(vResult, vLengthSq);
		vResult = _mm_or_ps(vTemp1, vTemp2);

		return vResult;
	}

	inline bool VX_CALLCONV Vector3IsInfinite(__m128 V)
	{
		// Mask off the sign bit
		__m128 vTemp = _mm_and_ps(V, g_VXAbsMask);
		// Compare to infinity
		vTemp = _mm_cmpeq_ps(vTemp, g_VXInfinity);
		// If x,y or z are infinity, the signs are true.
		return ((_mm_movemask_ps(vTemp) & 7) != 0);
	}

	inline bool VX_CALLCONV Vector3Equal(__m128 V1, __m128 V2)
	{
		auto vTemp = _mm_cmpeq_ps(V1, V2);
		return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
	}

	inline __m128 VX_CALLCONV VectorSelect(__m128 V1, __m128 V2, __m128 Control)
	{
		auto vTemp1 = _mm_andnot_ps(Control, V1);
		auto vTemp2 = _mm_and_ps(V2, Control);
		return _mm_or_ps(vTemp1, vTemp2);
	}

	inline __m128 VX_CALLCONV VectorNegate(__m128 V)
	{
		auto zero = _mm_setzero_ps();

		return _mm_sub_ps(zero, V);
	}

	inline __m128 VX_CALLCONV Vector3Cross(__m128 v1, __m128 v2)
	{
		auto tmp = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1));
		auto c1 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2));
		c1 = _mm_mul_ps(tmp, c1);

		tmp = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 1, 0, 2));
		auto c2 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 0, 2, 1));
		c2 = _mm_mul_ps(tmp, c2);

		return _mm_sub_ps(c1, c2);
	}

	inline __m128 VX_CALLCONV Vector3Dot
		(
		__m128 V1,
		__m128 V2
		)
	{
		return _mm_dp_ps(V1, V2, _VX_DOT(1, 1, 1, 1, 1, 1, 1, 0));
	}

	inline __m128 VX_CALLCONV Vector4Dot
		(
		__m128 V1,
		__m128 V2
		)
	{
		return _mm_dp_ps(V1, V2, 255);
	}

	inline __m128 VX_CALLCONV Vector3Normalize
		(
		__m128 V
		)
	{
		// Perform the dot product on x,y and z only
		auto vLengthSq = _mm_dp_ps(V, V, _VX_DOT(1, 1, 1, 1, 1, 1, 1, 0));

		// Prepare for the division
		auto vResult = _mm_sqrt_ps(vLengthSq);
		// Create zero with a single instruction
		auto vZeroMask = _mm_setzero_ps();
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
		auto vTemp1 = _mm_andnot_ps(vLengthSq, g_VXQNaN);
		auto vTemp2 = _mm_and_ps(vResult, vLengthSq);
		vResult = _mm_or_ps(vTemp1, vTemp2);
		return vResult;
	}

	inline __m128 VX_CALLCONV Vector3Rotate
		(
		__m128 V,
		__m128 RotationQuaternion
		)
	{
		__m128 A = VectorSelect(g_VXSelect1110.v, V, g_VXSelect1110.v);
		__m128 Q = QuaternionConjugate(RotationQuaternion);
		__m128 Result = QuaternionMultiply(Q, A);
		return QuaternionMultiply(Result, RotationQuaternion);
	}

	inline __m128 VX_CALLCONV QuaternionMultiply(__m128 Q1, __m128 Q2)
	{
		static const __m128 ControlWZYX = { 1.0f, -1.0f, 1.0f, -1.0f };
		static const __m128 ControlZWXY = { 1.0f, 1.0f, -1.0f, -1.0f };
		static const __m128 ControlYXWZ = { -1.0f, 1.0f, 1.0f, -1.0f };
		// Copy to SSE registers and use as few as possible for x86
		auto Q2X = Q2;
		auto Q2Y = Q2;
		auto Q2Z = Q2;
		auto vResult = Q2;
		// Splat with one instruction
		vResult = VX_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 3, 3, 3));
		Q2X = VX_PERMUTE_PS(Q2X, _MM_SHUFFLE(0, 0, 0, 0));
		Q2Y = VX_PERMUTE_PS(Q2Y, _MM_SHUFFLE(1, 1, 1, 1));
		Q2Z = VX_PERMUTE_PS(Q2Z, _MM_SHUFFLE(2, 2, 2, 2));
		// Retire Q1 and perform Q1*Q2W
		vResult = _mm_mul_ps(vResult, Q1);
		auto Q1Shuffle = Q1;
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

	inline __m128 VX_CALLCONV QuaternionConjugate(__m128 Q)
	{
		static const __m128 NegativeOne3 = { -1.0f, -1.0f, -1.0f, 1.0f };
		return _mm_mul_ps(Q, NegativeOne3);
	}

	inline __m128 VX_CALLCONV QuaternionRotationRollPitchYawFromVector(__m128 Angles)
	{
		static const __m128  Sign = { 1.0f, -1.0f, -1.0f, 1.0f };

		auto HalfAngles = _mm_mul_ps(Angles, g_VXOneHalf);

		__m128 SinAngles, CosAngles;
		VectorSinCos(&SinAngles, &CosAngles, HalfAngles);

		auto P0 = VectorPermute<VX_PERMUTE_0X, VX_PERMUTE_1X, VX_PERMUTE_1X, VX_PERMUTE_1X>(SinAngles, CosAngles);
		auto Y0 = VectorPermute<VX_PERMUTE_1Y, VX_PERMUTE_0Y, VX_PERMUTE_1Y, VX_PERMUTE_1Y>(SinAngles, CosAngles);
		auto R0 = VectorPermute<VX_PERMUTE_1Z, VX_PERMUTE_1Z, VX_PERMUTE_0Z, VX_PERMUTE_1Z>(SinAngles, CosAngles);
		auto P1 = VectorPermute<VX_PERMUTE_0X, VX_PERMUTE_1X, VX_PERMUTE_1X, VX_PERMUTE_1X>(CosAngles, SinAngles);
		auto Y1 = VectorPermute<VX_PERMUTE_1Y, VX_PERMUTE_0Y, VX_PERMUTE_1Y, VX_PERMUTE_1Y>(CosAngles, SinAngles);
		auto R1 = VectorPermute<VX_PERMUTE_1Z, VX_PERMUTE_1Z, VX_PERMUTE_0Z, VX_PERMUTE_1Z>(CosAngles, SinAngles);

		auto Q1 = _mm_mul_ps(P1, Sign);
		auto Q0 = _mm_mul_ps(P0, Y0);
		Q1 = _mm_mul_ps(Q1, Y1);
		Q0 = _mm_mul_ps(Q0, R0);

		auto Q = _mm_fmadd_ps(Q1, R1, Q0);

		return Q;
	}

	inline void VX_CALLCONV VectorSinCos(__m128* pSin, __m128* pCos, __m128 V)
	{
		// Force the value within the bounds of pi
		__m128 x = VectorModAngles(V);

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
		const auto CC1 = g_VXCosCoefficients1;
		vConstants = VX_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_mul_ps(vConstants, x2);

		const auto CC0 = g_VXCosCoefficients0;
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

	inline __m128 VX_CALLCONV VectorModAngles(__m128 Angles)
	{
		// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
		auto vResult = _mm_mul_ps(Angles, g_VXReciprocalTwoPi);
		// Use the inline function due to complexity for rounding

		vResult = _mm_round_ps(vResult, _MM_FROUND_TO_NEAREST_INT);
		//vResult = XMVectorRound(vResult);

		vResult = _mm_mul_ps(vResult, g_VXTwoPi);
		vResult = _mm_sub_ps(Angles, vResult);
		return vResult;
	}
}