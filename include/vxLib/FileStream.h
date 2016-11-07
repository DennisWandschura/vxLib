#pragma once

#include <vxLib/Stream.h>
#include <vxlib/file.h>

namespace vx
{
	class FileStream : public OutStream, public InStream
	{
		vx::File m_file;

	public:
		FileStream() :OutStream(), InStream(), m_file() {}
		~FileStream() {}

		bool open(const char* file, FileAccess access)
		{
			return m_file.open(file, access);
		}

		void close()
		{
			m_file.close();
		}

		s32 read(u8* dst, s32 size) override
		{
			s32 readBytes = 0;
			auto result = m_file.read(dst, size, &readBytes);
			VX_ASSERT(result);
			return readBytes;
		}

		s32 write(const u8* src, s32 size) override
		{
			s32 writteSize = 0;
			auto result = m_file.write(src, size, &writteSize);
			VX_ASSERT(result);
			return writteSize;
		}
	};
}