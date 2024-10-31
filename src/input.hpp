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
	
	extern uint16_t pressedButtons;
	extern uint16_t justPressedButtons;
	extern uint16_t justReleasedButtons;
	extern uint16_t previouslyPressedButtons;
	
	bool isPressed(int keynum);
	bool isJustPressed(int keynum);
	bool isJustReleased(int keynum);
	
	void init();
	void beforeFrame();
}