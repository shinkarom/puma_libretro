#pragma once

#include <cstdint>

namespace color {
	
	void init();
	
	extern uint32_t palette16bit[65536];
	extern uint32_t palette8bit[256];
	extern uint32_t palette4bit[64];
	
	uint32_t convert16to32color(uint16_t color);
	uint16_t convert32to16color(uint32_t color);
	uint8_t convert32to8color(uint32_t color);
	uint32_t convert8to32color(uint8_t color);
	uint8_t convert32to4color(uint32_t color);
	uint32_t convert4to32color(uint8_t color);
	
}
