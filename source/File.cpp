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
#include <vxLib/File.h>
#ifdef _VX_PLATFORM_ANDROID
#include <fcntl.h>
#include <sys/stat.h>
#else
#include <strsafe.h>
#include <Windows.h>
#endif

/*void printError(DWORD error)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, 0, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, 0);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + 40)* sizeof(TCHAR));

	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("Failed with error %d: %s"),
		error, lpMsgBuf);

	printf("%ws\n", (wchar_t*)lpDisplayBuf);
}*/

namespace vx
{
	namespace FileCpp
	{
		bool createFile(const char* file, FileAccess access, File::FileHandle* handle)
		{
#ifdef _VX_PLATFORM_WINDOWS
			auto tmp = CreateFileA(file, static_cast<u32>(access), 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (tmp == INVALID_HANDLE_VALUE)
			{
				return false;
			}
#else
			auto tmp = creat(file, static_cast<s32>(access));
			if (tmp < 0)
				return false;
#endif
			*handle = tmp;
			return true;
		}

		bool openFile(const char* file, FileAccess access, File::FileHandle* handle)
		{
#ifdef _VX_PLATFORM_WINDOWS
			auto tmp = CreateFileA(file, static_cast<u32>(access), 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (tmp == INVALID_HANDLE_VALUE)
			{
				return false;
			}
#else
			auto tmp = open(file, O_TRUNC, static_cast<s32>(access));
			if (tmp < 0)
				return false;
#endif
			*handle = tmp;
			return true;
		}
	}

	File::File()
		:m_handle(0)
	{
	}

	File::~File()
	{
		close();
	}

	bool File::create(const char* file, FileAccess access)
	{
		return FileCpp::createFile(file, access, &m_handle);
	}

	bool File::open(const char *file, FileAccess access)
	{
		return FileCpp::openFile(file, access, &m_handle);
	}

#ifdef _VX_PLATFORM_WINDOWS
	bool File::create(const wchar_t* file, FileAccess access)
	{
		auto r = CreateFileW(file, static_cast<u32>(access), 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (r == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		m_handle = r;
		return true;
	}

	bool File::open(const wchar_t *file, FileAccess access)
	{
		auto r = CreateFileW(file, static_cast<u32>(access), 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (r == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		m_handle = r;
		return true;
	}
#endif

	bool File::close()
	{
#ifdef _VX_PLATFORM_WINDOWS
		bool result = false;
		if (m_handle && CloseHandle(m_handle) != 0)
		{
			m_handle = 0;
			result = true;
		}
		return result;
#else
		if (m_handle == 0)
			return true;

		return (::close(m_handle) == 0);
#endif
	}

	bool File::read(void *ptr, s32 size, s32* readBytes)
	{
#ifdef _VX_PLATFORM_WINDOWS
		return (ReadFile(m_handle, ptr, size, (DWORD*)readBytes, nullptr) != 0);
#else
		return (::read(m_handle, ptr, size) >= 0);
#endif
	}

	bool File::write(const u8 *ptr, s32 size, s32 *pWrittenBytes)
	{
#ifdef _VX_PLATFORM_WINDOWS
		return (WriteFile(m_handle, ptr, size, (DWORD*)pWrittenBytes, nullptr) != 0);
#else
		auto written = ::write(m_handle, ptr, size);
		if (written < 0)
			return false;
		*pWrittenBytes = written;
		return true;
#endif
	}

	bool File::setEof()
	{
#ifdef _VX_PLATFORM_WINDOWS
		return (SetEndOfFile(m_handle) != 0);
#else
		return (::lseek(m_handle, 0, SEEK_END) >= 0);
#endif
	}

	bool File::seek(s64 offset, FileSeekPosition from)
	{
#ifdef _VX_PLATFORM_WINDOWS
		LARGE_INTEGER tmp;
		tmp.QuadPart = offset;
		return (SetFilePointerEx(m_handle, tmp, nullptr, (u32)from) != 0);
#else
		return (::lseek(m_handle, offset, static_cast<s32>(from)) >= 0);
#endif
	}

	s64 File::getSize() const
	{
#ifdef _VX_PLATFORM_WINDOWS
		LARGE_INTEGER fileSize;
		if (GetFileSizeEx(m_handle, &fileSize) == 0)
		{
			fileSize.QuadPart = 0;
		}

		return fileSize.QuadPart;
#else
		struct stat stat_buf;
		auto rc = ::fstat(m_handle, &stat_buf);
		return rc == 0 ? stat_buf.st_size : -1;
#endif
	}

	bool File::isOpen() const
	{
		return (m_handle != 0);
	}

	bool File::flush()
	{
#ifdef _VX_PLATFORM_WINDOWS
		return (FlushFileBuffers(m_handle) != 0);
#else
		return (::fsync(m_handle) == 0);
#endif
	}
}