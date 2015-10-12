#pragma once

#include <vxLib/types.h>
#include <cmath>
#include <intrin.h>

typedef union float4
{
	struct
	{
		f32 x, y, z, w;
	};
	f32 v[4];
} float4;

inline __m128 VX_CALLCONV loadFloat4(const float4* src)
{
	return _mm_loadu_ps(src->v);
}

inline void VX_CALLCONV storeFloat4(float4* dst, __m128 src)
{
	_mm_storeu_ps(dst->v, src);
}

inline float4 float4_add(const float4* lhs, const float4* rhs)
{
	float4 result;
	_mm_store_ps(result.v, _mm_add_ps(loadFloat4(lhs), loadFloat4(rhs)));
	return result;
}

inline float4 float4_sub(const float4* lhs, const float4* rhs)
{
	float4 result;
	_mm_store_ps(result.v, _mm_sub_ps(loadFloat4(lhs), loadFloat4(rhs)));
}

inline float4 float4_mul(const float4* lhs, const float4* rhs)
{
	float4 result;
	_mm_store_ps(result.v, _mm_mul_ps(loadFloat4(lhs), loadFloat4(rhs)));
}

inline float4 float4_div(const float4* lhs, const float4* rhs)
{
	float4 result;
	_mm_store_ps(result.v, _mm_div_ps(loadFloat4(lhs), loadFloat4(rhs)));
}

inline f32 float4_dot(const float4* v0, const float4* v1)
{
	return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z + v0->w * v1->w;
}

inline float4 float4_normalize(const float4* v)
{
	auto l = sqrtf(float4_dot(v, v));

	float4 result;
	result.x = v->x;
	result.y = v->y;
	result.z = v->z;
	result.w = v->w;

	if (l > 0.0f)
	{
		result.x /= l;
		result.y /= l;
		result.z /= l;
		result.w /= l;
	}

	return result;
}

inline float4 float4_abs(const float4* v, float4* result)
{
	__m128 vResult = _mm_setzero_ps();
	__m128 VV = loadFloat4(v);

	vResult = _mm_sub_ps(vResult, VV);
	vResult = _mm_max_ps(vResult, VV);

	storeFloat4(result, vResult);
}

inline f32 float4_length(const float4* v)
{
	return sqrtf(float4_dot(v, v));
}