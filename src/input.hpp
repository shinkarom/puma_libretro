#pragma once

#include <cstdint>
#include "libretro.h"
#include "common.hpp"

namespace input {
	
	constexpr int inputMapping[numInputs] = {
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
		RETRO_DEVICE_ID_JOYPAD_R,
	};
	
	extern bool pressedInputs[numInputs];
	extern bool justPressedInputs[numInputs];
	extern bool justReleasedInputs[numInputs];
	extern bool previouslyPressedInputs[numInputs];
	
	bool isPressed(int keynum);
	bool isJustPressed(int keynum);
	bool isJustReleased(int keynum);
	
	void init();
	void beforeFrame();
}