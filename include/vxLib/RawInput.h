#ifndef __VX_RAWINPUT_H
#define __VX_RAWINPUT_H
#pragma once

#include <vxLib\math\Vector.h>
#include <vxLib\Container\vector.h>
#include <Windows.h>

namespace vx
{
	struct Keyboard
	{
		U8 m_keys[256];
	};

	struct Mouse
	{
		vx::int2 m_relative;
		U8 m_keys[4];
	};

	struct KeyEvent
	{
		enum Event : U8 { Pressed, Released };

		U16 key;
		Event evt;

		KeyEvent(){}
		KeyEvent(U16 _key, Event type) :evt(type), key(_key){}
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

		static void handleKeyboard(const RAWKEYBOARD &keyboard);
		static void handleMouse(const RAWMOUSE &mouse);

		RawInput();
		~RawInput();

		static bool initialize(HWND window);
		static void shutdown();

		static void update(LPARAM lparam);
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