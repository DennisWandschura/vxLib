#pragma once

#include <vxLib/types.h>

namespace vx
{
	class InStream
	{
	public:
		virtual ~InStream() {}

		virtual s32 read(u8* dst, s32 size) = 0;
	};

	class OutStream
	{
	public:
		virtual ~OutStream() {}

		virtual s32 write(const u8* src, s32 size) = 0;
	};
}