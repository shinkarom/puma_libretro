#include "input.hpp"

#include "common.hpp"
#include "bus.hpp"

#include <iostream>

namespace input {
	uint16_t pressedButtons;
	uint16_t justPressedButtons;
	uint16_t justReleasedButtons;
	uint16_t previouslyPressedButtons;
	
	void init() {
		previouslyPressedButtons = 0;
	}
	
	bool isPressed(int keynum) {
		if(keynum > numButtons) {
			return false;
		}
		bool result = (pressedButtons & (1<<keynum) != 0);
		return result;
	}
	
	bool isJustPressed(int keynum) {
		if(keynum > numButtons) {
			return false;
		}
		return (justPressedButtons & (1<<keynum) != 0);
	}
	
	bool isJustReleased(int keynum) {
		if(keynum > numButtons) {
			return false;
		}
		return (justReleasedButtons & (1<<keynum) != 0);
	}
	
	void beforeFrame() {
		
	}
}