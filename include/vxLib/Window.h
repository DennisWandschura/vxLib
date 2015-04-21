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
		void setCursorPos(I32 x, I32 y);

		void setWindowCloseCallback(WindowCloseCallback fn);
		void setForeground();

		void focus();
	};
}
#endif