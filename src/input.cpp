#include "input.hpp"

#include "bus.hpp"

namespace input {
	uint16_t pressedButtons;
	uint16_t justPressedButtons;
	uint16_t justReleasedButtons;
	uint16_t previouslyPressedButtons;
	
	void init() {
		previouslyPressedButtons = 0;
	}
	
	void beforeFrame() {
		bus::write16(0x60000, pressedButtons);
		bus::write16(0x60002, justPressedButtons);
		bus::write16(0x60004, justReleasedButtons);
	}
}