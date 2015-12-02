/*
The MIT License(MIT)

Copyright(c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <vxLib/Window.h>
#include <vxLib/RawInput.h>

namespace vx
{
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	Window *g_windowHandle = nullptr;

	struct Window::ColdData
	{
		const wchar_t* m_windowName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;
		vx::uint2 m_windowSize{ 0, 0 };
		bool m_bFullscreen;
	};

	Window::Window()
		:m_msg(),
		m_windowCloseCallback(nullptr),
		m_coldData()
	{
	}

	Window::Window(Window &&rhs)
		: m_msg(rhs.m_msg),
		m_windowCloseCallback(rhs.m_windowCloseCallback),
		m_coldData(rhs.m_coldData)
	{
		rhs.m_coldData = nullptr;
	}

	Window::~Window()
	{
		if (m_coldData)
		{
			delete m_coldData;
			m_coldData = nullptr;
		}
	}

	Window& Window::operator = (Window &&rhs)
	{
		if (this != &rhs)
		{
			m_msg = rhs.m_msg;
			m_windowCloseCallback = rhs.m_windowCloseCallback;
			m_coldData =rhs.m_coldData;
			rhs.m_coldData = nullptr;
		}

		return *this;
	}

	void Window::registerWindow(const wchar_t *windowName, bool bFullscreen)
	{
		// Give the application a name.
		m_coldData->m_windowName = windowName;

		m_coldData->m_bFullscreen = bFullscreen;

		// Get an external pointer to this object.	
		g_windowHandle = this;

		// Get the instance of this application.
		m_coldData->m_hinstance = GetModuleHandle(NULL);

		WNDCLASSEX wc;
		// Setup the windows class with default settings.
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_coldData->m_hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = m_coldData->m_windowName;
		wc.cbSize = sizeof(WNDCLASSEX);

		// Register the window class.
		RegisterClassEx(&wc);
	}

	bool Window::createWindow(const vx::uint2 &windowSize)
	{
		// Determine the resolution of the clients desktop screen.
		m_coldData->m_windowSize.x = GetSystemMetrics(SM_CXSCREEN);
		m_coldData->m_windowSize.y = GetSystemMetrics(SM_CYSCREEN);

		int posX, posY;
		// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
		if (m_coldData->m_bFullscreen)
		{
			DEVMODE dmScreenSettings;
			// If full screen set the screen to maximum size of the users desktop and 32bit.
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = (unsigned long)m_coldData->m_windowSize.x;
			dmScreenSettings.dmPelsHeight = (unsigned long)m_coldData->m_windowSize.y;
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			// Change the display settings to full screen.
			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

			// Set the position of the window to the top left corner.
			posX = posY = 0;
		}
		else
		{
			// If windowed then set it to 800x600 resolution.
			m_coldData->m_windowSize = windowSize;

			// Place the window in the middle of the screen.
			posX = (GetSystemMetrics(SM_CXSCREEN) - m_coldData->m_windowSize.x) / 2;
			posY = (GetSystemMetrics(SM_CYSCREEN) - m_coldData->m_windowSize.y) / 2;
		}

		// Create the window with the screen settings and get the handle to it.
		m_coldData->m_hwnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			m_coldData->m_windowName,
			m_coldData->m_windowName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX,
			posY,
			m_coldData->m_windowSize.x,
			m_coldData->m_windowSize.y,
			NULL,
			NULL,
			m_coldData->m_hinstance,
			NULL);

		if (m_coldData->m_hwnd == NULL)
			return false;

		// Bring the window up on the screen and set it as main focus.
		ShowWindow(m_coldData->m_hwnd, SW_SHOW);
		SetForegroundWindow(m_coldData->m_hwnd);
		SetFocus(m_coldData->m_hwnd);

		//SetCursorPos(posX + m_windowSize.x / 2, posY + m_windowSize.y /2);
		setCursorPos(0, 0);

		// Hide the mouse cursor.
		ShowCursor(false);

		return true;
	}

	bool Window::initialize(const wchar_t *windowName, const vx::uint2 &windowSize, bool bFullscreen, DefaultContainerAllocator<LinearAllocator> &&allocator, u32 maxKeyEvtCount)
	{
		ZeroMemory(&m_msg, sizeof(MSG));
		m_coldData = new ColdData();

		registerWindow(windowName, bFullscreen);

		if (!createWindow(windowSize))
		{
			//getLastError();
			return false;
		}

		if (!RawInput::initialize(m_coldData->m_hwnd, std::move(allocator), maxKeyEvtCount))
			return false;

		return true;
	}

	void Window::shutdown()
	{
		close();

		RawInput::shutdown();
	}

	void Window::close()
	{
		// Show the mouse cursor.
		ShowCursor(true);

		if (m_coldData)
		{
			// Fix the display settings if leaving full screen mode.
			if (m_coldData->m_bFullscreen)
				ChangeDisplaySettings(NULL, 0);

			// Remove the window.
			DestroyWindow(m_coldData->m_hwnd);
			m_coldData->m_hwnd = nullptr;

			// Remove the application instance.
			UnregisterClass(m_coldData->m_windowName, m_coldData->m_hinstance);
			m_coldData->m_hinstance = nullptr;
		}

		// Release the pointer to this class.
		g_windowHandle = nullptr;
	}

	void Window::update()
	{
		RawInput::beginFrame();

		MSG msg;
		// Handle the windows messages.
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			/*switch (msg.message)
			{
			case WM_INPUT:
			{
				//m_inputCallback(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				handleInput(msg.lParam);
				//return 0;
			}
			case WM_ACTIVATE:
			{
				if (msg.wParam == 1)
				{
					puts("WM_ACTIVATE");
					SetCursorPos(0, 0);
				}
			}
			case WM_ACTIVATEAPP:
			{
				//
				puts("WM_ACTIVATEAPP");
				if (msg.wParam == 1)
				{
					SetFocus(m_hwnd);

					RECT windowRect;
					GetWindowRect(m_hwnd, &windowRect);
					auto halfExtend = m_windowSize / 2u;

					auto mouse = RawInput::getMouse();
					SetCursorPos(0, 0);
				}

				//return 0;
			};
			case WM_CLOSE:
			{
				if (m_windowCloseCallback)
				{
					m_windowCloseCallback();
					//return 0;
				}
			}break;
			default:
				break;
			}*/
		}

		RawInput::endFrame();
	}

	void Window::handleInput(LPARAM lParam)
	{
		vx::RawInput::update(lParam);
	}

	HWND Window::getHwnd() const
	{
		return m_coldData->m_hwnd;
	}

	HINSTANCE Window::getHinstance() const
	{
		return m_coldData->m_hinstance;
	}

	LPCWSTR Window::getClassName() const
	{
		return m_coldData->m_windowName;
	}

	const vx::uint2& Window::getSize() const
	{
		return m_coldData->m_windowSize;
	}

	void Window::showCursor(bool b)
	{
		ShowCursor(b);
	}

	void Window::setCursorPos(const vx::int2 &pos)
	{
		RECT windowRect;
		GetWindowRect(m_coldData->m_hwnd, &windowRect);

		SetCursorPos(windowRect.left + m_coldData->m_windowSize.x / 2 + pos.x, windowRect.top + m_coldData->m_windowSize.y / 2 + pos.y);

		//SetCursorPos(pos.x, pos.y);
	}

	void Window::setCursorPos(s32 x, s32 y)
	{
		RECT windowRect;
		GetWindowRect(m_coldData->m_hwnd, &windowRect);

		SetCursorPos(windowRect.left + m_coldData->m_windowSize.x / 2 + x, windowRect.top + m_coldData->m_windowSize.y / 2 + y);
	}


	void Window::setWindowCloseCallback(WindowCloseCallback fn)
	{
		m_windowCloseCallback = fn;
	}

	void Window::setForeground()
	{
		SetForegroundWindow(m_coldData->m_hwnd);
	}

	void Window::focus()
	{
		SetFocus(m_coldData->m_hwnd);
	}

	LRESULT CALLBACK Window::messageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
	{
		switch (umsg)
		{
		case WM_INPUT:
		{
			handleInput(lParam);
			return 0;
		}
		case WM_ACTIVATE:
		{
			auto lo = LOWORD(wParam);
			if (lo == 1)
			{
				auto mouse = RawInput::getMouse();
				setCursorPos(mouse.m_position.x, mouse.m_position.y);
				focus();

				return 0;
			}
		}
		case WM_CLOSE:
		{
			if (m_windowCloseCallback)
			{
				m_windowCloseCallback();
				return 0;
			}
		}
		default:
		{
			return DefWindowProc(hwnd, umsg, wParam, lParam);
		}
		}
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wParam, LPARAM lParam)
	{
		switch (umessage)
		{
			// Check if the window is being closed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			//WindowHandle->close();
			return 0;
		}
		// All other messages pass to the message handler in the system class.
		default:
		{
			return g_windowHandle->messageHandler(hwnd, umessage, wParam, lParam);
		}
		}
	}
}