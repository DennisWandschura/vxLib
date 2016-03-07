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
#include <vxLib/Allocator/LinearAllocator.h>
#include <Windows.h>

namespace vx
{
	Keyboard RawInput::s_keyboard{};
	Mouse RawInput::s_mouse{};
	vx::array<KeyEvent, DelegateAllocator<AllocatorBase>> RawInput::s_keyEvents{};
	vx::array<MouseEvent, DelegateAllocator<AllocatorBase>> RawInput::s_mouseEvents{};
	Input::KeyEventCallback RawInput::s_keyEventPressedCallback{ nullptr };
	Input::KeyEventCallback RawInput::s_keyEventReleasedCallback{ nullptr };
	Input::MouseEventCallback RawInput::s_mouseEventPressedCallback{nullptr};
	Input::MouseEventCallback RawInput::s_mouseEventReleasedCallback{nullptr};

	template<USHORT BUTTON_MASK_DOWN, USHORT BUTTON_MASK_UP>
	inline void handleMouseButton(vx::MouseButtons button, USHORT buttonFlags, u8 &mouseKey, const vx::int2 &mousePosition, vx::array<MouseEvent, DelegateAllocator<AllocatorBase>>* mouseEvents)
	{
		if ((buttonFlags & BUTTON_MASK_DOWN) == BUTTON_MASK_DOWN)
		{
			if (mouseKey == 0)
				mouseEvents->push_back(MouseEvent(button, vx::MouseEvent::Pressed, mousePosition));

			mouseKey = 1;
		}
		else if ((buttonFlags & BUTTON_MASK_UP) == BUTTON_MASK_UP)
		{
			if (mouseKey != 0)
				mouseEvents->push_back(MouseEvent(button, vx::MouseEvent::Released, mousePosition));

			mouseKey = 0;
		}
	}

	struct InputHandler
	{
		static void handleKeyboard(const RAWKEYBOARD &rawKeyboard, Keyboard* keyboard, vx::array<KeyEvent, DelegateAllocator<AllocatorBase>>* keyEvents)
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

		static void handleMouse(const RAWMOUSE &rawMouse, Mouse* mouse, const vx::int2 &halfSize, vx::array<MouseEvent, DelegateAllocator<AllocatorBase>>* mouseEvents)
		{
			if ((rawMouse.usFlags & MOUSE_MOVE_RELATIVE) == MOUSE_MOVE_RELATIVE)
			{
				auto offset = vx::int2(rawMouse.lLastX, -rawMouse.lLastY);
				auto newPosition = mouse->m_position + offset;

				auto newOffset = offset;
				if (newPosition.x > halfSize.x || newPosition.x < -halfSize.x)
					newOffset.x = 0;

				if (newPosition.y > halfSize.y || newPosition.y < -halfSize.y)
					newOffset.y = 0;

				mouse->m_relative = offset;
				mouse->m_position += newOffset;
			}

			auto buttonFlags = rawMouse.usButtonFlags;
			if ((buttonFlags & RI_MOUSE_WHEEL) == RI_MOUSE_WHEEL)
			{
				mouse->m_mouseWheel = *reinterpret_cast<const SHORT*>(&rawMouse.usButtonData);
			}

			auto mousePosition = mouse->m_position;
			handleMouseButton<RI_MOUSE_LEFT_BUTTON_DOWN, RI_MOUSE_LEFT_BUTTON_UP>(vx::MouseButtons::Left, buttonFlags, mouse->m_keys[0], mousePosition, mouseEvents);
			handleMouseButton<RI_MOUSE_RIGHT_BUTTON_DOWN, RI_MOUSE_RIGHT_BUTTON_UP>(vx::MouseButtons::Right, buttonFlags, mouse->m_keys[1], mousePosition, mouseEvents);
			handleMouseButton<RI_MOUSE_MIDDLE_BUTTON_DOWN, RI_MOUSE_MIDDLE_BUTTON_UP>(vx::MouseButtons::Middle, buttonFlags, mouse->m_keys[2], mousePosition, mouseEvents);
			handleMouseButton<RI_MOUSE_BUTTON_1_DOWN, RI_MOUSE_BUTTON_1_UP>(vx::MouseButtons::Button1, buttonFlags, mouse->m_keys[3], mousePosition, mouseEvents);
			handleMouseButton<RI_MOUSE_BUTTON_2_DOWN, RI_MOUSE_BUTTON_2_UP>(vx::MouseButtons::Button2, buttonFlags, mouse->m_keys[4], mousePosition, mouseEvents);
			handleMouseButton<RI_MOUSE_BUTTON_3_DOWN, RI_MOUSE_BUTTON_3_UP>(vx::MouseButtons::Button3, buttonFlags, mouse->m_keys[5], mousePosition, mouseEvents);
		}
	};

	RawInput::RawInput()
	{
	}

	RawInput::~RawInput()
	{
		s_mouseEvents.release();
		s_keyEvents.release();
	}

	bool RawInput::initialize(void* window, AllocatorBase* allocator, u32 maxEventCount, u32 maxMouseEventCount)
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
			//GetLastError();
			return false;
		}

		s_mouse.m_position.x = 0;
		s_mouse.m_position.y = 0;
		s_mouse.m_relative.x = 0;
		s_mouse.m_relative.y = 0;

		vx::array <KeyEvent, DelegateAllocator<AllocatorBase>> evtArray(allocator, maxEventCount);
		s_keyEvents.swap(evtArray);
		
		s_mouseEvents = vx::array<MouseEvent, DelegateAllocator<AllocatorBase>>(allocator, maxMouseEventCount);

		return true;
	}

	void RawInput::shutdown()
	{
		s_mouseEvents.release();
		s_keyEvents.release();
	}

	void RawInput::update(LPARAM lparam, const vx::int2 &halfSize)
	{
		UINT dwSize = 64;
		BYTE lpb[64];

		GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			InputHandler::handleKeyboard(raw->data.keyboard, &s_keyboard, &s_keyEvents);
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			// handle mouse input
			InputHandler::handleMouse(raw->data.mouse, &s_mouse, halfSize, &s_mouseEvents);
		}
	}

	void RawInput::beginFrame()
	{
		// reset relative mouse movement
		s_mouse.m_relative.x = 0;
		s_mouse.m_relative.y = 0;
		s_mouse.m_mouseWheel = 0;
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

		for (auto &it : s_mouseEvents)
		{
			switch (it.evt)
			{
			case KeyEvent::Pressed:
			{
				if (s_mouseEventPressedCallback)
					s_mouseEventPressedCallback(it.button, it.position);
			}break;
			case KeyEvent::Released:
			{
				if (s_mouseEventReleasedCallback)
					s_mouseEventReleasedCallback(it.button, it.position);
			}break;
			}
		}

		s_mouseEvents.clear();
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

	void RawInput::setCallbackMouseButtonPressed(Input::MouseEventCallback callback)
	{
		s_mouseEventPressedCallback = callback;
	}

	void RawInput::setCallbackMouseButtonReleased(Input::MouseEventCallback callback)
	{
		s_mouseEventReleasedCallback = callback;
	}
}