#include <vxLib\RawInput.h>

namespace vx
{
	Keyboard RawInput::s_keyboard{};
	Mouse RawInput::s_mouse{};
	vx::hybrid_vector<KeyEvent, 10> RawInput::s_keyEvents{};
	Input::KeyEventCallback RawInput::s_keyEventPressedCallback{ nullptr };
	Input::KeyEventCallback RawInput::s_keyEventReleasedCallback{ nullptr };

	RawInput::RawInput()
	{
	}

	RawInput::~RawInput()
	{
		s_keyEvents.clear();
	}

	bool RawInput::initialize(HWND window)
	{
		RAWINPUTDEVICE rid[2];

		// keyboard
		rid[0].usUsagePage = 0x01;
		rid[0].usUsage = 0x06;
		rid[0].dwFlags = 0;
		rid[0].hwndTarget = window;

		// mouse
		rid[1].usUsagePage = 0x01;
		rid[1].usUsage = 0x02;
		rid[1].dwFlags = RIDEV_NOLEGACY;
		rid[1].hwndTarget = window;

		if (RegisterRawInputDevices(rid, 2, sizeof(rid[0])) == FALSE)
		{
			GetLastError();
			puts("Could not register Raw input !");
			return false;
		}

		s_mouse.m_relative.x = 0;
		s_mouse.m_relative.y = 0;

		puts("Registered raw input");
		return true;
	}

	void RawInput::shutdown()
	{
		s_keyEvents.clear();
	}

	void RawInput::handleKeyboard(const RAWKEYBOARD &keyboard)
	{
		auto flag = keyboard.Flags;
		auto key = keyboard.VKey;

		if (flag == RI_KEY_MAKE)
		{
			s_keyboard.m_keys[key] = 1;

			s_keyEvents.push_back(KeyEvent(key, KeyEvent::Pressed));
		}
		// key up
		else if (flag == RI_KEY_BREAK)
		{
			s_keyboard.m_keys[key] = 0;
			s_keyEvents.push_back(KeyEvent(key, KeyEvent::Released));
		}
	}

	void RawInput::handleMouse(const RAWMOUSE &mouse)
	{
		if (mouse.usFlags == MOUSE_MOVE_RELATIVE)
		{
			s_mouse.m_relative.x = mouse.lLastX;
			s_mouse.m_relative.y = mouse.lLastY;
		}
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
			handleKeyboard(raw->data.keyboard);
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			// handle mouse input
			handleMouse(raw->data.mouse);
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

	U8 RawInput::isKeyPressed(U16 keyCode)
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