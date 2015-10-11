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

inline void float2_add(const float2* lhs, const float2* rhs, float2* result)
{
	result->x = lhs->x + rhs->x;
	result->y = lhs->y + rhs->y;
}

inline void float2_sub(const float2* lhs, const float2* rhs, float2* result)
{
	result->x = lhs->x - rhs->x;
	result->y = lhs->y - rhs->y;
}

inline void float2_mul(const float2* lhs, const float2* rhs, float2* result)
{
	result->x = lhs->x * rhs->x;
	result->y = lhs->y * rhs->y;
}

inline void float2_div(const float2* lhs, const float2* rhs, float2* result)
{
	result->x = lhs->x / rhs->x;
	result->y = lhs->y / rhs->y;
}

inline f32 float2_dot(const float2* v0, const float2* v1)
{
	return v0->x * v1->x + v0->y * v1->y;
}

inline void float2_normalize2(const float2* v, float2* result)
{
	auto l = sqrtf(float2_dot(v, v));

	result->x = v->x;
	result->y = v->y;

	if (l > 0.0f)
	{
		result->x /= l;
		result->y /= l;
	}
}

inline void float2_abs(const float2* v, float2* result)
{
	result->x = fabs(v->x);
	result->y = fabs(v->y);
}

inline f32 float2_length(const float2* v)
{
	return sqrtf(float2_dot(v, v));
}