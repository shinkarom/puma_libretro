#pragma once

#include <cstdint>

namespace input {
	extern uint16_t pressedButtons;
	extern uint16_t justPressedButtons;
	extern uint16_t justReleasedButtons;
	extern uint16_t previouslyPressedButtons;
	
	void init();
	void beforeFrame();
}