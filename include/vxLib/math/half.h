#pragma once

#include <vxLib/types.h>

struct f16
{
private:
	u16 m_value;

	static u16 toF16(f32 f);

public:
	f16() :m_value(0) {}
	f16(f32 f) :m_value(toF16(f)) {}

	f32 toF32() const;
};