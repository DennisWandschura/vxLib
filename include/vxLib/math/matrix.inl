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
#pragma once

namespace vx
{
	inline mat4 VX_CALLCONV add(const mat4 &M1, const mat4 &M2)
	{
		mat4 mResult;

		mResult.c[0] = _mm_add_ps(M1.c[0], M2.c[0]);
		mResult.c[1] = _mm_add_ps(M1.c[1], M2.c[1]);
		mResult.c[2] = _mm_add_ps(M1.c[2], M2.c[2]);
		mResult.c[3] = _mm_add_ps(M1.c[3], M2.c[3]);

		return mResult;
	}

	inline mat4 VX_CALLCONV subtract(const mat4 &M1, const mat4 &M2)
	{
		mat4 mResult;

		mResult.c[0] = _mm_sub_ps(M1.c[0], M2.c[0]);
		mResult.c[1] = _mm_sub_ps(M1.c[1], M2.c[1]);
		mResult.c[2] = _mm_sub_ps(M1.c[2], M2.c[2]);
		mResult.c[3] = _mm_sub_ps(M1.c[3], M2.c[3]);

		return mResult;
	}

	inline mat4 VX_CALLCONV mul(const mat4 &M1, const mat4 &M2)
	{
		mat4 mResult;
		// Use vW to hold the original row
		__m128 vW = M2.c[0];
		// Splat the component X,Y,Z then W
		__m128 vX = VX_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 vY = VX_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 vZ = VX_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = VX_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		// Perform the operation on the first row
		vX = _mm_mul_ps(vX, M1.c[0]);
		vY = _mm_mul_ps(vY, M1.c[1]);
		vZ = _mm_mul_ps(vZ, M1.c[2]);
		vW = _mm_mul_ps(vW, M1.c[3]);
		// Perform a binary add to reduce cumulative errors
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.c[0] = vX;
		// Repeat for the other 3 rows
		vW = M2.c[1];
		vX = VX_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = VX_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = VX_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = VX_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M1.c[0]);
		vY = _mm_mul_ps(vY, M1.c[1]);
		vZ = _mm_mul_ps(vZ, M1.c[2]);
		vW = _mm_mul_ps(vW, M1.c[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.c[1] = vX;
		vW = M2.c[2];
		vX = VX_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = VX_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = VX_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = VX_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M1.c[0]);
		vY = _mm_mul_ps(vY, M1.c[1]);
		vZ = _mm_mul_ps(vZ, M1.c[2]);
		vW = _mm_mul_ps(vW, M1.c[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.c[2] = vX;
		vW = M2.c[3];
		vX = VX_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = VX_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = VX_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = VX_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M1.c[0]);
		vY = _mm_mul_ps(vY, M1.c[1]);
		vZ = _mm_mul_ps(vZ, M1.c[2]);
		vW = _mm_mul_ps(vW, M1.c[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.c[3] = vX;
		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixIdentity()
	{
		mat4 mResult;

		mResult.c[0] = g_VXIdentityR0;
		mResult.c[1] = g_VXIdentityR1;
		mResult.c[2] = g_VXIdentityR2;
		mResult.c[3] = g_VXIdentityR3;

		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixTranspose(const mat4 &M)
	{
		// x.x,x.y,y.x,y.y
		__m128 vTemp1 = _mm_shuffle_ps(M.c[0], M.c[1], _MM_SHUFFLE(1, 0, 1, 0));
		// x.z,x.w,y.z,y.w
		__m128 vTemp3 = _mm_shuffle_ps(M.c[0], M.c[1], _MM_SHUFFLE(3, 2, 3, 2));
		// z.x,z.y,w.x,w.y
		__m128 vTemp2 = _mm_shuffle_ps(M.c[2], M.c[3], _MM_SHUFFLE(1, 0, 1, 0));
		// z.z,z.w,w.z,w.w
		__m128 vTemp4 = _mm_shuffle_ps(M.c[2], M.c[3], _MM_SHUFFLE(3, 2, 3, 2));
		mat4 mResult;

		// x.x,y.x,z.x,w.x
		mResult.c[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
		// x.y,y.y,z.y,w.y
		mResult.c[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
		// x.z,y.z,z.z,w.z
		mResult.c[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
		// x.w,y.w,z.w,w.w
		mResult.c[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixInverse(const mat4 &M)
	{
		mat4 MT = MatrixTranspose(M);
		__m128 V00 = VX_PERMUTE_PS(MT.c[2], _MM_SHUFFLE(1, 1, 0, 0));
		__m128 V10 = VX_PERMUTE_PS(MT.c[3], _MM_SHUFFLE(3, 2, 3, 2));
		__m128 V01 = VX_PERMUTE_PS(MT.c[0], _MM_SHUFFLE(1, 1, 0, 0));
		__m128 V11 = VX_PERMUTE_PS(MT.c[1], _MM_SHUFFLE(3, 2, 3, 2));
		__m128 V02 = _mm_shuffle_ps(MT.c[2], MT.c[0], _MM_SHUFFLE(2, 0, 2, 0));
		__m128 V12 = _mm_shuffle_ps(MT.c[3], MT.c[1], _MM_SHUFFLE(3, 1, 3, 1));

		__m128 D0 = _mm_mul_ps(V00, V10);
		__m128 D1 = _mm_mul_ps(V01, V11);
		__m128 D2 = _mm_mul_ps(V02, V12);

		V00 = VX_PERMUTE_PS(MT.c[2], _MM_SHUFFLE(3, 2, 3, 2));
		V10 = VX_PERMUTE_PS(MT.c[3], _MM_SHUFFLE(1, 1, 0, 0));
		V01 = VX_PERMUTE_PS(MT.c[0], _MM_SHUFFLE(3, 2, 3, 2));
		V11 = VX_PERMUTE_PS(MT.c[1], _MM_SHUFFLE(1, 1, 0, 0));
		V02 = _mm_shuffle_ps(MT.c[2], MT.c[0], _MM_SHUFFLE(3, 1, 3, 1));
		V12 = _mm_shuffle_ps(MT.c[3], MT.c[1], _MM_SHUFFLE(2, 0, 2, 0));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		D0 = _mm_sub_ps(D0, V00);
		D1 = _mm_sub_ps(D1, V01);
		D2 = _mm_sub_ps(D2, V02);
		// V11 = D0Y,D0W,D2Y,D2Y
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
		V00 = VX_PERMUTE_PS(MT.c[1], _MM_SHUFFLE(1, 0, 2, 1));
		V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
		V01 = VX_PERMUTE_PS(MT.c[0], _MM_SHUFFLE(0, 1, 0, 2));
		V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
		// V13 = D1Y,D1W,D2W,D2W
		__m128 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
		V02 = VX_PERMUTE_PS(MT.c[3], _MM_SHUFFLE(1, 0, 2, 1));
		V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
		__m128 V03 = VX_PERMUTE_PS(MT.c[2], _MM_SHUFFLE(0, 1, 0, 2));
		V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

		__m128 C0 = _mm_mul_ps(V00, V10);
		__m128 C2 = _mm_mul_ps(V01, V11);
		__m128 C4 = _mm_mul_ps(V02, V12);
		__m128 C6 = _mm_mul_ps(V03, V13);

		// V11 = D0X,D0Y,D2X,D2X
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
		V00 = VX_PERMUTE_PS(MT.c[1], _MM_SHUFFLE(2, 1, 3, 2));
		V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
		V01 = VX_PERMUTE_PS(MT.c[0], _MM_SHUFFLE(1, 3, 2, 3));
		V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
		// V13 = D1X,D1Y,D2Z,D2Z
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
		V02 = VX_PERMUTE_PS(MT.c[3], _MM_SHUFFLE(2, 1, 3, 2));
		V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
		V03 = VX_PERMUTE_PS(MT.c[2], _MM_SHUFFLE(1, 3, 2, 3));
		V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		V03 = _mm_mul_ps(V03, V13);
		C0 = _mm_sub_ps(C0, V00);
		C2 = _mm_sub_ps(C2, V01);
		C4 = _mm_sub_ps(C4, V02);
		C6 = _mm_sub_ps(C6, V03);

		V00 = VX_PERMUTE_PS(MT.c[1], _MM_SHUFFLE(0, 3, 0, 3));
		// V10 = D0Z,D0Z,D2X,D2Y
		V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
		V10 = VX_PERMUTE_PS(V10, _MM_SHUFFLE(0, 2, 3, 0));
		V01 = VX_PERMUTE_PS(MT.c[0], _MM_SHUFFLE(2, 0, 3, 1));
		// V11 = D0X,D0W,D2X,D2Y
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
		V11 = VX_PERMUTE_PS(V11, _MM_SHUFFLE(2, 1, 0, 3));
		V02 = VX_PERMUTE_PS(MT.c[3], _MM_SHUFFLE(0, 3, 0, 3));
		// V12 = D1Z,D1Z,D2Z,D2W
		V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
		V12 = VX_PERMUTE_PS(V12, _MM_SHUFFLE(0, 2, 3, 0));
		V03 = VX_PERMUTE_PS(MT.c[2], _MM_SHUFFLE(2, 0, 3, 1));
		// V13 = D1X,D1W,D2Z,D2W
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
		V13 = VX_PERMUTE_PS(V13, _MM_SHUFFLE(2, 1, 0, 3));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		V03 = _mm_mul_ps(V03, V13);
		__m128 C1 = _mm_sub_ps(C0, V00);
		C0 = _mm_add_ps(C0, V00);
		__m128 C3 = _mm_add_ps(C2, V01);
		C2 = _mm_sub_ps(C2, V01);
		__m128 C5 = _mm_sub_ps(C4, V02);
		C4 = _mm_add_ps(C4, V02);
		__m128 C7 = _mm_add_ps(C6, V03);
		C6 = _mm_sub_ps(C6, V03);

		C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
		C0 = VX_PERMUTE_PS(C0, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = VX_PERMUTE_PS(C2, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = VX_PERMUTE_PS(C4, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = VX_PERMUTE_PS(C6, _MM_SHUFFLE(3, 1, 2, 0));
		// Get the determinate
		__m128 vTemp = Vector4Dot(C0, MT.c[0]);

		//if (pDeterminant != nullptr)
		//	*pDeterminant = vTemp;

		vTemp = _mm_div_ps(g_VXOne, vTemp);
		mat4 mResult;
		mResult.c[0] = _mm_mul_ps(C0, vTemp);
		mResult.c[1] = _mm_mul_ps(C2, vTemp);
		mResult.c[2] = _mm_mul_ps(C4, vTemp);
		mResult.c[3] = _mm_mul_ps(C6, vTemp);
		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixPerspectiveFovRH(F32 fovAngleY, F32 aspectHByW, F32 nearZ, F32 farZ)
	{
		assert(!scalarNearEqual(fovAngleY, 0.0f, 0.00001f * 2.0f));
		assert(!scalarNearEqual(aspectHByW, 0.0f, 0.00001f));
		assert(!scalarNearEqual(farZ, nearZ, 0.00001f));

		float    SinFov;
		float    CosFov;
		scalarSinCos(&SinFov, &CosFov, 0.5f * fovAngleY);
		float fRange = 1.0f / (farZ - nearZ);
		// Note: This is recorded on the stack
		float height = CosFov / SinFov;
		__m128 rMem = {
			height / aspectHByW,
			height,
			-fRange * (farZ + nearZ),
			-2.0f * fRange * nearZ * farZ
		};

		mat4 mResult;

		__m128 values = rMem;
		__m128 tmp = _mm_setzero_ps();
		mResult.c[0] = _mm_move_ss(tmp, values);

		tmp = values;
		tmp = _mm_and_ps(tmp, g_VXMaskY);
		mResult.c[1] = tmp;

		tmp = _mm_setzero_ps();
		values = _mm_shuffle_ps(values, g_VXNegIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
		// 0,0,fRange,-1.0f
		tmp = _mm_shuffle_ps(tmp, values, _MM_SHUFFLE(3, 0, 0, 0));
		mResult.c[2] = tmp;

		tmp = _mm_shuffle_ps(tmp, values, _MM_SHUFFLE(2, 1, 0, 0));
		mResult.c[3] = tmp;

		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixOrthographicRH
		(
		float ViewWidth,
		float ViewHeight,
		float NearZ,
		float FarZ
		)
	{
		assert(!scalarNearEqual(ViewWidth, 0.0f, 0.00001f));
		assert(!scalarNearEqual(ViewHeight, 0.0f, 0.00001f));
		assert(!scalarNearEqual(FarZ, NearZ, 0.00001f));

		mat4 M;
		float fRange = 1.0f / (FarZ - NearZ);
		// Note: This is recorded on the stack
		__m128 rMem = {
			2.0f / ViewWidth,
			2.0f / ViewHeight,
			-2.0f * fRange,
			-(FarZ + NearZ) * fRange
		};

		__m128 vValues = rMem;
		__m128 vTemp = _mm_setzero_ps();
		// Copy x only
		vTemp = _mm_move_ss(vTemp, vValues);
		// 2.0f / ViewWidth,0,0,0
		M.c[0] = vTemp;
		// 0,2.0f / ViewHeight,0,0
		vTemp = vValues;
		vTemp = _mm_and_ps(vTemp, g_VXMaskY);
		M.c[1] = vTemp;
		// x=fRange,y=fRange * NearZ,0,1.0f
		vTemp = _mm_setzero_ps();
		vValues = _mm_shuffle_ps(vValues, g_VXIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
		// 0,0,fRange,0.0f
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
		M.c[2] = vTemp;
		// 0,0,fRange * NearZ,1.0f
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
		M.c[3] = vTemp;

		return M;
	}

	inline mat4 VX_CALLCONV MatrixOrthographicOffCenterRH
		(
		float ViewLeft,
		float ViewRight,
		float ViewBottom,
		float ViewTop,
		float NearZ,
		float FarZ
		)
	{
		assert(!scalarNearEqual(ViewRight, ViewLeft, 0.00001f));
		assert(!scalarNearEqual(ViewTop, ViewBottom, 0.00001f));
		assert(!scalarNearEqual(FarZ, NearZ, 0.00001f));

		mat4 M;

		F32 rl = 1.0f / (ViewRight - ViewLeft);
		F32 tb = 1.0f / (ViewTop - ViewBottom);
		F32 fn = -1.0f / (FarZ - NearZ);

		__m128 rMem =
		{
			rl,
			tb,
			fn,
			1.0f
		};

		__m128 rMem2 =
		{
			-(ViewRight + ViewLeft),
			-(ViewTop + ViewBottom),
			(FarZ + NearZ),
			1.0f
		};

		__m128 vValues = rMem;
		__m128 vTemp = _mm_setzero_ps();
		// Copy x only
		vTemp = _mm_move_ss(vTemp, vValues);
		// 2 * rl
		vTemp = _mm_add_ps(vTemp, vTemp);
		M.c[0] = vTemp;

		vTemp = vValues;
		vTemp = _mm_and_ps(vTemp, g_VXMaskY);
		vTemp = _mm_add_ps(vTemp, vTemp);
		M.c[1] = vTemp;

		vTemp = vValues;
		vTemp = _mm_and_ps(vTemp, g_VXMaskZ);
		vTemp = _mm_add_ps(vTemp, vTemp);
		M.c[2] = vTemp;

		vValues = _mm_mul_ps(vValues, rMem2);
		M.c[3] = vValues;

		return M;
	}

	inline mat4 VX_CALLCONV MatrixTranslation
		(
		const F32 x,
		const F32 y,
		const F32 z
		)
	{
		mat4 mResult;

		mResult.c[0] = g_VXIdentityR0;
		mResult.c[1] = g_VXIdentityR1;
		mResult.c[2] = g_VXIdentityR2;
		mResult.c[3] = _mm_set_ps(1.0f, z, y, x);

		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixTranslation
		(
		const __m128 &v
		)
	{
		mat4 mResult;

		mResult.c[0] = g_VXIdentityR0;
		mResult.c[1] = g_VXIdentityR1;
		mResult.c[2] = g_VXIdentityR2;
		mResult.c[3] = v;

		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixRotationX
		(
		const F32 Angle
		)
	{
		float    SinAngle;
		float    CosAngle;
		scalarSinCos(&SinAngle, &CosAngle, Angle);

		auto vSin = _mm_set_ss(SinAngle);
		auto vCos = _mm_set_ss(CosAngle);
		// x = 0,y = cos,z = sin, w = 0
		vCos = _mm_shuffle_ps(vCos, vSin, _MM_SHUFFLE(3, 0, 0, 3));
		mat4 M;
		M.c[0] = g_VXIdentityR0;
		M.c[1] = vCos;
		// x = 0,y = sin,z = cos, w = 0
		vCos = VX_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 1, 2, 0));
		// x = 0,y = -sin,z = cos, w = 0
		vCos = _mm_mul_ps(vCos, g_VXNegateY);
		M.c[2] = vCos;
		M.c[3] = g_VXIdentityR3;
		return M;
	}

	inline mat4 VX_CALLCONV MatrixRotationY
		(
		const F32 rad
		)
	{
		F32 sin, cos;
		scalarSinCos(&sin, &cos, rad);

		__m128 vSin = _mm_load_ss(&sin);
		__m128 vCos = _mm_load_ss(&cos);

		// sin cos 0 0
		__m128 vTmp = _mm_unpacklo_ps(vSin, vCos);
		// sin 0 cos 0
		vTmp = _mm_shuffle_ps(vTmp, vTmp, _MM_SHUFFLE(3, 1, 2, 0));

		mat4 mResult;

		mResult.c[2] = vTmp;
		mResult.c[3] = g_VXIdentityR3;
		// cos 0 sin 0
		vTmp = _mm_shuffle_ps(vTmp, vTmp, _MM_SHUFFLE(3, 0, 1, 2));
		// cos 0 -sin 0
		vTmp = _mm_mul_ps(vTmp, g_VXNegateZ);
		mResult.c[0] = vTmp;
		mResult.c[1] = g_VXIdentityR1;

		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixRotationZ
		(
		const F32 rad
		)
	{
		F32 sin, cos;
		scalarSinCos(&sin, &cos, rad);

		__m128 vSin = _mm_load_ss(&sin);
		__m128 vCos = _mm_load_ss(&cos);

		// cos sin 0 0
		__m128 vTmp = _mm_unpacklo_ps(vCos, vSin);

		mat4 mResult;

		mResult.c[0] = vTmp;
		// sin cos 0 0
		vTmp = _mm_shuffle_ps(vTmp, vTmp, _MM_SHUFFLE(3, 2, 0, 1));
		// -sin cos 0 0
		vTmp = _mm_mul_ps(vTmp, g_VXNegateX);
		mResult.c[1] = vTmp;
		mResult.c[2] = g_VXIdentityR2;
		mResult.c[3] = g_VXIdentityR3;

		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixScaling
		(
		const F32 x,
		const F32 y,
		const F32 z
		)
	{
		mat4 mResult;

		mResult.c[0] = _mm_set_ps(0.0f, 0.0f, 0.0f, x);
		mResult.c[1] = _mm_set_ps(0.0f, 0.0f, y, 0.0f);
		mResult.c[2] = _mm_set_ps(0.0f, z, 0.0f, 0.0f);
		mResult.c[3] = g_VXIdentityR3;

		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixScaling
		(
		const __m128 &v
		)
	{
		mat4 mResult;

		mResult.c[0] = _mm_and_ps(v, g_VXMaskX);
		mResult.c[1] = _mm_and_ps(v, g_VXMaskY);
		mResult.c[2] = _mm_and_ps(v, g_VXMaskZ);
		mResult.c[3] = g_VXIdentityR3;

		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixRotationRollPitchYaw
		(
		const F32 roll, const F32 pitch, const F32 yaw
		)
	{
		__m128 angles = _mm_set_ps(0.0f, yaw, pitch, roll);
		return MatrixRotationRollPitchYaw(angles);
	}

	inline mat4 VX_CALLCONV MatrixRotationRollPitchYaw
		(
		const __m128 &angles
		)
	{
		auto q = QuaternionRotationRollPitchYawFromVector(angles);
		return MatrixRotationQuaternion(q);
	}

	inline mat4 VX_CALLCONV MatrixRotationAxis
		(
		const __m128 &Axis,
		float     Angle
		)
	{
		assert(!Vector3Equal(Axis, g_VXZero));
		assert(!Vector3IsInfinite(Axis));

		__m128 Normal = Vector3Normalize(Axis);
		return MatrixRotationNormal(Normal, Angle);
	}

	inline mat4 VX_CALLCONV MatrixRotationNormal
		(
		const __m128 &NormalAxis,
		float     Angle
		)
	{
		float    fSinAngle;
		float    fCosAngle;
		scalarSinCos(&fSinAngle, &fCosAngle, Angle);

		__m128 C2 = _mm_set_ps1(1.0f - fCosAngle);
		__m128 C1 = _mm_set_ps1(fCosAngle);
		__m128 C0 = _mm_set_ps1(fSinAngle);

		__m128 N0 = VX_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 N1 = VX_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 1, 0, 2));

		__m128 V0 = _mm_mul_ps(C2, N0);
		V0 = _mm_mul_ps(V0, N1);

		__m128 R0 = _mm_mul_ps(C2, NormalAxis);
		R0 = _mm_mul_ps(R0, NormalAxis);
		R0 = _mm_add_ps(R0, C1);

		__m128 R1 = _mm_mul_ps(C0, NormalAxis);
		R1 = _mm_add_ps(R1, V0);
		__m128 R2 = _mm_mul_ps(C0, NormalAxis);
		R2 = _mm_sub_ps(V0, R2);

		V0 = _mm_and_ps(R0, g_VXMask3);
		__m128 V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
		V1 = VX_PERMUTE_PS(V1, _MM_SHUFFLE(0, 3, 2, 1));
		__m128 V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
		V2 = VX_PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 2, 0));

		R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
		R2 = VX_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 2, 0));

		mat4 M;
		M.c[0] = R2;

		R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
		R2 = VX_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 0, 2));
		M.c[1] = R2;

		V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
		M.c[2] = V2;
		M.c[3] = g_VXIdentityR3;
		return M;
	}

	inline mat4 VX_CALLCONV MatrixRotationQuaternion
		(
		const __m128 &q
		)
	{
		const __m128 d = { 1.0f, 1.0f, 1.0f, 0.0f };

		__m128 qSquared = _mm_mul_ps(q, q);

		// x*x y*y z*z 0
		__m128 q1 = _mm_and_ps(qSquared, g_VXMask3);

		// z z y 0
		__m128 v1 = VX_PERMUTE_PS(q1, _MM_SHUFFLE(3, 1, 2, 2));
		// y x x 0
		q1 = VX_PERMUTE_PS(q1, _MM_SHUFFLE(3, 0, 0, 1));

		__m128 qd = _mm_add_ps(q1, v1);
		qd = _mm_add_ps(qd, qd);
		qd = _mm_sub_ps(d, qd);

		// x y
		// x z
		// x w

		// x y z 0
		q1 = _mm_and_ps(q, g_VXMask3);
		// 0 y z w
		v1 = _mm_shuffle_ps(q1, q, _MM_SHUFFLE(3, 2, 1, 3));
		__m128 tmp = v1;
		// y z w 0
		v1 = VX_PERMUTE_PS(v1, _MM_SHUFFLE(0, 3, 2, 1));
		// x x x 0
		q1 = VX_PERMUTE_PS(q1, _MM_SHUFFLE(3, 0, 0, 0));

		// x*y, x*z, x*w, 0
		auto q0 = _mm_mul_ps(q1, v1);
		q0 = _mm_add_ps(q0, q0);

		// y w
		// y z
		// z w

		// x y z 0
		q1 = _mm_and_ps(q, g_VXMask3);
		// y y z 0
		q1 = VX_PERMUTE_PS(q1, _MM_SHUFFLE(3, 2, 1, 1));
		// 0 y z w
		v1 = tmp;
		// w z w 0
		v1 = VX_PERMUTE_PS(v1, _MM_SHUFFLE(0, 3, 2, 3));

		// y*w, y*z, z*w, 0
		q1 = _mm_mul_ps(q1, v1);
		q1 = _mm_add_ps(q1, q1);

		mat4 mResult;

		// x*y, x*z, xy xw
		tmp = VX_PERMUTE_PS(q0, _MM_SHUFFLE(2, 0, 1, 0));
		// z*w, y*w, zw yz
		v1 = VX_PERMUTE_PS(q1, _MM_SHUFFLE(1, 2, 0, 2));
		// xy + zw, xz - wy, xy - zw, xw + yz
		const __m128 neg = { 1.0f, -1.0f, -1.0f, 1.0f };
		v1 = _mm_mul_ps(v1, neg);
		tmp = _mm_add_ps(tmp, v1);

		// x 0 0 0
		v1 = _mm_and_ps(qd, g_VXMaskX);
		// x y 0 z
		v1 = _mm_unpacklo_ps(v1, tmp);

		mResult.c[0] = VX_PERMUTE_PS(v1, _MM_SHUFFLE(2, 3, 1, 0));

		// 0 y 0 0
		v1 = _mm_and_ps(qd, g_VXMaskY);
		// x z x z
		tmp = VX_PERMUTE_PS(tmp, _MM_SHUFFLE(3, 2, 3, 2));
		// x 0 z y
		tmp = _mm_unpacklo_ps(tmp, v1);
		mResult.c[1] = VX_PERMUTE_PS(tmp, _MM_SHUFFLE(1, 2, 3, 0));

		// xz xw 0 0
		tmp = VX_PERMUTE_PS(q0, _MM_SHUFFLE(3, 3, 2, 1));
		tmp = _mm_mul_ps(tmp, g_VXNegateY);
		// yw yz 0 0
		v1 = VX_PERMUTE_PS(q1, _MM_SHUFFLE(3, 3, 1, 0));
		// xz + yw, -xw + yz, 0, 0
		tmp = _mm_add_ps(tmp, v1);

		// 0 0 z 0
		v1 = _mm_and_ps(qd, g_VXMaskZ);
		// x y 0 0
		tmp = _mm_shuffle_ps(tmp, v1, _MM_SHUFFLE(0, 2, 1, 0));

		mResult.c[2] = tmp;

		mResult.c[3] = g_VXIdentityR3;

		return mResult;
	}

	inline mat4 VX_CALLCONV MatrixLookAtLH
		(
		const __m128 &EyePosition,
		const __m128 &FocusPosition,
		const __m128 &UpDirection
		)
	{
		__m128 EyeDirection = _mm_sub_ps(FocusPosition, EyePosition);
		return MatrixLookToLH(EyePosition, EyeDirection, UpDirection);
	}

	inline mat4 VX_CALLCONV MatrixLookToLH
		(
		const __m128 &EyePosition,
		const __m128 &EyeDirection,
		const __m128 &UpDirection
		)
	{
		assert(!Vector3Equal(EyeDirection, g_VXZero));
		assert(!Vector3IsInfinite(EyeDirection));
		assert(!Vector3Equal(UpDirection, g_VXZero));
		assert(!Vector3IsInfinite(UpDirection));

		__m128 R2 = Vector3Normalize(EyeDirection);

		__m128 R0 = Vector3Cross(UpDirection, R2);
		R0 = Vector3Normalize(R0);

		__m128 R1 = Vector3Cross(R2, R0);

		__m128 NegEyePosition = VectorNegate(EyePosition);

		__m128 D0 = Vector3Dot(R0, NegEyePosition);
		__m128 D1 = Vector3Dot(R1, NegEyePosition);
		__m128 D2 = Vector3Dot(R2, NegEyePosition);

		mat4 M;

		// r0x r1x r0y r1y
		__m128 tmp = _mm_unpacklo_ps(R0, R1);
		// r0x r1x r2x r2y
		tmp = _mm_shuffle_ps(tmp, R2, _MM_SHUFFLE(1, 0, 1, 0));
		M.c[0] = _mm_and_ps(tmp, g_VXMask3);

		// r0x r1x r0y r1y
		tmp = _mm_unpacklo_ps(R0, R1);
		// r0y r1y r2y r2x
		tmp = _mm_shuffle_ps(tmp, R2, _MM_SHUFFLE(0, 1, 3, 2));
		M.c[1] = _mm_and_ps(tmp, g_VXMask3);

		// r0z r1z r0w r1w
		tmp = _mm_unpackhi_ps(R0, R1);
		// r0z r1z r2z r2z
		tmp = _mm_shuffle_ps(tmp, R2, _MM_SHUFFLE(2, 2, 1, 0));
		M.c[2] = _mm_and_ps(tmp, g_VXMask3);

		// x y x y
		auto xy = _mm_unpacklo_ps(D0, D1);
		// z z x y
		tmp = _mm_movehl_ps(xy, D2);
		// 1 z x y
		tmp = _mm_move_ss(tmp, g_VXIdentityR0);
		// x y z 1
		tmp = VX_PERMUTE_PS(tmp, _MM_SHUFFLE(0, 1, 3, 2));
		M.c[3] = tmp;

		return M;
	}

	inline mat4 VX_CALLCONV MatrixLookToRH
		(
		const __m128 &EyePosition,
		const __m128 &EyeDirection,
		const __m128 &UpDirection
		)
	{
		__m128 NegEyeDirection = VectorNegate(EyeDirection);
		return MatrixLookToLH(EyePosition, NegEyeDirection, UpDirection);
	}

	inline __m128 VX_CALLCONV Vector3TransformCoord(const mat4 &M, const __m128 &V)
	{
		__m128 Z = VX_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 Y = VX_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 X = VX_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

		__m128 Result = _mm_fmadd_ps(Z, M.c[2], M.c[3]);
		Result = _mm_fmadd_ps(Y, M.c[1], Result);
		Result = _mm_fmadd_ps(X, M.c[0], Result);

		__m128 W = VX_PERMUTE_PS(Result, _MM_SHUFFLE(3, 3, 3, 3));
		return _mm_div_ps(Result, W);
	}

	inline __m128 VX_CALLCONV Vector4Transform(const mat4 &M, const __m128 &V)
	{
		// Splat x,y,z and w
		__m128 vTempX = VX_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 vTempY = VX_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 vTempZ = VX_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 vTempW = VX_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
		// Mul by the matrix
		vTempX = _mm_mul_ps(vTempX, M.c[0]);
		vTempY = _mm_mul_ps(vTempY, M.c[1]);
		vTempZ = _mm_mul_ps(vTempZ, M.c[2]);
		vTempW = _mm_mul_ps(vTempW, M.c[3]);
		// Add them all together
		vTempX = _mm_add_ps(vTempX, vTempY);
		vTempZ = _mm_add_ps(vTempZ, vTempW);
		vTempX = _mm_add_ps(vTempX, vTempZ);

		return vTempX;
	}
}