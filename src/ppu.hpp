#pragma once

#include <cstdint>

namespace ppu {
	
	void init();
	void deinit();
	void afterLoad();
	void beforeFrame();
	void afterFrame();
	
	void setPixel(int x, int y, uint16_t color);
	uint16_t getPixel(int x, int y);
	void queueDimensionsChange(int w, int h);
	void clear(uint32_t color);
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint32_t options);
	void setPixelType(int value);
	
	uint32_t* getBuffer();
}