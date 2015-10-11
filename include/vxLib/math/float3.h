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

inline void float3_add(const float3* lhs, const float3* rhs, float3* result)
{
	result->x = lhs->x + rhs->x;
	result->y = lhs->y + rhs->y;
	result->z = lhs->z + rhs->z;
}

inline void float3_sub(const float3* lhs, const float3* rhs, float3* result)
{
	result->x = lhs->x - rhs->x;
	result->y = lhs->y - rhs->y;
	result->z = lhs->z - rhs->z;
}

inline void float3_mul(const float3* lhs, const float3* rhs, float3* result)
{
	result->x = lhs->x * rhs->x;
	result->y = lhs->y * rhs->y;
	result->z = lhs->z * rhs->z;
}

inline void float3_div(const float3* lhs, const float3* rhs, float3* result)
{
	result->x = lhs->x / rhs->x;
	result->y = lhs->y / rhs->y;
	result->z = lhs->z / rhs->z;
}

inline f32 float3_dot(const float3* v0, const float3* v1)
{
	return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;
}

inline void float3_normalize2(const float3* v, float3* result)
{
	auto l = sqrtf(float3_dot(v, v));

	result->x = v->x;
	result->y = v->y;
	result->z = v->z;

	if (l > 0.0f)
	{
		result->x /= l;
		result->y /= l;
		result->z /= l;
	}
}

inline void float3_abs(const float3* v, float3* result)
{
	result->x = fabs(v->x);
	result->y = fabs(v->y);
	result->z = fabs(v->z);
}


inline f32 float3_length(const float3* v)
{
	return sqrtf(float3_dot(v, v));
}