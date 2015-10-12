#pragma once

#include <vxLib/types.h>
#include <cmath>

typedef union float3
{
	struct
	{
		f32 x, y, z;
	};
	f32 v[3];
} float3;

inline float3 float3_add(const float3* lhs, const float3* rhs)
{
	float3 result;
	result.x = lhs->x + rhs->x;
	result.y = lhs->y + rhs->y;
	result.z = lhs->z + rhs->z;
	return result;
}

inline float3 float3_sub(const float3* lhs, const float3* rhs)
{
	float3 result;
	result.x = lhs->x - rhs->x;
	result.y = lhs->y - rhs->y;
	result.z = lhs->z - rhs->z;
	return result;
}

inline float3 float3_mul(const float3* lhs, const float3* rhs)
{
	float3 result;
	result.x = lhs->x * rhs->x;
	result.y = lhs->y * rhs->y;
	result.z = lhs->z * rhs->z;
	return result;
}

inline float3 float3_div(const float3* lhs, const float3* rhs)
{
	float3 result;
	result.x = lhs->x / rhs->x;
	result.y = lhs->y / rhs->y;
	result.z = lhs->z / rhs->z;
	return result;
}

inline f32 float3_dot(const float3* v0, const float3* v1)
{
	return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;
}

inline float3 float3_normalize(const float3* v)
{
	auto l = sqrtf(float3_dot(v, v));

	float3 result;
	result.x = v->x;
	result.y = v->y;
	result.z = v->z;

	if (l > 0.0f)
	{
		result.x /= l;
		result.y /= l;
		result.z /= l;
	}

	return result;
}

inline float3 float3_abs(const float3* v)
{
	float3 result;
	result.x = fabs(v->x);
	result.y = fabs(v->y);
	result.z = fabs(v->z);
	return result;
}

inline f32 float3_length(const float3* v)
{
	return sqrtf(float3_dot(v, v));
}

inline f32 float3_distance(const float3* v1, const float3* v2)
{
	float3 tmp = float3_sub(v2, v1);
	return sqrtf(float3_dot(&tmp, &tmp));
}

inline float3 float3_min(const float3* v0, const float3* v1)
{
	float3 result;
	result.x = fminf(v0->x, v1->x);
	result.y = fminf(v0->y, v1->y);
	result.z = fminf(v0->z, v1->z);
	return result;
}

inline float3 float3_max(const float3* v0, const float3* v1)
{
	float3 result;
	result.x = fmaxf(v0->x, v1->x);
	result.y = fmaxf(v0->y, v1->y);
	result.z = fmaxf(v0->z, v1->z);
	return result;
}

inline float3 float3_cross(const float3* v1, const float3* v2)
{
	float3 result;

	result.x = v1->y * v2->z - v1->z * v2->y;
	result.y = v1->z * v2->x - v1->x * v2->z;
	result.z = v1->x * v2->y - v1->y * v2->x;

	return result;
}

inline __m128 VX_CALLCONV loadFloat3(const float3* source)
{
	__m128 x = _mm_load_ss(&source->x);
	__m128 y = _mm_load_ss(&source->y);
	__m128 z = _mm_load_ss(&source->z);
	__m128 xy = _mm_unpacklo_ps(x, y);
	return _mm_movelh_ps(xy, z);
}

inline void VX_CALLCONV storeFloat3(float3* pDestination, __m128 V)
{
	auto T1 = _mm_shuffle_ps(V, V, _MM_SHUFFLE(1, 1, 1, 1));
	auto T2 = _mm_shuffle_ps(V, V, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(&pDestination->x, V);
	_mm_store_ss(&pDestination->y, T1);
	_mm_store_ss(&pDestination->z, T2);
}