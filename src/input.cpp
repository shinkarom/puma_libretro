#include "input.hpp"

#include "bus.hpp"

namespace input {
	uint16_t pressedButtons;
	uint16_t justPressedButtons;
	uint16_t justReleasedButtons;
	uint16_t previouslyPressedButtons;
	uint32_t pressedKeys[4];
	uint32_t justPressedKeys[4];
	uint32_t justReleasedKeys[4];
	uint32_t previouslyPressedKeys[4];
	
	void init() {
		previouslyPressedButtons = 0;
	}
	
	void beforeFrame() {
		bus::write16(0x60000, pressedButtons);
		bus::write16(0x60002, justPressedButtons);
		bus::write16(0x60004, justReleasedButtons);
		auto a = 0x60006;
		for(int i = 0; i < 4; i++) {
			bus::write32(a, pressedKeys[i]);
			a+=4;
		}
		for(int i = 0; i < 4; i++) {
			bus::write32(a, justPressedKeys[i]);
			a+=4;
		}
		for(int i = 0; i < 4; i++) {
			bus::write32(a, justReleasedKeys[i]);
			a+=4;
		}
	}
}