#pragma once

/*
The MIT License (MIT)

Copyright (c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <vxLib/types.h>

namespace vx
{
	enum class FileAccess : u32
	{
#ifdef _VX_PLATFORM_WINDOWS
		Read = 0x80000000L,
		Write = 0x40000000L,
		Read_Write = Read | Write
#else
		Read = 0,
		Write = 00000001,
		Read_Write = 00000002
#endif
	};

	enum class FileSeekPosition : u32
	{
		Begin = 0,
		Current = 1,
		End = 2
	};

	class File
	{
	public:
#ifdef _VX_PLATFORM_WINDOWS
		typedef void* FileHandle;
#else
		typedef s32 FileHandle;
#endif

	private:
		FileHandle m_handle;

	public:
		File();
		~File();

		bool create(const char* file, FileAccess access);
		bool open(const char *file, FileAccess access);

#ifdef _VX_PLATFORM_WINDOWS
		bool create(const wchar_t* file, FileAccess access);
		bool open(const wchar_t *file, FileAccess access);
#endif

		bool close();

		bool read(void *ptr, s32 size, s32* readBytes = nullptr);

		template<typename T>
		bool read(T &value, s32* readBytes = nullptr)
		{
			return read(&value, sizeof(T), readBytes);
		}

		bool write(const u8 *ptr, s32 size, s32* pWrittenBytes = nullptr);

		template<typename T>
		bool write(const T &value)
		{
			static_assert( sizeof(T) <= vx::s32_max, "value too large");
			return write((const u8*)&value, (s32)sizeof(T), nullptr);
		}

		bool setEof();

		bool seek(s64 offset, FileSeekPosition from);

		s64 getSize() const;

		bool isOpen() const;

		bool flush();
	};
}