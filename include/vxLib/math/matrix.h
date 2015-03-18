#ifndef __VX_MATRIX_H
#define __VX_MATRIX_H
#pragma once

#include <vxLib\math\Vector.h>

/*
Matrix
*/
namespace vx
{
	struct VX_ALIGN(64) mat4x4
	{
		__m128 c[4];

		mat4x4() {}

		mat4x4(F32 _11, F32 _12, F32 _13, F32 _14,
			F32 _21, F32 _22, F32 _23, F32 _24,
			F32 _31, F32 _32, F32 _33, F32 _34,
			F32 _41, F32 _42, F32 _43, F32 _44)
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

		operator float*()
		{
			return (float*)c;
		}

		operator const float*() const
		{
			return (float*)c;
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

	using mat4 = mat4x4;

	inline mat4 VX_CALLCONV add(const mat4 &M1, const mat4 &M2);
	inline mat4 VX_CALLCONV subtract(const mat4 &M1, const mat4 &M2);
	inline mat4 VX_CALLCONV mul(const mat4 &M1, const mat4 &M2);
	inline mat4 VX_CALLCONV MatrixIdentity();
	inline mat4 VX_CALLCONV MatrixTranspose(const mat4 &M);
	inline mat4 VX_CALLCONV MatrixInverse(const mat4 &M);
	inline mat4 VX_CALLCONV MatrixPerspectiveFovRH(F32 fovAngleY, F32 aspectHByW, F32 nearZ, F32 farZ);
	inline mat4 VX_CALLCONV MatrixOrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
	inline mat4 VX_CALLCONV MatrixOrthographicOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
	inline mat4 VX_CALLCONV MatrixTranslation(const F32 x, const F32 y, const F32 z);
	inline mat4 VX_CALLCONV MatrixTranslation(const __m128 &v);
	inline mat4 VX_CALLCONV MatrixRotationX(const F32 rad);
	inline mat4 VX_CALLCONV MatrixRotationY(const F32 rad);
	inline mat4 VX_CALLCONV MatrixRotationZ(const F32 rad);
	inline mat4 VX_CALLCONV MatrixScaling(const F32 x, const F32 y, const F32 z);
	inline mat4 VX_CALLCONV MatrixScaling(const __m128 &v);
	inline mat4 VX_CALLCONV MatrixRotationRollPitchYaw(const F32 roll, const F32 pitch, const F32 yaw);
	inline mat4 VX_CALLCONV MatrixRotationRollPitchYaw(const __m128 &angles);
	inline mat4 VX_CALLCONV MatrixRotationAxis(const __m128 &Axis, float Angle);
	inline mat4 VX_CALLCONV MatrixRotationNormal(const __m128 &NormalAxis, float Angle);
	inline mat4 VX_CALLCONV MatrixRotationQuaternion(const __m128 &q);
	inline mat4 VX_CALLCONV MatrixLookAtLH(const __m128 &EyePosition, const __m128 &FocusPosition, const __m128 &UpDirection);
	inline mat4 VX_CALLCONV MatrixLookToLH(const __m128 &EyePosition, const __m128 &EyeDirection, const __m128 &UpDirection);
	inline mat4 VX_CALLCONV MatrixLookToRH(const __m128 &EyePosition, const __m128 &EyeDirection, const __m128 &UpDirection);

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

#include "matrix.inl"
#endif