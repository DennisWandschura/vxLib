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
#ifndef __VX_WINDOW_H
#define __VX_WINDOW_H
#pragma once

#include <vxLib/math/Vector.h>
#include <Windows.h>

namespace vx
{
	class Window
	{
	public:
		typedef void(*WindowCloseCallback)();

	private:
#if UNICODE
		typedef wchar_t char_type;
#else
		typedef char char_type;
#endif

		MSG m_msg;
		WindowCloseCallback m_windowCloseCallback;
		const char_type* m_windowName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;
		vx::uint2 m_windowSize{0, 0};
		bool m_bFullscreen;

		void registerWindow(const char_t* windowName, bool bFullscreen);
		bool createWindow(const vx::uint2 &windowSize);

		void close();

		void handleInput(LPARAM lParam);

	public:
		Window();
		Window(const Window&) = delete;
		Window(Window &&rhs);
		~Window();

		Window& operator=(const Window&) = delete;
		Window& operator=(Window &&rhs);

		bool initialize(const char_t *windowName, const vx::uint2 &windowSize, bool bFullscreen);

		void shutdown();

		void update();

		HWND getHwnd() const;
		HINSTANCE getHinstance() const;
		const char_type* getClassName() const;

		const vx::uint2& getSize() const;

		LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);

		void showCursor(bool b);
		// in relation to center of window
		void setCursorPos(const vx::int2 &pos);
		void setCursorPos(s32 x, s32 y);

		void setWindowCloseCallback(WindowCloseCallback fn);
		void setForeground();

		void focus();
	};
}
#endif