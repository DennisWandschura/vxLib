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

#include <vxLib/math/Vector.h>

/*
Matrix
*/
namespace vx
{
	struct VX_ALIGN(16) mat4x4
	{
		__m128 c[4];

		mat4x4():c() {}

		mat4x4(f32 _11, f32 _12, f32 _13, f32 _14,
			f32 _21, f32 _22, f32 _23, f32 _24,
			f32 _31, f32 _32, f32 _33, f32 _34,
			f32 _41, f32 _42, f32 _43, f32 _44)
		{
			c[0] = _mm_set_ps(_41, _31, _21, _11);
			c[1] = _mm_set_ps(_42, _32, _22, _12);
			c[2] = _mm_set_ps(_43, _33, _23, _13);
			c[3] = _mm_set_ps(_44, _34, _24, _14);
		}

		// column major
		mat4x4(const __m128 &c0, const __m128 &c1, const __m128 &c2, const __m128 &c3)
		{
			c[0] = c0;
			c[1] = c1;
			c[2] = c2;
			c[3] = c3;
		}

		mat4x4(const mat4x4 &rhs)
		{
			c[0] = rhs.c[0];
			c[1] = rhs.c[1];
			c[2] = rhs.c[2];
			c[3] = rhs.c[3];
		}

		~mat4x4()
		{
		}

		operator float*()
		{
			return (float*)c;
		}

		operator const float*() const
		{
			return (float*)c;
		}

		const __m128& operator[](u32 i) const
		{
			return c[i];
		}

		mat4x4& operator=(const mat4x4 &rhs)
		{
			c[0] = rhs.c[0];
			c[1] = rhs.c[1];
			c[2] = rhs.c[2];
			c[3] = rhs.c[3];

			return *this;
		}
	};

	struct VX_ALIGN(32) mat4x4d
	{
		__m256d c[4];

		mat4x4d() :c() {}

		// column major
		mat4x4d(const __m256d &c0, const __m256d &c1, const __m256d &c2, const __m256d &c3)
		{
			c[0] = c0;
			c[1] = c1;
			c[2] = c2;
			c[3] = c3;
		}

		mat4x4d(const mat4x4d &rhs)
		{
			c[0] = rhs.c[0];
			c[1] = rhs.c[1];
			c[2] = rhs.c[2];
			c[3] = rhs.c[3];
		}
		
		~mat4x4d()
		{
		}

		operator f64*()
		{
			return (f64*)c;
		}

		operator const f64*() const
		{
			return (f64*)c;
		}

		mat4x4d& operator=(const mat4x4d &rhs)
		{
			c[0] = rhs.c[0];
			c[1] = rhs.c[1];
			c[2] = rhs.c[2];
			c[3] = rhs.c[3];

			return *this;
		}

		void asFloat(mat4x4* m)
		{
			m->c[0] = _mm256_cvtpd_ps(c[0]);
			m->c[1] = _mm256_cvtpd_ps(c[1]);
			m->c[2] = _mm256_cvtpd_ps(c[2]);
			m->c[3] = _mm256_cvtpd_ps(c[3]);
		}
	};

	typedef mat4x4 mat4;
	typedef mat4x4d mat4d;

	inline mat4 VX_CALLCONV add(const mat4 &M1, const mat4 &M2);
	inline mat4 VX_CALLCONV subtract(const mat4 &M1, const mat4 &M2);
	inline mat4 VX_CALLCONV mul(const mat4 &M1, const mat4 &M2);
	inline mat4 VX_CALLCONV MatrixIdentity();
	inline mat4 VX_CALLCONV MatrixTranspose(const mat4 &M);
	inline mat4 VX_CALLCONV MatrixInverse(const mat4 &M);

