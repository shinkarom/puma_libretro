#pragma once

#include <cstdint>

namespace ppu {
	void init();
	void deinit();
	void afterLoad();
	void beforeFrame();
	void afterFrame();
	
	void setPixel(int x, int y, uint32_t color);
	uint32_t getPixel(int x, int y);
	
	uint32_t* getBuffer();
}