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
#ifndef __VX_RAWINPUT_H
#define __VX_RAWINPUT_H
#pragma once

#include <vxLib/math/Vector.h>
#include <vxLib/Container/vector.h>
#include <vxLib/Keyboard.h>

namespace vx
{
	struct Mouse
	{
		vx::int2 m_position;
		vx::int2 m_relative;
		U8 m_keys[4];
	};

	struct KeyEvent
	{
		enum Event : U8 { Pressed, Released };

		U16 key;
		Event evt;

		KeyEvent(){}
		KeyEvent(U16 _key, Event type) :key(_key), evt(type) {}
	};

	namespace Input
	{
		using KeyEventCallback = void(*)(U16 key);
	}

	class RawInput
	{
		friend class Window;

		static Keyboard s_keyboard;
		static Mouse s_mouse;
		static vx::hybrid_vector<KeyEvent, 10> s_keyEvents;
		static Input::KeyEventCallback s_keyEventPressedCallback;
		static Input::KeyEventCallback s_keyEventReleasedCallback;

		RawInput();
		~RawInput();

		static bool initialize(void* window);
		static void shutdown();

		static void update(I64 lparam);
		// call before updating input
		static void beginFrame();
		// call after gathering input events
		static void endFrame();

	public:
		static void setCallbackKeyPressed(Input::KeyEventCallback callback);
		static void setCallbackKeyReleased(Input::KeyEventCallback callback);

		static U8 isKeyPressed(U16 keyCode);
		static void getKeyboard(Keyboard &keyboard);
		static Mouse getMouse();
	};
}
#endif