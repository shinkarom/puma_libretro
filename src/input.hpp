#pragma once

#include <cstdint>
#include "libretro.h"

namespace input {
	
	constexpr int buttonMapping[12] = {
		RETRO_DEVICE_ID_JOYPAD_UP,
		RETRO_DEVICE_ID_JOYPAD_DOWN, 
		RETRO_DEVICE_ID_JOYPAD_LEFT,
		RETRO_DEVICE_ID_JOYPAD_RIGHT,
		RETRO_DEVICE_ID_JOYPAD_SELECT,
		RETRO_DEVICE_ID_JOYPAD_START,
		RETRO_DEVICE_ID_JOYPAD_A,
		RETRO_DEVICE_ID_JOYPAD_B, 
		RETRO_DEVICE_ID_JOYPAD_X,
		RETRO_DEVICE_ID_JOYPAD_Y,
		RETRO_DEVICE_ID_JOYPAD_L,
		RETRO_DEVICE_ID_JOYPAD_R
	};
	
	constexpr int keyMapping[] = {
		RETROK_BACKSPACE,
	   RETROK_TAB,
	   RETROK_RETURN,
	   RETROK_ESCAPE,
	   RETROK_SPACE,
	   RETROK_QUOTE,
	   RETROK_COMMA,
	   RETROK_MINUS,
	   RETROK_PERIOD,
	   RETROK_SLASH,
	   RETROK_0,
	   RETROK_1,
	   RETROK_2,
	   RETROK_3,
	   RETROK_4,
	   RETROK_5,
	   RETROK_6,
	   RETROK_7,
	   RETROK_8,
	   RETROK_9,
	   RETROK_SEMICOLON,
	   RETROK_EQUALS,
	   RETROK_LEFTBRACKET,
	   RETROK_BACKSLASH,
	   RETROK_RIGHTBRACKET,
	   RETROK_BACKQUOTE,
	   RETROK_a,
	   RETROK_b,
	   RETROK_c,
	   RETROK_d,
	   RETROK_e,
	   RETROK_f,
	   RETROK_g,
	   RETROK_h,
	   RETROK_i,
	   RETROK_j,
	   RETROK_k,
	   RETROK_l,
	   RETROK_m,
	   RETROK_n,
	   RETROK_o,
	   RETROK_p,
	   RETROK_q,
	   RETROK_r,
	   RETROK_s,
	   RETROK_t,
	   RETROK_u,
	   RETROK_v,
	   RETROK_w,
	   RETROK_x,
	   RETROK_y,
	   RETROK_z,
	   RETROK_DELETE,

	   RETROK_KP0,
	   RETROK_KP1,
	   RETROK_KP2,
	   RETROK_KP3,
	   RETROK_KP4,
	   RETROK_KP5,
	   RETROK_KP6,
	   RETROK_KP7,
	   RETROK_KP8,
	   RETROK_KP9,
	   RETROK_KP_PERIOD,
	   RETROK_KP_DIVIDE,
	   RETROK_KP_MULTIPLY,
	   RETROK_KP_MINUS,
	   RETROK_KP_PLUS,
	   RETROK_KP_ENTER,
	   RETROK_KP_EQUALS,

	   RETROK_UP,
	   RETROK_DOWN,
	   RETROK_RIGHT,
	   RETROK_LEFT,
	   RETROK_INSERT,
	   RETROK_HOME,
	   RETROK_END,
	   RETROK_PAGEUP,
	   RETROK_PAGEDOWN,

	   RETROK_F1,
	   RETROK_F2,
	   RETROK_F3,
	   RETROK_F4,
	   RETROK_F5,
	   RETROK_F6,
	   RETROK_F7,
	   RETROK_F8,
	   RETROK_F9,
	   RETROK_F10,
	   RETROK_F11,
	   RETROK_F12,

	   RETROK_CAPSLOCK,
	   RETROK_RSHIFT,
	   RETROK_LSHIFT,
	   RETROK_RCTRL,
	   RETROK_LCTRL,
	   RETROK_RALT,
	   RETROK_LALT,
	};
	
	extern uint16_t pressedButtons;
	extern uint16_t justPressedButtons;
	extern uint16_t justReleasedButtons;
	extern uint16_t previouslyPressedButtons;
	extern uint32_t pressedKeys[5];
	extern uint32_t justPressedKeys[5];
	extern uint32_t justReleasedKeys[5];
	extern uint32_t previouslyPressedKeys[5];
	
	void init();
	void beforeFrame();
}