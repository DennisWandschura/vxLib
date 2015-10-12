#pragma once

#include <vxLib/types.h>
#include <cmath>

typedef union float2
{
	struct
	{
		f32 x, y;
	};
	f32 v[2];
} float2;

inline float2 float2_add(const float2* lhs, const float2* rhs)
{
	float2 result;
	result.x = lhs->x + rhs->x;
	result.y = lhs->y + rhs->y;
	return result;
}

inline float2 float2_sub(const float2* lhs, const float2* rhs)
{
	float2 result;
	result.x = lhs->x - rhs->x;
	result.y = lhs->y - rhs->y;
	return result;
}

inline float2 float2_mul(const float2* lhs, const float2* rhs)
{
	float2 result;
	result.x = lhs->x * rhs->x;
	result.y = lhs->y * rhs->y;
	return result;
}

inline float2 float2_div(const float2* lhs, const float2* rhs)
{
	float2 result;
	result.x = lhs->x / rhs->x;
	result.y = lhs->y / rhs->y;
	return result;
}

inline f32 float2_dot(const float2* v0, const float2* v1)
{
	return v0->x * v1->x + v0->y * v1->y;
}

inline float2 float2_normalize(const float2* v)
{
	auto l = sqrtf(float2_dot(v, v));

	float2 result;
	result.x = v->x;
	result.y = v->y;

	if (l > 0.0f)
	{
		result.x /= l;
		result.y /= l;
	}
	return result;
}

inline float2 float2_abs(const float2* v)
{
	float2 result;
	result.x = fabs(v->x);
	result.y = fabs(v->y);
	return result;
}

inline f32 float2_length(const float2* v)
{
	return sqrtf(float2_dot(v, v));
}

inline __m128 VX_CALLCONV loadFloat2(const float2* source)
{
	__m128 x = _mm_load_ss(&source->x);
	__m128 y = _mm_load_ss(&source->y);
	return _mm_unpacklo_ps(x, y);
}

inline void VX_CALLCONV storeFloat2(float2* pDestination, __m128 V)
{
	auto T = _mm_shuffle_ps(V, V, _MM_SHUFFLE(1, 1, 1, 1));
	_mm_store_ss(&pDestination->x, V);
	_mm_store_ss(&pDestination->y, T);
}