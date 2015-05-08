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
#include <Windows.h>
#include <strsafe.h>

void printError(DWORD error)
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
}

namespace vx
{
	File::File()
		:m_pFile(nullptr)
	{

	}

	File::~File()
	{
		close();
	}

	bool File::create(const char* file, FileAccess access)
	{
		auto r = CreateFileA(file, static_cast<u32>(access), 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (r == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		m_pFile = r;
		return true;
	}

	bool File::open(const char *file, FileAccess access)
	{
		auto r = CreateFileA(file, static_cast<u32>(access), 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (r == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		m_pFile = r;
		return true;
	}

	bool File::close()
	{
		bool result = false;
		if (m_pFile && CloseHandle(m_pFile) != 0)
		{
			m_pFile = nullptr;
			result = true;
		}

		return result;
	}

	bool File::read(void *ptr, u32 size)
	{
		return (ReadFile(m_pFile, ptr, size, nullptr, nullptr) != 0);
	}

	bool File::write(const void *ptr, u32 size, u32 *pWrittenBytes)
	{
		return (WriteFile(m_pFile, ptr, size, (DWORD*)pWrittenBytes, nullptr) != 0);
	}

	u32 File::getSize() const
	{
		LARGE_INTEGER fileSize;
		if (GetFileSizeEx(m_pFile, &fileSize) == 0)
		{
			fileSize.QuadPart = 0;
		}

		return fileSize.QuadPart;
	}
}