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
#include <vxLib/RawInput.h>
#include <Windows.h>

namespace vx
{
	Keyboard RawInput::s_keyboard{};
	Mouse RawInput::s_mouse{};
	vx::hybrid_vector<KeyEvent, 10> RawInput::s_keyEvents{};
	Input::KeyEventCallback RawInput::s_keyEventPressedCallback{ nullptr };
	Input::KeyEventCallback RawInput::s_keyEventReleasedCallback{ nullptr };

	struct InputHandler
	{
		static void handleKeyboard(const RAWKEYBOARD &rawKeyboard, Keyboard* keyboard, vx::hybrid_vector<KeyEvent, 10>* keyEvents)
		{
			auto flag = rawKeyboard.Flags;
			auto key = rawKeyboard.VKey;

			if (flag == RI_KEY_MAKE)
			{
				keyboard->m_keys[key] = 1;
				keyEvents->push_back(KeyEvent(key, KeyEvent::Pressed));
			}
			// key up
			else if (flag == RI_KEY_BREAK)
			{
				keyboard->m_keys[key] = 0;
				keyEvents->push_back(KeyEvent(key, KeyEvent::Released));
			}
		}

		static void handleMouse(const RAWMOUSE &rawMouse, Mouse* mouse)
		{
			if (rawMouse.usFlags == MOUSE_MOVE_RELATIVE)
			{
				mouse->m_relative.x = rawMouse.lLastX;
				mouse->m_relative.y = rawMouse.lLastY;

				mouse->m_position.x += rawMouse.lLastX;
				mouse->m_position.y += rawMouse.lLastY;
			}
		}
	};

	RawInput::RawInput()
	{
	}

	RawInput::~RawInput()
	{
		s_keyEvents.clear();
	}

	bool RawInput::initialize(void* window)
	{
		RAWINPUTDEVICE rid[2];

		// keyboard
		rid[0].usUsagePage = 0x01;
		rid[0].usUsage = 0x06;
		rid[0].dwFlags = 0;
		rid[0].hwndTarget = (HWND)window;

		// mouse
		rid[1].usUsagePage = 0x01;
		rid[1].usUsage = 0x02;
		rid[1].dwFlags = RIDEV_NOLEGACY;
		rid[1].hwndTarget = (HWND)window;

		if (RegisterRawInputDevices(rid, 2, sizeof(rid[0])) == FALSE)
		{
			GetLastError();
			puts("Could not register Raw input !");
			return false;
		}

		s_mouse.m_position.x = 0;
		s_mouse.m_position.y = 0;
		s_mouse.m_relative.x = 0;
		s_mouse.m_relative.y = 0;

		puts("Registered raw input");
		return true;
	}

	void RawInput::shutdown()
	{
		s_keyEvents.clear();
	}

	void RawInput::update(LPARAM lparam)
	{
		UINT dwSize;
		// get size of structure
		GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

		const auto bufferSize = 64u;

		assert(dwSize <= bufferSize);

		BYTE buffer[bufferSize];

		// get input
		if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, buffer, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		{
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
			return;
		}

		auto *raw = (RAWINPUT*)buffer;

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			InputHandler::handleKeyboard(raw->data.keyboard, &s_keyboard, &s_keyEvents);
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			// handle mouse input
			InputHandler::handleMouse(raw->data.mouse, &s_mouse);
		}
	}

	void RawInput::beginFrame()
	{
		// reset relative mouse movement
		s_mouse.m_relative.x = 0;
		s_mouse.m_relative.y = 0;
	}

	void RawInput::endFrame()
	{
		for (auto &it : s_keyEvents)
		{
			switch (it.evt)
			{
			case KeyEvent::Pressed:
			{
				if (s_keyEventPressedCallback)
					s_keyEventPressedCallback(it.key);
			}break;
			case KeyEvent::Released:
			{
				if (s_keyEventReleasedCallback)
					s_keyEventReleasedCallback(it.key);
			}break;
			}
		}

		s_keyEvents.clear();
	}

	u8 RawInput::isKeyPressed(u16 keyCode)
	{
		return s_keyboard.m_keys[keyCode];
	}

	void RawInput::getKeyboard(Keyboard &keyboard)
	{
		keyboard = s_keyboard;
	}

	Mouse RawInput::getMouse()
	{
		return s_mouse;
	}

	void RawInput::setCallbackKeyPressed(Input::KeyEventCallback callback)
	{
		s_keyEventPressedCallback = callback;
	}

	void RawInput::setCallbackKeyReleased(Input::KeyEventCallback callback)
	{
		s_keyEventReleasedCallback = callback;
	}
}