	inline mat4 VX_CALLCONV MatrixPerspectiveFovRH(f32 fovAngleY, f32 aspectHByW, f32 nearZ, f32 farZ);
	inline mat4 VX_CALLCONV MatrixPerspectiveFovRHDX(f32 FovAngleY, f32 AspectHByW, f32 NearZ, f32 FarZ);
	inline mat4 VX_CALLCONV MatrixPerspectiveFovLHDX(f32 FovAngleY, f32 AspectHByW, f32 NearZ, f32 FarZ);

	inline mat4 VX_CALLCONV MatrixOrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
	inline mat4 VX_CALLCONV MatrixOrthographicRHDX(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
	inline mat4 VX_CALLCONV MatrixOrthographicOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);

	inline mat4 VX_CALLCONV MatrixTranslation(const f32 x, const f32 y, const f32 z);
	inline mat4 VX_CALLCONV MatrixTranslation(const __m128 &v);

	inline mat4 VX_CALLCONV MatrixRotationX(const f32 rad);
	inline mat4 VX_CALLCONV MatrixRotationY(const f32 rad);
	inline mat4 VX_CALLCONV MatrixRotationZ(const f32 rad);

	inline mat4 VX_CALLCONV MatrixScaling(const f32 x, const f32 y, const f32 z);
	inline mat4 VX_CALLCONV MatrixScaling(const __m128 &v);

	inline mat4 VX_CALLCONV MatrixRotationRollPitchYaw(const f32 roll, const f32 pitch, const f32 yaw);
	inline mat4 VX_CALLCONV MatrixRotationRollPitchYaw(const __m128 &angles);
	inline mat4 VX_CALLCONV MatrixRotationAxis(const __m128 &Axis, float Angle);
	inline mat4 VX_CALLCONV MatrixRotationNormal(const __m128 &NormalAxis, float Angle);
	inline mat4 VX_CALLCONV MatrixRotationQuaternion(const __m128 &q);

	inline mat4 VX_CALLCONV MatrixLookAtLH(const __m128 &EyePosition, const __m128 &FocusPosition, const __m128 &UpDirection);
	inline mat4 VX_CALLCONV MatrixLookToLH(const __m128 &EyePosition, const __m128 &EyeDirection, const __m128 &UpDirection);
	inline mat4 VX_CALLCONV MatrixLookToRH(const __m128 &EyePosition, const __m128 &EyeDirection, const __m128 &UpDirection);

	inline mat4d VX_CALLCONV mul(const mat4d &M1, const mat4d &M2);

	inline mat4d VX_CALLCONV MatrixLookToLH(const __m256d EyePosition, const __m256d EyeDirection, const __m256d &UpDirection);
	inline mat4d VX_CALLCONV MatrixLookToRH(const __m256d EyePosition, const __m256d EyeDirection, const __m256d &UpDirection);

	inline mat4d VX_CALLCONV MatrixPerspectiveFovRHDX(f64 FovAngleY, f64 AspectHByW, f64 NearZ, f64 FarZ);
	inline mat4d VX_CALLCONV MatrixPerspectiveFovLHDX(f64 FovAngleY, f64 AspectHByW, f64 NearZ, f64 FarZ);

	inline __m128 VX_CALLCONV Vector3TransformCoord(const mat4 &M, const __m128 &V);
	inline __m128 VX_CALLCONV Vector4Transform(const mat4 &M, const __m128 &V);
}

inline vx::mat4 VX_CALLCONV operator +(const vx::mat4 &lhs, const vx::mat4 &rhs)
{
	return vx::add(lhs, rhs);
}

inline vx::mat4 VX_CALLCONV operator -(const vx::mat4 &lhs, const vx::mat4 &rhs)
{
	return vx::subtract(lhs, rhs);
}

inline vx::mat4 VX_CALLCONV operator *(const vx::mat4 &lhs, const vx::mat4 &rhs)
{
	return vx::mul(lhs, rhs);
}

inline vx::mat4d VX_CALLCONV operator *(const vx::mat4d &lhs, const vx::mat4d &rhs)
{
	return vx::mul(lhs, rhs);
}

#include "matrix.inl"