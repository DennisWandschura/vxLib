#pragma once

#include <vxLib/StringID.h>

namespace vx
{
	struct Variant
	{
		union
		{
			U8 u8;
			I8 i8;
			U16 u16;
			I16 i16;
			U32 u32;
			I32 i32;
			U64 u64;
			I64 i64;
			F32 f32;
			F64 f64;
			vx::StringID sid;
			void* ptr;
		};

		Variant(){}
		Variant(vx::StringID sid_) :sid(sid_){}
		Variant(U32 u) :u32(u){}
		Variant(F32 f) :f32(f){}
		Variant(void* p) :ptr(p){}
	};
}